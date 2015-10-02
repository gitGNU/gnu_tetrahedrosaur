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


#ifndef MESH_STRUCTUREMODIFICATION_H
#define MESH_STRUCTUREMODIFICATION_H


#include <cstdlib>
#include <list>
#include <ostream>


namespace mesh {


/***************************************************************************
 *   StructureModification class declaration                               *
 ***************************************************************************/


class StructureModification
{
   public:
      enum Type
      {
         T_VERTEX_MOVE = 0
      };

      struct Entry
      {
         explicit Entry(Type type, size_t oldIndex = 0, size_t newIndex = 0);
         Type type;
         size_t oldIndex;
         size_t newIndex;
      };

      typedef std::list<Entry>::const_iterator const_iterator;

      inline const_iterator begin() const;
      inline const_iterator end() const;
      inline bool empty() const;
      inline void clear();

      void insertVertexMove(size_t oldIndex, size_t newIndex);

      inline bool operator==(const StructureModification &other) const;
      inline bool operator!=(const StructureModification &other) const;

   private:
      std::list<Entry> m_list;
};


bool operator==(
   const StructureModification::Entry &x,
   const StructureModification::Entry &y
);


inline StructureModification::const_iterator StructureModification::begin(
) const
{
   return m_list.begin();
}


inline StructureModification::const_iterator StructureModification::end() const
{
   return m_list.end();
}


inline bool StructureModification::empty() const
{
   return m_list.empty();
}


inline void StructureModification::clear()
{
   m_list.clear();
}


inline bool StructureModification::operator==(
   const StructureModification &other
) const
{
   return (m_list == other.m_list);
}


inline bool StructureModification::operator!=(
   const StructureModification &other
) const
{
   return !(m_list == other.m_list);
}


std::ostream & operator<<(
   std::ostream & os,
   const StructureModification & structMod
);


}


#endif
