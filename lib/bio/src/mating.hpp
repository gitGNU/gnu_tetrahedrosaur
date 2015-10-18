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


#ifndef BIO_MATING_HPP
#define BIO_MATING_HPP


#include <algorithm>
#include <vector>


#include <boost/shared_ptr.hpp>


namespace algo {
class UniformNormalizedFloatRandomGenerator;
}


namespace bio {


struct Config;
struct MutationParams;
struct OrganismDesc;


/***************************************************************************
 *   Mating declaration                                                    *
 ***************************************************************************/


std::vector<std::pair<size_t, size_t> > mate(
   size_t singleCount,
   size_t pairCount,
   const algo::UniformNormalizedFloatRandomGenerator & generator
);


std::vector<boost::shared_ptr<OrganismDesc> > mate(
   const std::vector<boost::shared_ptr<const OrganismDesc> > & prevGeneration,
   OrganismDesc *(* createDesc)(),
   size_t nextGenerationSize,
   boost::shared_ptr<const Config> config,
   const MutationParams & mutationParams
);


}


#endif
