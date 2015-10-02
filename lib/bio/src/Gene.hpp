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


#ifndef BIO_GENE_HPP
#define BIO_GENE_HPP


#include <ostream>
#include <vector>


#include <boost/optional.hpp>


#include "datatypes/geometry.hpp"
#include "datatypes/numeric.hpp"


#include "mesh/BuddingParams.hpp"


#include "RangeCondition.hpp"


namespace bio {


class Instruction;
struct Config;
struct GeneInitializer;


/***************************************************************************
 *   Gene structure declaration                                            *
 ***************************************************************************/


struct Gene
{
   explicit Gene(const Config & config, const GeneInitializer & init);
   virtual ~Gene();

   bool hasResponse() const;

   // Conditions;
   RangeCondition<dt::UInt16> generationCondition;
   RangeCondition<dt::Int16> xCondition;
   RangeCondition<dt::Int16> yCondition;

   // Responses;
   std::vector<Instruction> responses;
};


std::ostream & operator<<(std::ostream & os, const Gene & gene);


}


#endif
