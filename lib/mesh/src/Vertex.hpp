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


#ifndef VERTEX_H
#define VERTEX_H


#include <ostream>
#include <GL/gl.h>


#include "datatypes/geometry.hpp"


namespace mesh {


/***************************************************************************
 *   Vertex structure declaration                                          *
 ***************************************************************************/


#pragma pack(push)
#pragma pack(1)


struct Vertex
{
   explicit Vertex();
   explicit Vertex(
      dt::Float x, dt::Float y, dt::Float z,
      dt::Float nx, dt::Float ny, dt::Float nz
   );

   dt::Float x, y, z, w;
   dt::Float nx, ny, nz, nw;
};


#pragma pack(pop)


std::ostream & operator<<(std::ostream & os, const Vertex & v);


/***************************************************************************
 *   DynamicVertex structure declaration                                   *
 ***************************************************************************/


#pragma pack(push)
#pragma pack(1)


struct DynamicVertex
{
   explicit DynamicVertex();
   explicit DynamicVertex(dt::Float x, dt::Float y, dt::Float z);

   Vertex vertex() const;
   dt::Pointf3 point() const;
   dt::Float speed() const;

   dt::Float x, y, z, selected;
   dt::Float nx, ny, nz, k;
   dt::Float vx, vy, vz, mass;
};


#pragma pack(pop)


std::ostream & operator<<(std::ostream & os, const DynamicVertex & dv);


/***************************************************************************
 *   StaticVertex structure declaration                                    *
 ***************************************************************************/


#pragma pack(push)
#pragma pack(1)


struct StaticVertex
{
   explicit StaticVertex();
   explicit StaticVertex(GLint connection);
   GLint connection;
};


#pragma pack(pop)


std::ostream & operator<<(std::ostream & os, const StaticVertex & sv);


}


#endif
