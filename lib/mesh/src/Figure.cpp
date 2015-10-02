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


#include <cstring>
#include <iostream>


#include "Figure.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "Vertex.hpp"


namespace mesh {


/***************************************************************************
 *   Figure class implementation                                           *
 ***************************************************************************/


Figure::Figure(const Mesh & mesh)
   : m_vertices(0),
   m_triangles(0),
   m_vertexCount(mesh.vertexCount()),
   m_triangleCount(mesh.triangleCount()),
   m_center(mesh.center()),
   m_dimensions(mesh.dimensions())
{
   const DynamicVertex * dynamicVertices = mesh.dynamicVertices();

   m_vertices = new Vertex[m_vertexCount];
   for (size_t i = 0; i < m_vertexCount; ++i)
   {
      m_vertices[i] = dynamicVertices[i].vertex();
   }

   m_triangles = new Triangle[m_triangleCount];
   memcpy(m_triangles, mesh.triangles(), sizeof(Triangle) * m_triangleCount);
}


Figure::~Figure()
{
   delete[] m_vertices;
   delete[] m_triangles;
}


std::ostream & operator<<(std::ostream & os, const Figure & figure)
{
   bool isFirst = true;
   os << "{";
   if (figure.vertexCount()) {
      const Vertex * vertices = figure.vertices();

      os << "DV: [0]" << vertices[0];
      for (size_t i = 1; i < figure.vertexCount(); ++ i) {
         os << ", [" << i << "]" << vertices[i];
      }
      isFirst = false;
   }
   if (figure.triangleCount()) {
      const Triangle * triangles = figure.triangles();

      if (!isFirst) os << " ";
      os << "TR: [0]" << triangles[0];
      for (size_t i = 1; i < figure.triangleCount(); ++ i) {
         os << ", [" << i << "]" << triangles[i];
      }
      isFirst = false;
   }
   return os;
}


}
