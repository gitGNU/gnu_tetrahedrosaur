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


#ifndef ALGO_DISTANCE_HPP
#define ALGO_DISTANCE_HPP


#include <algorithm>
#include <functional>
#include <vector>


#include <boost/multi_array.hpp>


namespace algo {


template<typename T, typename Equals = std::equal_to<T> >
size_t getLevenshteinDistance(
   const std::vector<T> & left,
   const std::vector<T> & right,
   const Equals & equals = Equals()
)
{
   const size_t leftCount = left.size() + 1;
   const size_t rightCount = right.size() + 1;
   boost::multi_array<size_t, 2> mt;
   mt.resize(boost::extents[leftCount][rightCount]);

   for (size_t i = 0; i < leftCount; ++i)
   {
      for (size_t j = 0; j < rightCount; ++j)
      {
         mt[i][j] = 0;
      }
   }

   for (size_t i = 1; i < leftCount; ++i)
   {
      mt[i][0] = i;
   }
   for (size_t j = 1; j < rightCount; ++j)
   {
      mt[0][j] = j;
   }

   for (size_t j = 1; j < rightCount; ++j)
   {
      for (size_t i = 1; i < leftCount; ++i)
      {
         if (equals(left[i - 1], right[j - 1]))
         {
            mt[i][j] = mt[i - 1][j - 1];
         }
         else
         {
            mt[i][j] = std::min(
               mt[i - 1][j] + 1,
               std::min(mt[i][j - 1] + 1, mt[i - 1][j - 1] + 1)
            );
         }
      }
   }

   return mt[leftCount - 1][rightCount - 1];
}


template<typename T, typename Equals = std::equal_to<T> >
float getLevenshteinDistanceSimilarity(
   const std::vector<T> & left,
   const std::vector<T> & right,
   const Equals & equals = Equals()
)
{
   const size_t ld = getLevenshteinDistance(left, right, equals);
   return 1.0f - (static_cast<float>(ld) / static_cast<float>(
      std::max(left.size(), right.size())));
}


} // namespace algo;


#endif
