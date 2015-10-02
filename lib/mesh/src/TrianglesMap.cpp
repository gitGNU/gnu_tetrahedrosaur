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


#include "TrianglesMap.hpp"


namespace mesh {


/***************************************************************************
 *   TrianglesMap class implementation                                     *
 ***************************************************************************/


TrianglesMap::TrianglesMap()
{
}


TrianglesMap::TrianglesMap(TrianglesMap && other)
   : m_map(std::move(other.m_map))
{
}


TrianglesMap & TrianglesMap::operator=(TrianglesMap && other)
{
   if (this != &other)
   {
      m_map = std::move(other.m_map);
   }
   return *this;
}


boost::optional<dt::TriangleId> TrianglesMap::findAny(
   const Triangle & triangle
) const
{
   for (int i = Triangle::C_FIRST; i <= Triangle::C_LAST; ++ i)
   {
      const_iterator it = m_map.find(triangle.combination(
         static_cast<Triangle::COMBINATION>(i)
      ));
      if (it != m_map.end())
      {
         return it->second;
      }
   }

   return boost::none;
}


void TrianglesMap::replace(const Triangle & t, const dt::TriangleId & newId)
{
   std::map<Triangle, dt::TriangleId>::iterator it = m_map.find(t);
   if (it != m_map.end())
   {
      it->second = newId;
   }
}


bool TrianglesMap::remove(const Triangle & triangle)
{
   std::map<Triangle, dt::TriangleId>::iterator it = m_map.find(triangle);
   if (it != m_map.end())
   {
      m_map.erase(it);
      return true;
   }
   return false;
}


TrianglesMap TrianglesMap::replaceVertexIndex(GLint oldIndex, GLint newIndex)
{
   TrianglesMap replacedTriangles;
   std::map<Triangle, dt::TriangleId>::iterator it = m_map.begin();
   while (it != m_map.end())
   {
      if (it->first.contains(oldIndex))
      {
         replacedTriangles.m_map.insert(std::pair<Triangle, dt::TriangleId>(
            it->first.replaced(oldIndex, newIndex),
            it->second
         ));
         m_map.erase(it ++);
      }
      else
      {
         ++ it;
      }
   }

   if (!replacedTriangles.empty())
   {
      m_map.insert(replacedTriangles.begin(), replacedTriangles.end());
   }

   return replacedTriangles;
}


std::ostream & operator<<(std::ostream & os, const TrianglesMap & trianglesMap)
{
   os << "{";
   TrianglesMap::const_iterator it = trianglesMap.begin();
   TrianglesMap::const_iterator ite = trianglesMap.end();
   for (int i = 0; it != ite; ++ it, ++ i)
   {
      if (i) os << ", ";
      os << it->first << ":" << it->second.get();
   }
   os << "}";
   return os;
}


}
