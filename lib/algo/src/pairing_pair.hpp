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


#ifndef ALGO_PAIRING_PAIR_HPP
#define ALGO_PAIRING_PAIR_HPP


#include <ostream>


#include <boost/optional.hpp>


namespace algo {


namespace pairing {


/***************************************************************************
 *   Pair structure declaration and implementation                         *
 ***************************************************************************/


template <typename Metric>
struct Pair
{
   explicit Pair(size_t left);
   explicit Pair(size_t left, size_t right, Metric metric);

   size_t left;
   boost::optional<size_t> right;
   boost::optional<Metric> metric;
};


template <typename Metric>
Pair<Metric>::Pair(size_t left)
   : left(left)
{
}


template <typename Metric>
Pair<Metric>::Pair(size_t left, size_t right, Metric metric)
   : left(left), right(right), metric(metric)
{
}


template <typename Metric>
bool operator<(const Pair<Metric> & lhs, const Pair<Metric> & rhs)
{
   const bool linit = lhs.right.is_initialized();
   const bool rinit = rhs.right.is_initialized();
   if (linit == rinit)
   {
      if (linit)
      {
         return (lhs.left < rhs.left && *lhs.right < *rhs.right);
      }
      return lhs.left < rhs.left;
   }
   return (linit && !rinit);
}


template <typename Metric>
std::ostream & operator<<(std::ostream & os, const Pair<Metric> & pair)
{
   os << "{" << pair.left;
   if (pair.right)
   {
      os << ", " << *pair.right;
      if (pair.metric)
      {
         os << "; " << *pair.metric;
      }
   }
   os << "}";
   return os;
}


} // namespace pairing;


} // namespace algo;


#endif
