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


#include "geometry.hpp"


namespace dt {


/***************************************************************************
 *   Pointf3 structure implementation                                      *
 ***************************************************************************/


Pointf3::Pointf3(Float x, Float y, Float z)
   : x(x), y(y), z(z)
{
}


Pointf3 & Pointf3::operator+=(const Vectorf3 & vector)
{
   x += vector.x;
   y += vector.y;
   z += vector.z;
   return *this;
}


const Pointf3 Pointf3::operator+(const Vectorf3 & vector) const
{
   return (Pointf3(*this) += vector);
}


bool Pointf3::operator==(const Pointf3 &other) const
{
   return (x == other.x && y == other.y && z == other.z);
}


/***************************************************************************
 *   Vectorf3 structure implementation                                     *
 ***************************************************************************/


Vectorf3::Vectorf3(Float x, Float y, Float z)
   : x(x), y(y), z(z)
{
}


Vectorf3::Vectorf3(const Pointf3 & begin, const Pointf3 & end)
   : x(end.x - begin.x), y(end.y - begin.y), z(end.z - begin.z)
{
}


Float Vectorf3::length() const
{
   return sqrt((x * x) + (y * y) + (z * z));
}


Vectorf3 Vectorf3::normalized() const
{
   dt::Float n = length();
   if (n > 0.0f)
   {
      return Vectorf3(x / n, y / n, z / n);
   }
   return Vectorf3(0.0f, 0.0f, 0.0f);
}


Vectorf3 & Vectorf3::operator*=(Float value)
{
   x *= value;
   y *= value;
   z *= value;
   return *this;
}


const Vectorf3 Vectorf3::operator*(Float value) const
{
   return (Vectorf3(*this) *= value);
}


bool Vectorf3::operator==(const Vectorf3 &other) const
{
   return (x == other.x && y == other.y && z == other.z);
}


Vectorf3 crossProduct(const Vectorf3 &v1, const Vectorf3 &v2)
{
   return Vectorf3(
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x
   );
}


Float dotProduct(const Vectorf3 &v1, const Vectorf3 &v2)
{
   return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}


/***************************************************************************
 *   LineSegmentf3 structure implementation                                *
 ***************************************************************************/


LineSegmentf3::LineSegmentf3(const Pointf3 & a, const Pointf3 & b)
   : a(a), b(b)
{
}


LineSegmentf3::LineSegmentf3(const Pointf3 & p, const Vectorf3 & vector)
   : a(p), b(p + vector)
{
}


Float LineSegmentf3::length() const
{
   const Float x = b.x - a.x;
   const Float y = b.y - a.y;
   const Float z = b.z - a.z;
   return sqrt((x * x) + (y * y) + (z * z));
}


}
