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
#include <cmath>


#include <boost/optional.hpp>
#include <boost/variant.hpp>


#include "Alignment.hpp"


namespace {


struct _Pair
{
   explicit _Pair(size_t key, size_t value);

   size_t key;
   size_t value;
};


_Pair::_Pair(size_t key, size_t value)
   : key(key), value(value)
{}


struct _PrevNext
{
   boost::optional<_Pair> prevPair;
   boost::optional<_Pair> nextPair;
};


size_t _valueByNormalizedKeyAndKeyRange(
   size_t key,
   size_t keyMax,
   size_t valueMax
)
{
   if (!valueMax)
   {
      return 0;
   }
   else if (!keyMax)
   {
      return (valueMax / 2);
   }
   else if (keyMax == valueMax)
   {
      return key;
   }

   // Resolve proportion;
   double value = (static_cast<double>(key) * static_cast<double>(valueMax)) /
      static_cast<double>(keyMax);
   return static_cast<size_t>(round(value));
}


size_t _valueByKeyAndKeyRange(
   const boost::optional<_Pair> &prev,
   const boost::optional<_Pair> &next,
   size_t key,
   size_t keySize,
   size_t valueSize
)
{
   assert(keySize > 0);
   assert(valueSize > 0);
   assert(key < keySize);

   if (prev && next)
   {
      //             0 10
      //             1 11 = prev->value
      // prev->key = 2 12
      //             3 13
      //       key = 4 14
      //             5 15
      // next->key = 6 16
      //             7 17 = next->value
      //             8 18
      //
      // k = key - prev->key - 1 = 4 - 2 - 1 = 1;
      // kmax = next->key - prev->key - 2 = 6 - 2 - 2 = 2;
      // vmax = next->value - prev->value - 2 = 17 - 11 - 2 = 4;
      //
      //             0
      //           0 1
      //       k = 1 2
      //    kmax = 2 3
      //             4 = vmax
      assert(key > prev->key && key < next->key);
      assert(prev->value < next->value);

      const size_t k = key - prev->key - 1;
      const size_t kmax = next->key - prev->key - 2;
      assert(k >= 0 && k <= kmax);

      if (next->value > (prev->value + 1))
      {
         const size_t vmax = next->value - prev->value - 2;
         return _valueByNormalizedKeyAndKeyRange(k, kmax, vmax) + prev->value + 1;
      }

      return ((k <= (kmax / 2)) ? prev->value : next->value);
   }
   else if (prev)
   {
      //             0 10
      //             1 11 = prev->value
      // prev->key = 2 12
      //             3 13
      //       key = 4 14
      //             5 15
      //               16
      //
      // k = key - prev->key - 1 = 4 - 2 - 1 = 1;
      // kmax = keySize - prev->key - 2 = 6 - 2 - 2 = 2;
      // vmax = valueSize - prev->value - 2 = 17 - 11 - 2 = 4;
      //
      //             0
      //           0 1
      //       k = 1 2
      //    kmax = 2 3
      //             4 = vmax
      assert(key > prev->key && key < keySize);
      assert(prev->value < valueSize);

      if (valueSize > (prev->value + 1))
      {
         const size_t k = key - prev->key - 1;
         const size_t kmax = keySize - prev->key - 2;
         assert(k >= 0 && k <= kmax);

         const size_t vmax = valueSize - prev->value - 2;
         return _valueByNormalizedKeyAndKeyRange(k, kmax, vmax) + prev->value + 1;
      }

      return prev->value;
   }
   else if (next)
   {
      //               0
      //             0 1
      //       key = 1 2
      //             2 3
      // next->key = 3 4
      //             4 5 = next->value
      //             5 6
      //
      // k = key = 1;
      // kmax = next->key - 1 = 3 - 1 = 2;
      // vmax = next->value - 1 = 5 - 1 = 4;
      //
      //             0
      //           0 1
      //       k = 1 2
      //    kmax = 2 3
      //             4 = vmax
      assert(key < next->key);

      if (next->value > 0)
      {
         const size_t k = key;
         const size_t kmax = next->key - 1;
         assert(k >= 0 && k <= kmax);

         const size_t vmax = next->value - 1;
         return _valueByNormalizedKeyAndKeyRange(k, kmax, vmax);
      }

      return next->value;
   }

   //             0
   //           0 1
   //     key = 1 2
   //           2 3
   //             4
   //
   // k = key = 1;
   // kmax = keySize - 1 = 3 - 1 = 2;
   // vmax = valueSize - 1 = 5 - 1 = 4;
   //
   //             0
   //           0 1
   //       k = 1 2
   //    kmax = 2 3
   //             4 = vmax
   return _valueByNormalizedKeyAndKeyRange(key, keySize - 1, valueSize - 1);
}


template<typename Map>
boost::variant<size_t, _PrevNext> _lookupMap(const Map &map, size_t key)
{
   typedef typename Map::const_iterator const_iterator;
   const std::pair<const_iterator, const_iterator> er = map.equal_range(key);
   const const_iterator begin = map.begin();
   const const_iterator end = map.end();
   const_iterator lower = er.first;
   const const_iterator upper = er.second;

   _PrevNext pn;

   if (lower != end)
   {
      while (lower->first > key && lower != begin)
      {
         --lower;
      }

      if (lower->first == key)
      {
         return boost::variant<size_t, _PrevNext>(lower->second);
      }
      else if (lower->first < key)
      {
         pn.prevPair = _Pair(lower->first, lower->second);
      }
   }
   else if (!map.empty())
   {
      --lower;
      pn.prevPair = _Pair(lower->first, lower->second);
   }

   if (upper != end)
   {
      pn.nextPair = _Pair(upper->first, upper->second);
   }

   return boost::variant<size_t, _PrevNext>(pn);
}


}


namespace algo {


/***************************************************************************
 *   LcsMatrix class implementation                                        *
 ***************************************************************************/


size_t LcsMatrix::lcsLength() const
{
   return m_data[m_data.shape()[0] - 1][m_data.shape()[1] - 1];
}


size_t LcsMatrix::at(size_t i, size_t j) const
{
   return m_data[i][j];
}


/***************************************************************************
 *   Alignment::const_iterator class implementation                        *
 ***************************************************************************/


Alignment::const_iterator::const_iterator()
   : m_alignment(0)
{
}


Alignment::const_iterator & Alignment::const_iterator::operator++()
{
   if (m_alignment)
   {
      if (m_nextIt == m_alignment->bimapEnd())
      {
         if (m_left)
         {
            const size_t next = *m_left + 1;
            m_left = ((next < m_alignment->leftSize()) ?
               boost::optional<size_t>(next) : boost::none
            );
         }
         if (m_right)
         {
            const size_t next = *m_right + 1;
            m_right = ((next < m_alignment->rightSize()) ?
               boost::optional<size_t>(next) : boost::none
            );
         }
      }
      else
      {
         if (m_left)
         {
            const size_t next = *m_left + 1;
            m_left = ((next < m_nextIt->left) ?
               boost::optional<size_t>(next) : boost::none
            );
         }
         if (m_right)
         {
            const size_t next = *m_right + 1;
            m_right = ((next < m_nextIt->right) ?
               boost::optional<size_t>(next) : boost::none
            );
         }
         if (!m_left && !m_right)
         {
            m_left = m_nextIt->left;
            m_right = m_nextIt->right;
            ++m_nextIt;
         }
      }
      assert(!m_left || *m_left < m_alignment->leftSize());
      assert(!m_right || *m_right < m_alignment->rightSize());
   }
   return *this;
}


bool Alignment::const_iterator::operator==(
   const const_iterator & other
) const
{
   return (m_alignment == other.m_alignment &&
      m_nextIt == other.m_nextIt &&
      m_left == other.m_left &&
      m_right == other.m_right);
}


bool Alignment::const_iterator::operator!=(
   const const_iterator & other
) const
{
   return (!operator==(other));
}


Alignment::const_iterator::const_iterator(
   const Alignment * alignment,
   bool begin
) : m_alignment(alignment)
{
   if (m_alignment)
   {
      if (begin)
      {
         m_nextIt = m_alignment->bimapBegin();
         if (m_nextIt != m_alignment->bimapEnd())
         {
            if (!m_nextIt->left && !m_nextIt->right)
            {
               operator++();
            }
            else
            {
               if (m_nextIt->left && m_alignment->leftSize())
               {
                  m_left = 0;
               }
               if (m_nextIt->right && m_alignment->rightSize())
               {
                  m_right = 0;
               }
            }
         }
      }
      else
      {
         m_nextIt = m_alignment->bimapEnd();
      }
      assert(!m_left || *m_left < m_alignment->leftSize());
      assert(!m_right || *m_right < m_alignment->rightSize());
   }
}


/***************************************************************************
 *   Alignment class implementation                                        *
 ***************************************************************************/


size_t Alignment::rightIndex(size_t leftIndex) const
{
   boost::variant<size_t, _PrevNext> v = _lookupMap(m_bimap.left, leftIndex);
   if (_PrevNext * prevNext = boost::get<_PrevNext>(&v))
   {
      return _valueByKeyAndKeyRange(
         prevNext->prevPair,
         prevNext->nextPair,
         leftIndex,
         m_leftSize,
         m_rightSize
      );
   }

   return *boost::get<size_t>(&v);
}


size_t Alignment::leftIndex(size_t rightIndex) const
{
   boost::variant<size_t, _PrevNext> v = _lookupMap(m_bimap.right, rightIndex);
   if (_PrevNext * prevNext = boost::get<_PrevNext>(&v))
   {
      return _valueByKeyAndKeyRange(
         prevNext->prevPair,
         prevNext->nextPair,
         rightIndex,
         m_rightSize,
         m_leftSize
      );
   }

   return *boost::get<size_t>(&v);
}


size_t Alignment::index(size_t i, bool rightByLeft) const
{
   return (rightByLeft ? rightIndex(i) : leftIndex(i));
}


std::ostream & operator<<(std::ostream & os, const Alignment & al)
{
   os << "{";
   Alignment::const_bimap_iterator it = al.bimapBegin();
   Alignment::const_bimap_iterator ite = al.bimapEnd();
   for (bool isFirst = true; it != ite; ++it)
   {
      if (isFirst)
      {
         isFirst = false;
         os << ", ";
      }
      os << "[" << it->left << ", " << it->right << "]";
   }
   os << "}";
   return os;
}


}
