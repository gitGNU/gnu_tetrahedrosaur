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


#ifndef BIO_MUTATIONPARAMS_HPP
#define BIO_MUTATIONPARAMS_HPP


#include <cstdint>


#include "algo/Probability.hpp"


namespace bio {


/***************************************************************************
 *   MutationParams struct declaration                                     *
 ***************************************************************************/


struct MutationParams
{
   explicit MutationParams();

   static MutationParams low();
   static MutationParams medium();
   static MutationParams high();

   uint16_t maxCellLimitDelta;
   algo::Probability cellLimitMutation;
   algo::Probability initialCoordMutation;
   uint16_t maxCodeSize;
   uint16_t maxDistanceBetweenCrossingPoints;
   algo::Probability chromosomeDeletion;
   algo::Probability instructionInsertion;
   algo::Probability instructionDeletion;
   algo::Probability instructionMutation;
   uint8_t maxInstructionBitFlips;
};


}


#endif
