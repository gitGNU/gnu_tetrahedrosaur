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


#ifndef MESH_MEMORYMODIFICATION_H
#define MESH_MEMORYMODIFICATION_H


#include <cstdlib>
#include <ostream>


#include <boost/icl/interval_set.hpp>


namespace mesh {


/***************************************************************************
 *   MemoryModification class declaration                                  *
 ***************************************************************************/


class MemoryModification
{
   public:
      typedef boost::icl::interval_set<size_t>::const_iterator const_iterator;

      inline const_iterator begin() const;
      inline const_iterator end() const;
      inline bool empty() const;
      inline void clear();

      void insertMemoryBlock(size_t offset, size_t size);
      void insertArrayElement(size_t index, size_t size);

      inline bool operator==(const MemoryModification &other) const;
      inline bool operator!=(const MemoryModification &other) const;

   private:
      boost::icl::interval_set<size_t> m_set;
};


inline MemoryModification::const_iterator MemoryModification::begin() const
{
   return m_set.begin();
}


inline MemoryModification::const_iterator MemoryModification::end() const
{
   return m_set.end();
}


inline bool MemoryModification::empty() const
{
   return m_set.empty();
}


inline void MemoryModification::clear()
{
   m_set.clear();
}


inline bool MemoryModification::operator==(
   const MemoryModification &other
) const
{
   return (m_set == other.m_set);
}


inline bool MemoryModification::operator!=(
   const MemoryModification &other
) const
{
   return (m_set != other.m_set);
}


std::ostream & operator<<(std::ostream & os, const MemoryModification & memMod);


}


#endif
