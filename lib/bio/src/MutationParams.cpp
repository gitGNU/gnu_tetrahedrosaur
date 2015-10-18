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


#include "MutationParams.hpp"


namespace bio {


/***************************************************************************
 *   MutationParams struct implementation                                  *
 ***************************************************************************/


MutationParams::MutationParams()
   : maxCellLimitDelta(0),
   cellLimitMutation(0.0f),
   initialCoordMutation(0.0f),
   maxCodeSize(1),
   maxDistanceBetweenCrossingPoints(1024),
   chromosomeDeletion(0.0f),
   instructionInsertion(0.0f),
   instructionDeletion(0.0f),
   instructionMutation(0.0f),
   maxInstructionBitFlips(0)
{
}


MutationParams MutationParams::low()
{
   MutationParams mp;
   mp.maxCellLimitDelta = 1;
   mp.cellLimitMutation = algo::Probability(0.01f);
   mp.initialCoordMutation = algo::Probability(0.01f);
   mp.maxCodeSize = 128;
   mp.maxDistanceBetweenCrossingPoints = 64;
   mp.chromosomeDeletion = algo::Probability(0.01f);
   mp.instructionInsertion = algo::Probability(0.01f);
   mp.instructionDeletion = algo::Probability(0.01f);
   mp.instructionMutation = algo::Probability(0.01f);
   mp.maxInstructionBitFlips = 1;
   return mp;
}


MutationParams MutationParams::medium()
{
   MutationParams mp;
   mp.maxCellLimitDelta = 16;
   mp.cellLimitMutation = algo::Probability(0.05f);
   mp.initialCoordMutation = algo::Probability(0.05f);
   mp.maxCodeSize = 256;
   mp.maxDistanceBetweenCrossingPoints = 32;
   mp.chromosomeDeletion = algo::Probability(0.05f);
   mp.instructionInsertion = algo::Probability(0.05f);
   mp.instructionDeletion = algo::Probability(0.05f);
   mp.instructionMutation = algo::Probability(0.05f);
   mp.maxInstructionBitFlips = 4;
   return mp;
}


MutationParams MutationParams::high()
{
   MutationParams mp;
   mp.maxCellLimitDelta = 32;
   mp.cellLimitMutation = algo::Probability(0.25f);
   mp.initialCoordMutation = algo::Probability(0.25f);
   mp.maxCodeSize = 128;
   mp.maxDistanceBetweenCrossingPoints = 16;
   mp.chromosomeDeletion = algo::Probability(0.25f);
   mp.instructionInsertion = algo::Probability(0.25f);
   mp.instructionDeletion = algo::Probability(0.25f);
   mp.instructionMutation = algo::Probability(0.25f);
   mp.maxInstructionBitFlips = 8;
   return mp;
}


}
