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


#ifndef MESH_TRIANGLESMAP_H
#define MESH_TRIANGLESMAP_H


#include <cstdlib>
#include <map>
#include <ostream>
#include <GL/gl.h>


#include <boost/optional.hpp>


#include "datatypes/mesh.hpp"


#include "Triangle.hpp"


namespace mesh {


/***************************************************************************
 *   TrianglesMap class declaration                                        *
 ***************************************************************************/


class TrianglesMap
{
   public:
      typedef std::map<Triangle, dt::TriangleId>::const_iterator const_iterator;

      explicit TrianglesMap();

      TrianglesMap(TrianglesMap && other);

      TrianglesMap & operator=(TrianglesMap && other);

      inline const_iterator begin() const
      {
         return m_map.begin();
      }

      inline const_iterator end() const
      {
         return m_map.end();
      }

      inline bool empty() const
      {
         return m_map.empty();
      }

      inline size_t size() const
      {
         return m_map.size();
      }

      inline const_iterator find(const Triangle & triangle) const
      {
         return m_map.find(triangle);
      }

      boost::optional<dt::TriangleId> findAny(const Triangle & triangle) const;

      inline void insert(const Triangle & t, const dt::TriangleId & id)
      {
         m_map.insert(std::pair<Triangle, dt::TriangleId>(t, id));
      }

      void replace(const Triangle & t, const dt::TriangleId & newId);
      bool remove(const Triangle & triangle);

      TrianglesMap replaceVertexIndex(GLint oldIndex, GLint newIndex);

   private:
      std::map<Triangle, dt::TriangleId> m_map;
};


std::ostream & operator<<(std::ostream & os, const TrianglesMap & trianglesMap);


}


#endif
