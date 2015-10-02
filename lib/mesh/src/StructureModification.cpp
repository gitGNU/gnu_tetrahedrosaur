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


#include "StructureModification.hpp"


namespace mesh {


/***************************************************************************
 *   StructureModification class implementation                            *
 ***************************************************************************/


StructureModification::Entry::Entry(
   Type type,
   size_t oldIndex,
   size_t newIndex
) : type(type), oldIndex(oldIndex), newIndex(newIndex)
{
}


void StructureModification::insertVertexMove(size_t oldIndex, size_t newIndex)
{
   m_list.push_back(Entry(T_VERTEX_MOVE, oldIndex, newIndex));
}


bool operator==(
   const StructureModification::Entry &x,
   const StructureModification::Entry &y
)
{
   return (x.type == y.type && x.newIndex == y.newIndex);
}


std::ostream & operator<<(
   std::ostream & os,
   const StructureModification & structMod
)
{
   os << "{";
   StructureModification::const_iterator it = structMod.begin();
   StructureModification::const_iterator ite = structMod.end();
   for (int i = 0; it != ite; ++ it, ++ i)
   {
      if (i) os << ", ";
      os << "[" << it->type << ", " << it->oldIndex << ", " <<
         it->newIndex << "]";
   }
   os << "}";
   return os;
}


} // mesh;
