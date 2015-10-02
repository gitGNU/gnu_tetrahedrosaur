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


#ifndef BIO_GENOME_H
#define BIO_GENOME_H


#include <ostream>
#include <vector>


#include <boost/shared_ptr.hpp>


#include "Chromosome.hpp"
#include "Gene.hpp"


namespace algo {
namespace pairing {
template<typename Metric> struct Pair;
}
}


namespace bio {


struct Config;


/***************************************************************************
 *   Genome class declaration                                              *
 ***************************************************************************/


class Genome
{
   public:
      explicit Genome(boost::shared_ptr<const Config> config);
      explicit Genome(
         boost::shared_ptr<const Config> config,
         const std::vector<Chromosome> & diploid
      );
      explicit Genome(
         boost::shared_ptr<const Config> config,
         const std::vector<Chromosome> & haploidLeft,
         const std::vector<Chromosome> & haploidRight
      );

      Genome(const Genome & other);
      Genome(Genome && other);
      virtual ~Genome();

      Genome & operator=(const Genome & other);
      Genome & operator=(Genome && other);

      boost::shared_ptr<const Config> config() const;
      inline const std::vector<Chromosome> & chromosomes() const;
      inline const std::vector<algo::pairing::Pair<float> > & pairs() const;
      inline const std::vector<Gene> & genes() const;

      std::vector<Chromosome> makeHaploid() const;

   private:
      void initializeGenes();

      boost::shared_ptr<const Config> m_config;
      std::vector<Chromosome> m_chromosomes;
      std::vector<algo::pairing::Pair<float> > m_pairs;
      std::vector<Gene> m_genes;
};


inline const std::vector<Chromosome> & Genome::chromosomes() const
{
   return m_chromosomes;
}


inline const std::vector<algo::pairing::Pair<float> > & Genome::pairs() const
{
   return m_pairs;
}


inline const std::vector<Gene> & Genome::genes() const
{
   return m_genes;
}


std::ostream & operator<<(std::ostream & os, const Genome & genome);


}


#endif
