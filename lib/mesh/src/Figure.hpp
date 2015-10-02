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


#ifndef MESH_FIGURE_H
#define MESH_FIGURE_H


#include <cstdlib>
#include <ostream>


#include "datatypes/geometry.hpp"


namespace mesh {


class Mesh;
struct Triangle;
struct Vertex;


/***************************************************************************
 *   Figure class declaration                                              *
 ***************************************************************************/


class Figure
{
   public:
      explicit Figure(const Mesh & mesh);
      virtual ~Figure();

      inline const Vertex * vertices() const {return m_vertices;}
      inline const Triangle * triangles() const {return m_triangles;}

      inline size_t vertexCount() const {return m_vertexCount;}
      inline size_t triangleCount() const {return m_triangleCount;}

      inline dt::Pointf3 center() const {return m_center;}
      inline dt::Vectorf3 dimensions() const {return m_dimensions;}

   private:
      Vertex * m_vertices;
      Triangle * m_triangles;

      size_t m_vertexCount;
      size_t m_triangleCount;

      dt::Pointf3 m_center;
      dt::Vectorf3 m_dimensions;
};


std::ostream & operator<<(std::ostream & os, const Figure & mesh);


}


#endif
