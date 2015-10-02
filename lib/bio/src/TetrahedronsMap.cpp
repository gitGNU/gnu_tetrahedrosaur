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


#include "Cell.hpp"
#include "TetrahedronsMap.hpp"


namespace bio {


/***************************************************************************
 *   TetrahedronsMap class implementation                                  *
 ***************************************************************************/


TetrahedronsMap::~TetrahedronsMap()
{
   for (iterator it = m_map.begin(), ite = m_map.end(); it != ite; ++it)
   {
      delete it->second;
   }
}


Cell * TetrahedronsMap::findAny(const mesh::Tetrahedron & t) const
{
   for (int i = mesh::Tetrahedron::C_FIRST; i <= mesh::Tetrahedron::C_LAST; ++i)
   {
      const_iterator it = m_map.find(t.combination(
         static_cast<mesh::Tetrahedron::COMBINATION>(i)
      ));
      if (it != m_map.end())
      {
         return it->second;
      }
   }

   return 0;
}


void TetrahedronsMap::remove(const mesh::Tetrahedron & t)
{
   iterator it = m_map.find(t);
   if (it != m_map.end())
   {
      delete it->second;
      m_map.erase(it);
   }
}


void TetrahedronsMap::replaceVertexIndex(size_t oldIndex, size_t newIndex)
{
   std::map<mesh::Tetrahedron, Cell *> tmp;

   iterator it = m_map.begin();
   while (it != m_map.end())
   {
      assert(!it->first.contains(newIndex));
      if (it->first.contains(oldIndex))
      {
         const mesh::Tetrahedron ttr = it->first.replaced(oldIndex, newIndex);
         tmp.insert(std::pair<mesh::Tetrahedron, Cell *>(ttr, it->second));
         it->second->setTetrahedron(ttr);
         it = m_map.erase(it);
      }
      else
      {
         ++it;
      }
   }

   it = tmp.begin();
   for (iterator ite = tmp.end(); it != ite; ++it)
   {
      m_map.insert(*it);
   }
}


std::ostream & operator<<(std::ostream & os, const TetrahedronsMap & tm)
{
   os << "{";
   TetrahedronsMap::const_iterator it = tm.begin();
   TetrahedronsMap::const_iterator ite = tm.end();
   for (int i = 0; it != ite; ++ it, ++ i)
   {
      if (i) os << ", ";
      os << it->first << ":" << *it->second;
   }
   os << "}";
   return os;
}


}
