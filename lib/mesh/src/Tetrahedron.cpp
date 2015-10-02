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


#include <boost/tuple/tuple.hpp>


#include "Tetrahedron.hpp"


namespace mesh {


/***************************************************************************
 *   Tetrahedron structure implementation                                  *
 ***************************************************************************/


Tetrahedron::Tetrahedron(size_t a, size_t b, size_t c, size_t d)
   : a(a), b(b), c(c), d(d)
{
   assert(a != b && a != c && a != d && b != c && b != d && c != d);
}


Tetrahedron Tetrahedron::replaced(size_t x, size_t y) const
{
   if (a == x) return Tetrahedron(y, b, c, d);
   else if (b == x) return Tetrahedron(a, y, c, d);
   else if (c == x) return Tetrahedron(a, b, y, d);
   else if (d == x) return Tetrahedron(a, b, c, y);

   return *this;
}


Tetrahedron Tetrahedron::combination(COMBINATION combination) const
{
   switch (combination) {
      case C_ABCD:
         return Tetrahedron(a, b, c, d);
      case C_ABDC:
         return Tetrahedron(a, b, d, c);
      case C_ACBD:
         return Tetrahedron(a, c, b, d);
      case C_ACDB:
         return Tetrahedron(a, c, d, b);
      case C_ADBC:
         return Tetrahedron(a, d, b, c);
      case C_ADCB:
         return Tetrahedron(a, d, c, b);
      case C_BACD:
         return Tetrahedron(b, a, c, d);
      case C_BADC:
         return Tetrahedron(b, a, d, c);
      case C_BCAD:
         return Tetrahedron(b, c, a, d);
      case C_BCDA:
         return Tetrahedron(b, c, d, a);
      case C_BDAC:
         return Tetrahedron(b, d, a, c);
      case C_BDCA:
         return Tetrahedron(b, d, c, a);
      case C_CABD:
         return Tetrahedron(c, a, b, d);
      case C_CADB:
         return Tetrahedron(c, a, d, b);
      case C_CBAD:
         return Tetrahedron(c, b, a, d);
      case C_CBDA:
         return Tetrahedron(c, b, d, a);
      case C_CDAB:
         return Tetrahedron(c, d, a, b);
      case C_CDBA:
         return Tetrahedron(c, d, b, a);
      case C_DABC:
         return Tetrahedron(d, a, b, c);
      case C_DACB:
         return Tetrahedron(d, a, c, b);
      case C_DBAC:
         return Tetrahedron(d, b, a, c);
      case C_DBCA:
         return Tetrahedron(d, b, c, a);
      case C_DCAB:
         return Tetrahedron(d, c, a, b);
      case C_DCBA:
         return Tetrahedron(d, c, b, a);
      default:;
   }
   return *this;
}


boost::optional<dt::TetrahedronFace> Tetrahedron::faceAny(
   size_t v1,
   size_t v2,
   size_t v3
) const
{
   const boost::tuple<size_t, size_t, size_t, dt::TetrahedronFace> f[] = {
      {a, b, c, dt::TF_ABC},
      {a, c, d, dt::TF_ACD},
      {a, d, b, dt::TF_ADB},
      {b, c, d, dt::TF_BCD}
   };
   for (size_t i = 0; i < 4; ++i) {
      if ((v1 == f[i].get<0>() && v2 == f[i].get<1>() && v3 == f[i].get<2>()) ||
         (v1 == f[i].get<0>() && v2 == f[i].get<2>() && v3 == f[i].get<1>()) ||
         (v1 == f[i].get<1>() && v2 == f[i].get<0>() && v3 == f[i].get<2>()) ||
         (v1 == f[i].get<1>() && v2 == f[i].get<2>() && v3 == f[i].get<0>()) ||
         (v1 == f[i].get<2>() && v2 == f[i].get<0>() && v3 == f[i].get<1>()) ||
         (v1 == f[i].get<2>() && v2 == f[i].get<1>() && v3 == f[i].get<0>())
      )
      {
         return f[i].get<3>();
      }
   }
   return boost::none;
}


boost::optional<dt::TetrahedronFace> Tetrahedron::borderFace(
   const Tetrahedron & other
) const
{
   const boost::tuple<size_t, size_t, size_t> v[] = {
      {other.a, other.b, other.c},
      {other.a, other.c, other.d},
      {other.a, other.d, other.b},
      {other.b, other.c, other.d}
   };

   for (size_t i = 0; i < 4; ++i) {
      auto face = faceAny(v[i].get<0>(), v[i].get<1>(), v[i].get<2>());
      if (face)
      {
         return face;
      }
   }
   return boost::none;
}


boost::optional<dt::TetrahedronFace> Tetrahedron::relativeFace(
   const Tetrahedron & other,
   dt::TetrahedronFace otherFace
) const
{
   // Make new tetrahedron for which otherFace will be dt::TF_ABC;
   Tetrahedron ttr = other;
   switch (otherFace)
   {
      case dt::TF_ABC:
         break;
      case dt::TF_ACD:
         ttr = Tetrahedron(other.a, other.c, other.d, other.b);
         break;
      case dt::TF_ADB:
         ttr = Tetrahedron(other.a, other.d, other.b, other.c);
         break;
      case dt::TF_BCD:
         ttr = Tetrahedron(other.b, other.c, other.d, other.a);
         break;
   }

   if ((a == ttr.a || a == ttr.b || a == ttr.c) &&
      (b == ttr.a || b == ttr.b || b == ttr.c) &&
      (c == ttr.a || c == ttr.b || c == ttr.c))
   {
      return dt::TF_ABC;
   }
   else if ((a == ttr.a || a == ttr.b || a == ttr.c) &&
      (c == ttr.a || c == ttr.b || c == ttr.c) &&
      (d == ttr.a || d == ttr.b || d == ttr.c))
   {
      return dt::TF_ACD;
   }
   else if ((a == ttr.a || a == ttr.b || a == ttr.c) &&
      (d == ttr.a || d == ttr.b || d == ttr.c) &&
      (b == ttr.a || b == ttr.b || b == ttr.c))
   {
      return dt::TF_ADB;
   }
   else if ((b == ttr.a || b == ttr.b || b == ttr.c) &&
      (c == ttr.a || c == ttr.b || c == ttr.c) &&
      (d == ttr.a || d == ttr.b || d == ttr.c))
   {
      return dt::TF_BCD;
   }

   return boost::none;
}


bool Tetrahedron::contains(size_t x) const
{
   return (a == x || b == x || c == x || d == x);
}


bool Tetrahedron::faceContains(
   dt::TetrahedronFace f,
   size_t x,
   size_t y,
   size_t z
) const
{
   #define _EQUALS(_a, _b, _c) ((_a == x && _b == y && _c == z) || \
      (_a == x && _b == z && _c == y) || \
      (_a == y && _b == x && _c == z) || \
      (_a == y && _b == z && _c == x) || \
      (_a == z && _b == x && _c == y) || \
      (_a == z && _b == y && _c == x) \
   )

   switch (f) {
      case dt::TF_ABC:
         return _EQUALS(a, b, c);
      case dt::TF_ACD:
         return _EQUALS(a, c, d);
      case dt::TF_ADB:
         return _EQUALS(a, d, b);
      case dt::TF_BCD:
         return _EQUALS(b, c, d);
   }
   return false;

   #undef _EQUALS
}


bool Tetrahedron::operator==(const Tetrahedron & other) const
{
   return (a == other.a && b == other.b && c == other.c && d == other.d);
}


bool Tetrahedron::operator<(const Tetrahedron & other) const
{
   if (a == other.a)
   {
      if (b == other.b)
      {
         if (c == other.c)
         {
            return (d < other.d);
         }
         return (c < other.c);
      }
      return (b < other.b);
   }
   return (a < other.a);
}


std::ostream & operator<<(std::ostream & os, const Tetrahedron & t)
{
   os << "{" << t.a << ", " << t.b << ", " << t.c << ", " << t.d << "}";
   return os;
}


}
