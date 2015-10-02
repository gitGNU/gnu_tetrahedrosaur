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


#include "MemoryModification.hpp"


namespace mesh {


/***************************************************************************
 *   MemoryModification class implementation                               *
 ***************************************************************************/


void MemoryModification::insertMemoryBlock(size_t offset, size_t size)
{
   if (size) {
      m_set.insert(boost::icl::interval<size_t>::right_open(
         offset,
         offset + size
      ));
   }
}


void MemoryModification::insertArrayElement(size_t index, size_t size)
{
   insertMemoryBlock(index * size, size);
}


std::ostream & operator<<(std::ostream & os, const MemoryModification & memMod)
{
   os << "{";
   MemoryModification::const_iterator it = memMod.begin();
   MemoryModification::const_iterator ite = memMod.end();
   for (int i = 0; it != ite; ++ it, ++ i)
   {
      if (i) os << ", ";
      os << "[" << it->lower() << ", " << it->upper() << ")";
   }
   os << "}";
   return os;
}


}
