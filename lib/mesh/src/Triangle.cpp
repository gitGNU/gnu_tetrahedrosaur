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


#include <cassert>


#include "Triangle.hpp"


namespace mesh {


/***************************************************************************
 *   Triangle structure implementation                                     *
 ***************************************************************************/


Triangle::Triangle()
   : a(0), b(0), c(0)
{
}


Triangle::Triangle(GLuint a, GLuint b, GLuint c)
   : a(a), b(b), c(c)
{
   assert(a != b && a != c && b != c);
}


Triangle Triangle::replaced(GLuint x, GLuint y) const
{
   if (a == x) return Triangle(y, b, c);
   else if (b == x) return Triangle(a, y, c);
   else if (c == x) return Triangle(a, b, y);

   return *this;
}


Triangle Triangle::combination(COMBINATION combination) const
{
   switch (combination) {
      case C_ACB:
         return Triangle(a, c, b);
      case C_BAC:
         return Triangle(b, a, c);
      case C_BCA:
         return Triangle(b, c, a);
      case C_CAB:
         return Triangle(c, a, b);
      case C_CBA:
         return Triangle(c, b, a);
      default:;
   }
   return *this;
}


bool Triangle::contains(GLuint x) const
{
   return (a == x || b == x || c == x);
}


boost::optional<GLuint> Triangle::third(GLuint x, GLuint y) const
{
   if ((a == x && b == y) || (b == x && a == y)) return c;
   else if ((a == x && c == y) || (c == x && a == y)) return b;
   else if ((b == x && c == y) || (c == x && b == y)) return a;

   return boost::optional<GLuint>();
}


boost::optional<Triangle::VERTEX_ORDER> Triangle::vertexOrder(
   GLuint x,
   GLuint y
) const
{
   if (x != y)
   {
      if ((a == x && b == y) || (b == x && c == y) || (c == x && a == y))
      {
         return VO_DIRECT;
      }
      else if ((a == y && b == x) || (b == y && c == x) || (c == y && a == x))
      {
         return VO_INVERSE;
      }
   }
   return boost::optional<VERTEX_ORDER>();
}


bool Triangle::operator==(const Triangle & other) const
{
   return (a == other.a && b == other.b && c == other.c);
}


bool Triangle::operator<(const Triangle & other) const
{
   if (a == other.a)
   {
      if (b == other.b)
      {
         return (c < other.c);
      }
      return (b < other.b);
   }
   return (a < other.a);
}


std::ostream & operator<<(std::ostream & os, const Triangle & t)
{
   os << "{" << t.a << ", " << t.b << ", " << t.c << "}";
   return os;
}


}
