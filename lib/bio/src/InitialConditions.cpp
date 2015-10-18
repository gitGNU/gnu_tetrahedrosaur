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


#include <cstdlib>
#include <limits>


#include "InitialConditions.hpp"
#include "MutationParams.hpp"


namespace {


int16_t _mean(int16_t a, int16_t b)
{
   int32_t m = a;
   m += b;
   m /= 2;
   return static_cast<int16_t>(m);
}


} // anonymous namespace;


namespace bio {


InitialConditions::InitialConditions()
   : cellLimit(1), x(0), y(0)
{
}


InitialConditions::InitialConditions(
   const InitialConditions & left,
   const InitialConditions & right,
   const MutationParams & mutationParams
) : cellLimit(0),
   x(_mean(left.x, right.x)),
   y(_mean(left.y, right.y))
{
   const uint32_t m = (left.cellLimit + right.cellLimit) / 2;
   cellLimit = static_cast<uint16_t>(m);

   applyMutations(mutationParams);
}


void InitialConditions::applyMutations(const MutationParams & params)
{
   // Modify cellLimit;
   if (params.cellLimitMutation.random())
   {
      uint16_t delta = 1 + (rand() % params.maxCellLimitDelta);
      if (rand() % 2)
      {
         if ((std::numeric_limits<uint16_t>::max() - cellLimit) > delta)
         {
            cellLimit += delta;
         }
         else
         {
            cellLimit = std::numeric_limits<uint16_t>::max();
         }
      }
      else
      {
         if (cellLimit > delta)
         {
            cellLimit -= delta;
         }
         else
         {
            cellLimit = 1;
         }
      }
   }

   // Modify x and y;
   if (params.initialCoordMutation.random())
   {
      x += (1 - (rand() % 3));
      y += (1 - (rand() % 3));
   }
}


std::ostream & operator<<(std::ostream & os, const InitialConditions & ic)
{
   os << "{cell limit: " << ic.cellLimit << ", x: " << ic.x << ", y: " << ic.y
      << "}";
   return os;
}


}
