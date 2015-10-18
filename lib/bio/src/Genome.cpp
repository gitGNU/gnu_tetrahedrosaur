/***************************************************************************
 *   Copyright (C) 2015 Andrey Timashov                                    *
 *                                                                         *
 *   This file is part of Tetrahedrosaur.                                  *
 *                                                                         *
 *   Tetrahedrosaur is free software: you can redistribute it and/or       *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   Tetrahedrosaur is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Tetrahedrosaur. If not, see <http://www.gnu.org/licenses/> *
 ***************************************************************************/


#include <cstring>


#include "algo/Alignment.hpp"
#include "algo/distance.hpp"
#include "algo/pairing.hpp"


#include "Config.hpp"
#include "crossingover.hpp"
#include "GeneInitializer.hpp"
#include "Genome.hpp"
#include "InstructionSet.hpp"
#include "MutationParams.hpp"


namespace bio {


namespace {

float _symmetricCompare(
   const Chromosome & lhs,
   const Chromosome & rhs,
   const InstructionEquals & equals
)
{
   return algo::getLevenshteinDistanceSimilarity(
      lhs.code(),
      rhs.code(),
      equals
   );
}

} // anonymous namespace;


/***************************************************************************
 *   Genome class implementation                                           *
 ***************************************************************************/


Genome::Genome(
   boost::shared_ptr<const Config> config,
   const MutationParams & mutationParams
) : m_config(config)
{
   const size_t codeSize = 1 + (rand() % mutationParams.maxCodeSize);
   std::vector<Instruction> code;
   code.reserve(codeSize);
   for (size_t i = 0; i < codeSize; ++i)
   {
      code.push_back(Instruction(rand() % 256, rand() % 256, rand() % 65536));
   }
   m_chromosomes.push_back(Chromosome(code));

   // Build homologous pairs;
   const InstructionEquals instrEquals(m_config);
   auto compare = std::bind(
      _symmetricCompare,
      std::placeholders::_1,
      std::placeholders::_2,
      instrEquals
   );
   m_pairs = std::move(algo::pairing::resolve<float>(
      m_chromosomes,
      compare,
      algo::pairing::MS_MORE_METRIC_MORE_ALIKE,
      0.51
   ));

   initializeGenes();
}


Genome::Genome(
   boost::shared_ptr<const Config> config,
   const std::vector<Chromosome> & diploid
) : m_config(config), m_chromosomes(diploid)
{
   // Build homologous pairs;
   const InstructionEquals instrEquals(m_config);
   auto compare = std::bind(
      _symmetricCompare,
      std::placeholders::_1,
      std::placeholders::_2,
      instrEquals
   );
   m_pairs = std::move(algo::pairing::resolve<float>(
      m_chromosomes,
      compare,
      algo::pairing::MS_MORE_METRIC_MORE_ALIKE,
      0.51
   ));

   initializeGenes();
}


Genome::Genome(
   boost::shared_ptr<const Config> config,
   const std::vector<Chromosome> & haploidLeft,
   const std::vector<Chromosome> & haploidRight
) : m_config(config)
{
   // Copy chromosomes;
   m_chromosomes.reserve(haploidLeft.size() + haploidRight.size());
   m_chromosomes.insert(
      m_chromosomes.end(),
      haploidLeft.begin(),
      haploidLeft.end()
   );
   m_chromosomes.insert(
      m_chromosomes.end(),
      haploidRight.begin(),
      haploidRight.end()
   );

   // Build homologous pairs;
   const InstructionEquals instrEquals(m_config);
   auto compare = std::bind(
      _symmetricCompare,
      std::placeholders::_1,
      std::placeholders::_2,
      instrEquals
   );
   m_pairs = std::move(algo::pairing::resolve<float>(
      m_chromosomes,
      compare,
      algo::pairing::MS_MORE_METRIC_MORE_ALIKE,
      0.51
   ));

   initializeGenes();
}


Genome::Genome(const Genome & other)
   : m_config(other.m_config),
   m_chromosomes(other.m_chromosomes),
   m_pairs(other.m_pairs),
   m_genes(other.m_genes)
{
}


Genome::Genome(Genome && other)
   : m_config(other.m_config),
   m_chromosomes(std::move(other.m_chromosomes)),
   m_pairs(std::move(other.m_pairs)),
   m_genes(std::move(other.m_genes))
{
}


Genome::~Genome()
{
}


Genome & Genome::operator=(const Genome & other)
{
   if (this != &other)
   {
      m_config = other.m_config;
      m_chromosomes = other.m_chromosomes;
      m_pairs = other.m_pairs;
      m_genes = other.m_genes;
   }
   return *this;
}


Genome & Genome::operator=(Genome && other)
{
   if (this != &other)
   {
      m_config = other.m_config;
      m_chromosomes = std::move(other.m_chromosomes);
      m_pairs = std::move(other.m_pairs);
      m_genes = std::move(other.m_genes);
   }
   return *this;
}


boost::shared_ptr<const Config> Genome::config() const
{
   return m_config;
}


std::vector<Chromosome> Genome::makeHaploid(
   const MutationParams & mutationParams
) const
{
   std::vector<Chromosome> haploid;
   haploid.reserve(m_pairs.size());
   for(const auto & pair : m_pairs)
   {
      assert(pair.left < m_chromosomes.size());
      const std::vector<Instruction> & codeLeft =
         m_chromosomes[pair.left].code();
      if (pair.right)
      {
         assert(*pair.right < m_chromosomes.size());
         const std::vector<Instruction> & codeRight =
            m_chromosomes[*pair.right].code();
         auto codePair = crossOver(
            codeLeft, codeRight,
            algo::Alignment(codeLeft, codeRight, InstructionEquals(m_config)),
            algo::UniformUInt16RandomGenerator(
               mutationParams.maxDistanceBetweenCrossingPoints
            )
         );
         Chromosome chromosome(codePair.first);
         chromosome.applyMutations(mutationParams);
         haploid.push_back(std::move(chromosome));
      }
      else
      {
         Chromosome chromosome(codeLeft);
         chromosome.applyMutations(mutationParams);
         haploid.push_back(std::move(chromosome));
      }
   }

   const size_t count = haploid.size();
   if ((count > 1) && mutationParams.chromosomeDeletion.random())
   {
      haploid.erase(haploid.begin() + (rand() % count));
   }
   return haploid;
}


void Genome::initializeGenes()
{
   // TODO: Eliminate gene copies from homologous cromosomes;
   m_genes.clear();
   const Config & config = *m_config;
   for (size_t i = 0, icount = m_chromosomes.size(); i < icount; ++i)
   {
      GeneInitializer init;
      const std::vector<Instruction> & code = m_chromosomes[i].code();
      for (size_t j = 0, jcount = code.size(); j < jcount; ++j)
      {
         const Instruction & instr = code[j];
         init.append(instr, config);
         if ((config.opcode(instr.cmd()) == OP_PROM) || (j + 1 == jcount))
         {
            Gene gene(*m_config, init);
            if (gene.hasResponse())
            {
               m_genes.push_back(std::move(gene));
            }
            init = GeneInitializer();
         }
      }
   }
   m_genes.shrink_to_fit();
}


std::ostream & operator<<(std::ostream & os, const Genome & genome)
{
   const std::vector<Chromosome> & chromosomes = genome.chromosomes();
   for (const Chromosome & cr : chromosomes)
   {
      os << "{" << cr << "}" << std::endl;
   }
   return os;
}


}
