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


#ifndef BIO_CROSSINGOVER_HPP
#define BIO_CROSSINGOVER_HPP


#include <algorithm>
#include <limits>
#include <vector>


#include "algo/Alignment.hpp"
#include "algo/random_generators.hpp"


namespace bio {


/***************************************************************************
 *   Crossing over implementation                                          *
 ***************************************************************************/


template<typename T>
std::pair<std::vector<T>, std::vector<T> > crossOver(
   const std::vector<T> & left,
   const std::vector<T> & right,
   const algo::Alignment & alignment,
   const algo::UniformUInt16RandomGenerator &distanceBetweenCrossingPoints
)
{
   std::vector<T> leftResult;
   std::vector<T> rightResult;
   const size_t leftSize = left.size();
   const size_t rightSize = right.size();
   if (leftSize && rightSize)
   {
      leftResult.reserve(leftSize + rightSize);
      rightResult.reserve(leftSize + rightSize);

      static const size_t max = std::numeric_limits<size_t>::max();
      typename std::vector<T>::const_iterator leftItb = left.begin();
      size_t leftBeg = 0;
      size_t leftEnd = 0;
      typename std::vector<T>::const_iterator rightItb = right.begin();
      size_t rightBeg = 0;
      size_t rightEnd = 0;
      bool nowOnLeft = true;
      while ((leftBeg < leftSize) && (rightBeg < rightSize))
      {
         const size_t distance = distanceBetweenCrossingPoints.generate();
         if (nowOnLeft)
         {
            if ((leftEnd > (max - distance)) || // (leftEnd + distance) > max;
               ((leftEnd + distance) > leftSize))
            {
               leftEnd = leftSize;
            }
            else
            {
               leftEnd += distance;
            }

            rightEnd = ((leftEnd < leftSize) ?
               alignment.rightIndex(leftEnd) : rightSize
            );
         }
         else
         {
            if ((rightEnd > (max - distance)) || // (rightEnd + distance) > max;
               ((rightEnd + distance) > rightSize))
            {
               rightEnd = rightSize;
            }
            else
            {
               rightEnd += distance;
            }

            leftEnd = ((rightEnd < rightSize) ?
               alignment.leftIndex(rightEnd) : leftSize
            );
         }

         std::copy(
            leftItb + leftBeg,
            leftItb + leftEnd,
            std::back_inserter(nowOnLeft ? leftResult : rightResult)
         );
         std::copy(
            rightItb + rightBeg,
            rightItb + rightEnd,
            std::back_inserter(nowOnLeft ? rightResult : leftResult)
         );

         leftBeg = leftEnd;
         rightBeg = rightEnd;

         nowOnLeft = !nowOnLeft;
      }

      leftResult.shrink_to_fit();
      rightResult.shrink_to_fit();
   }
   else
   {
      leftResult = left;
      rightResult = right;
   }

   return std::pair<std::vector<T>, std::vector<T> >(
      std::move(leftResult),
      std::move(rightResult)
   );
}


}


#endif
