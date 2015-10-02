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


#include <cmath>


#include "Vertex.hpp"


namespace mesh {


/***************************************************************************
 *   Vertex structure implementation                                       *
 ***************************************************************************/


Vertex::Vertex()
   : x(0.0f), y(0.0f), z(0.0f), w(1.0f),
   nx(0.0f), ny(0.0f), nz(0.0f), nw(1.0f)
{
}


Vertex::Vertex(
   dt::Float x, dt::Float y, dt::Float z,
   dt::Float nx, dt::Float ny, dt::Float nz
) : x(x), y(y), z(z), w(1.0f),
   nx(nx), ny(ny), nz(nz), nw(1.0f)
{
}


std::ostream & operator<<(std::ostream & os, const Vertex & v)
{
   os << "{x: " << v.x << ", y: " << v.y << ", z: " << v.z <<
      ", nx: " << v.nx << ", ny: " << v.ny << ", nz: " << v.nz << "}";
   return os;
}


/***************************************************************************
 *   DynamicVertex structure implementation                                *
 ***************************************************************************/


DynamicVertex::DynamicVertex()
   : x(0.0f), y(0.0f), z(0.0f), selected(0.0f),
   nx(0.0f), ny(0.0f), nz(0.0f), k(0.0f),
   vx(0.0f), vy(0.0f), vz(0.0f), mass(0.0f)
{
}


DynamicVertex::DynamicVertex(dt::Float x, dt::Float y, dt::Float z)
   : x(x), y(y), z(z), selected(0.0f),
   nx(0.0f), ny(0.0f), nz(0.0f), k(1.0f),
   vx(0.0f), vy(0.0f), vz(0.0f), mass(0.25f)
{
}


Vertex DynamicVertex::vertex() const
{
   return Vertex(x, y, z, nx, ny, nz);
}


dt::Pointf3 DynamicVertex::point() const
{
   return dt::Pointf3(x, y, z);
}


dt::Float DynamicVertex::speed() const
{
   return sqrtf(vx * vx + vy * vy + vz * vz);
}


std::ostream & operator<<(std::ostream & os, const DynamicVertex & dv)
{
   os << "{x: " << dv.x << ", y: " << dv.y << ", z: " << dv.z <<
      ", nx: " << dv.nx << ", ny: " << dv.ny << ", nz: " << dv.nz <<
      ", vx: " << dv.vx << ", vy: " << dv.vy << ", vz: " << dv.vz <<
      ", k: " << dv.k << ", m: " << dv.mass;
   if (dv.selected > 0.0f) os << ", selected";
   os << "}";
   return os;
}


/***************************************************************************
 *   StaticVertex structure implementation                                 *
 ***************************************************************************/


StaticVertex::StaticVertex()
   : connection(-1)
{
}


StaticVertex::StaticVertex(GLint connection)
   : connection(connection)
{
}


std::ostream & operator<<(std::ostream & os, const StaticVertex & sv)
{
   os << "{c: " << sv.connection << "}";
   return os;
}


}
