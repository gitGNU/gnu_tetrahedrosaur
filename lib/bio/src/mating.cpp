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


#include <cassert>


#include "Genome.hpp"
#include "mating.hpp"
#include "OrganismDesc.hpp"


#include "algo/random_generators.hpp"


namespace {


size_t _generate(
   const std::vector<float> & probabilities,
   const algo::UniformNormalizedFloatRandomGenerator & generator
)
{
   const float f = generator.generate();
   for (size_t i = 0, count = probabilities.size(); i < count; ++i)
   {
      if (f < probabilities[i])
      {
         return probabilities.size() - i;
      }
   }
   return 0;
}


void _init(
   bio::OrganismDesc & desc,
   boost::shared_ptr<const bio::Config> config,
   const bio::MutationParams & mutationParams,
   const bio::OrganismDesc & left,
   const bio::OrganismDesc & right
)
{
   desc.initialConditions = bio::InitialConditions(
      left.initialConditions,
      right.initialConditions,
      mutationParams
   );
   desc.genome.reset(new bio::Genome(
      config,
      left.genome->makeHaploid(mutationParams),
      right.genome->makeHaploid(mutationParams)
   ));
}


void _init(
   bio::OrganismDesc & desc,
   boost::shared_ptr<const bio::Config> config,
   const bio::MutationParams & mutationParams,
   const bio::OrganismDesc & left
)
{
   desc.initialConditions = left.initialConditions;
   desc.initialConditions.applyMutations(mutationParams);
   desc.genome.reset(new bio::Genome(
      config,
      left.genome->makeHaploid(mutationParams),
      left.genome->makeHaploid(mutationParams) // FIXME:;
   ));
}


} // anonymous namespace;


namespace bio {


/***************************************************************************
 *   Mating implementation                                                 *
 ***************************************************************************/


std::vector<std::pair<size_t, size_t> > mate(
   size_t singleCount,
   size_t pairCount,
   const algo::UniformNormalizedFloatRandomGenerator & generator
)
{
   std::vector<std::pair<size_t, size_t> > pairs;
   if ((singleCount >= 2) && pairCount)
   {
      float p = 0.5f;
      std::vector<float> probabilities(singleCount - 1, 0.0f);
      for (int i = probabilities.size() - 1; i >= 0; --i)
      {
         probabilities[i] = p;
         p *= 0.5f;
      }

      pairs.reserve(pairCount);
      for (size_t i = 0; i < pairCount; ++i)
      {
         const size_t first = _generate(probabilities, generator);
         std::pair<size_t, size_t> pair(first, (first ? 0 : 1));
         for (size_t j = 0; j < 1024; ++j)
         {
            const size_t second = _generate(probabilities, generator);
            if (second != pair.first)
            {
               pair.second = second;
               break;
            }
         }
         pairs.push_back(pair);
      }
   }
   return pairs;
}


std::vector<boost::shared_ptr<OrganismDesc> > mate(
   const std::vector<boost::shared_ptr<const OrganismDesc> > & prevGeneration,
   OrganismDesc *(* createDesc)(),
   size_t nextGenerationSize,
   boost::shared_ptr<const Config> config,
   const MutationParams & mutationParams
)
{
   assert(createDesc);
   const size_t prevGenerationSize = prevGeneration.size();
   std::vector<boost::shared_ptr<OrganismDesc> > nextGeneration;
   if (nextGenerationSize)
   {
      nextGeneration.reserve(nextGenerationSize);
      if (prevGenerationSize > 1)
      {
         const std::vector<std::pair<size_t, size_t> > pairs = std::move(
            mate(
               prevGenerationSize,
               nextGenerationSize,
               algo::UniformNormalizedFloatRandomGenerator()
            )
         );
         assert(pairs.size() == nextGenerationSize);

         for (const std::pair<size_t, size_t> & pair : pairs)
         {
            assert(pair.first < prevGenerationSize);
            assert(pair.second < prevGenerationSize);
            assert(pair.first != pair.second);
            OrganismDesc * desc = createDesc();
            assert(desc);
            _init(
               *desc,
               config,
               mutationParams,
               *prevGeneration[pair.first],
               *prevGeneration[pair.second]
            );
            nextGeneration.push_back(boost::shared_ptr<OrganismDesc>(desc));
         }
      }
      else if (prevGenerationSize == 1)
      {
         for (size_t i = 0; i < nextGenerationSize; ++i)
         {
            OrganismDesc * desc = createDesc();
            assert(desc);
            _init(*desc, config, mutationParams, *prevGeneration.front());
            nextGeneration.push_back(boost::shared_ptr<OrganismDesc>(desc));
         }
      }
   }
   return nextGeneration;
}


}
