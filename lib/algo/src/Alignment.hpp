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


#ifndef ALGO_ALIGNMENT_HPP
#define ALGO_ALIGNMENT_HPP


#include <cassert>
#include <functional>
#include <vector>


#include <boost/bimap.hpp>
#include <boost/multi_array.hpp>
#include <boost/optional.hpp>


namespace algo {


/***************************************************************************
 *   LcsMatrix class declaration                                           *
 ***************************************************************************/


class LcsMatrix
{
   public:
      template<typename T, typename Equals = std::equal_to<T> >
      explicit LcsMatrix(
         const std::vector<T> & left,
         const std::vector<T> & right,
         const Equals & equals = Equals()
      );

      size_t lcsLength() const;
      size_t at(size_t i, size_t j) const;

   private:
      boost::multi_array<size_t, 2> m_data;
};


template<typename T, typename Equals>
LcsMatrix::LcsMatrix(
   const std::vector<T> & left,
   const std::vector<T> & right,
   const Equals & equals
)
{
   const size_t leftCount = left.size() + 1;
   const size_t rightCount = right.size() + 1;
   m_data.resize(boost::extents[leftCount][rightCount]);

   for (size_t i = 0; i < leftCount; ++i)
   {
      m_data[i][0] = 0;
   }
   for (size_t j = 0; j < rightCount; ++j)
   {
      m_data[0][j] = 0;
   }

   for (size_t i = 1; i < leftCount; ++i)
   {
      for (size_t j = 1; j < rightCount; ++j)
      {
         if (equals(left[i - 1], right[j - 1]))
         {
            m_data[i][j] = m_data[i - 1][j - 1] + 1;
         }
         else
         {
            m_data[i][j] = std::max(m_data[i][j - 1], m_data[i - 1][j]);
         }
      }
   }
}


/***************************************************************************
 *   Alignment class declaration                                           *
 ***************************************************************************/


class Alignment
{
   public:
      typedef boost::bimap<size_t, size_t>::const_iterator
         const_bimap_iterator;

      class const_iterator
      {
         friend class Alignment;

         public:
            const_iterator();

            inline boost::optional<size_t> left() const {return m_left;}
            inline boost::optional<size_t> right() const {return m_right;}

            const_iterator & operator++();
            bool operator==(const const_iterator & other) const;
            bool operator!=(const const_iterator & other) const;

         private:
            const_iterator(const Alignment * alignment, bool begin);

            const Alignment * m_alignment;
            const_bimap_iterator m_nextIt;
            boost::optional<size_t> m_left;
            boost::optional<size_t> m_right;
      };

      template<typename T, typename Equals = std::equal_to<T> >
      explicit Alignment(
         const std::vector<T> & left,
         const std::vector<T> & right,
         const Equals & equals = Equals()
      );

      inline size_t leftSize() const {return m_leftSize;}
      inline size_t rightSize() const {return m_rightSize;}

      size_t rightIndex(size_t leftIndex) const;
      size_t leftIndex(size_t rightIndex) const;
      size_t index(size_t i, bool rightByLeft) const;

      inline const_bimap_iterator bimapBegin() const;
      inline const_bimap_iterator bimapEnd() const;

      inline const_iterator begin() const;
      inline const_iterator end() const;

   private:
      boost::bimap<size_t, size_t> m_bimap;
      size_t m_leftSize;
      size_t m_rightSize;
};


inline Alignment::const_bimap_iterator Alignment::bimapBegin() const
{
   return m_bimap.begin();
}


inline Alignment::const_bimap_iterator Alignment::bimapEnd() const
{
   return m_bimap.end();
}


inline Alignment::const_iterator Alignment::begin() const
{
   return const_iterator(this, true);
}


inline Alignment::const_iterator Alignment::end() const
{
   return const_iterator(this, false);
}


std::ostream & operator<<(std::ostream & os, const Alignment & al);


template<typename T, typename Equals>
Alignment::Alignment(
   const std::vector<T> & left,
   const std::vector<T> & right,
   const Equals & equals
) : m_leftSize(left.size()), m_rightSize(right.size())
{
   LcsMatrix matrix(left, right, equals);
   //assert(matrix.shape()[0] == m_leftSize + 1);
   //assert(matrix.shape()[1] == m_rightSize + 1);

   size_t i = m_leftSize;
   size_t j = m_rightSize;
   while (i > 0 && j > 0)
   {
      if (equals(left[i - 1], right[j - 1]))
      {
         m_bimap.insert(m_bimap.begin(),
            boost::bimap<size_t, size_t>::value_type(i - 1, j - 1));
         --i;
         --j;
      }
      else if (matrix.at(i - 1, j) > matrix.at(i, j - 1))
      {
         --i;
      }
      else
      {
         --j;
      }
   }
}


}


#endif
