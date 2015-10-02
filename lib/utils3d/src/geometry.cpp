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


namespace utils3d {


/***************************************************************************
 *   Geometry utils implementation                                         *
 ***************************************************************************/


dt::Float distance(const dt::Pointf3 & p1, const dt::Pointf3 & p2)
{
   return dt::Vectorf3(p1, p2).length();
}


dt::Vectorf3 getNormalToPlane(
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3
)
{
   return dt::crossProduct(
      dt::Vectorf3(p1, p2),
      dt::Vectorf3(p1, p3)
   ).normalized();
}


dt::Pointf3 getTetrahedronTopPoint(
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3,
   dt::Float budTopRadius,
   dt::Float budTopPolarAngle,
   dt::Float budTopAzimuthalAngle
)
{
   const dt::Pointf3 pSph(
      budTopRadius * sin(budTopPolarAngle) * cos(budTopAzimuthalAngle),
      budTopRadius * sin(budTopPolarAngle) * sin(budTopAzimuthalAngle),
      budTopRadius * cos(budTopPolarAngle)
   );

   const dt::Vectorf3 nx = dt::Vectorf3(p1, p2).normalized();
   const dt::Vectorf3 nz = getNormalToPlane(p1, p2, p3);
   const dt::Vectorf3 ny = dt::crossProduct(nx, nz).normalized();

   const dt::Pointf3 p(
      nx.x * pSph.x + ny.x * pSph.y + nz.x * pSph.z,
      nx.y * pSph.x + ny.y * pSph.y + nz.y * pSph.z,
      nx.z * pSph.x + ny.z * pSph.y + nz.z * pSph.z
   );

   const dt::Pointf3 p0(
      (p1.x + p2.x + p3.x) / 3.0f,
      (p1.y + p2.y + p3.y) / 3.0f,
      (p1.z + p2.z + p3.z) / 3.0f
   );
   return dt::Pointf3(p.x + p0.x, p.y + p0.y, p.z + p0.z);
}


boost::optional<dt::Pointf3> getLinePlaneIntersection(
   const dt::LineSegmentf3 &ls,
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3
)
{
   const dt::Vectorf3 a(p1, p2);
   const dt::Vectorf3 b(p1, p3);
   const dt::Vectorf3 x(ls.a, p1);
   const dt::Vectorf3 y(ls.a, ls.b);

   // Calculate normal to the plane as a cross product of [a, b];
   const dt::Vectorf3 n = dt::crossProduct(a, b).normalized();

   // Calculate dot product of (n, x);
   dt::Float v = dt::dotProduct(n, x);

   // Calculate dot product of (n, y);
   dt::Float w = dt::dotProduct(n, y);

   if (w != 0.0f)
   { // Single intersection point;
      return dt::Pointf3(ls.a + (y * (v / w)));
   }
   else if (v != 0.0f)
   { // The line is parallel to the plane;
      return boost::none;
   }

   // The line is contained in the plane;
   return boost::none;
}


boost::optional<dt::Pointf3> getLineSegmentPlaneIntersection(
   const dt::LineSegmentf3 &ls,
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3
)
{
   if (const auto p = getLinePlaneIntersection(ls, p1, p2, p3))
   {
      const dt::Float length = ls.length();
      if (distance(*p, ls.a) <= length && distance(*p, ls.b) <= length)
      {
         return p;
      }
   }
   return boost::none;
}


boost::optional<dt::Pointf3> getLineSegmentTriangleIntersection(
   const dt::LineSegmentf3 &ls,
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c
)
{
   #define _VEC_P3P2(p1, p1p2, p3) \
      p1.x + (p1p2.x) - p3.x, \
      p1.y + (p1p2.y) - p3.y, \
      p1.z + (p1p2.z) - p3.z

   if (const auto point = getLineSegmentPlaneIntersection(ls, a, b, c))
   {
      const dt::Pointf3 & p = *point;
      const dt::Vectorf3 ab(a, b);
      const dt::Vectorf3 bc(b, c);
      const dt::Vectorf3 ac(a, c);
      const dt::Vectorf3 ad = dt::crossProduct(ab, ac).normalized();

      // Check the position of P relative to AC edge;
      const dt::Vectorf3 ae = dt::crossProduct(ac, ad).normalized();
      const dt::Vectorf3 pe(_VEC_P3P2(a, ae, p));
      const dt::Vectorf3 pen(_VEC_P3P2(a, -ae, p));
      if (pe.length() > pen.length()) {
         return boost::none;
      }

      // Check the position of P relative to AB edge;
      const dt::Vectorf3 af = dt::crossProduct(ad, ab).normalized();
      const dt::Vectorf3 pf(_VEC_P3P2(a, af, p));
      const dt::Vectorf3 pfn(_VEC_P3P2(a, -af, p));
      if (pf.length() > pfn.length()) {
         return boost::none;
      }

      // Check the position of P relative to BC edge;
      const dt::Vectorf3 bg = dt::crossProduct(ad, bc).normalized();
      const dt::Vectorf3 pg(_VEC_P3P2(b, bg, p));
      const dt::Vectorf3 pgn(_VEC_P3P2(b, -bg, p));
      if (pg.length() > pgn.length()) {
         return boost::none;
      }

      return p;
   }
   return boost::none;

   #undef _VEC_P3P2
}


bool doTrianglesIntersect(
   const dt::Pointf3 & x,
   const dt::Pointf3 & y,
   const dt::Pointf3 & z,
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c
)
{
   return
      getLineSegmentTriangleIntersection(dt::LineSegmentf3(x, y), a, b, c) ||
      getLineSegmentTriangleIntersection(dt::LineSegmentf3(x, z), a, b, c) ||
      getLineSegmentTriangleIntersection(dt::LineSegmentf3(y, z), a, b, c) ||
      getLineSegmentTriangleIntersection(dt::LineSegmentf3(a, b), x, y, z) ||
      getLineSegmentTriangleIntersection(dt::LineSegmentf3(a, c), x, y, z) ||
      getLineSegmentTriangleIntersection(dt::LineSegmentf3(b, c), x, y, z);
}


bool doesPointBelongToRightTriangularPrismWithInfinitHeight(
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c,
   const dt::Pointf3 & p,
   bool * above
)
{
   #define _VEC_P3P2(p1, p1p2, p3) \
      p1.x + (p1p2.x) - p3.x, \
      p1.y + (p1p2.y) - p3.y, \
      p1.z + (p1p2.z) - p3.z

   if (above)
   {
      *above = false;
   }

   const dt::Vectorf3 ab(a, b);
   const dt::Vectorf3 bc(b, c);
   const dt::Vectorf3 ac(a, c);

   // Check the position of P relative to ABC plane;
   const dt::Vectorf3 ad = dt::crossProduct(ab, ac).normalized();
   const dt::Vectorf3 pd(_VEC_P3P2(a, ad, p));
   const dt::Vectorf3 pdn(_VEC_P3P2(a, -ad, p));
   if (pd.length() > pdn.length()) {
      return false;
   }

   if (above)
   {
      *above = true;
   }

   // Check the position of P relative to AC edge;
   const dt::Vectorf3 ae = dt::crossProduct(ac, ad).normalized();
   const dt::Vectorf3 pe(_VEC_P3P2(a, ae, p));
   const dt::Vectorf3 pen(_VEC_P3P2(a, -ae, p));
   if (pe.length() > pen.length()) {
      return false;
   }

   // Check the position of P relative to AB edge;
   const dt::Vectorf3 af = dt::crossProduct(ad, ab).normalized();
   const dt::Vectorf3 pf(_VEC_P3P2(a, af, p));
   const dt::Vectorf3 pfn(_VEC_P3P2(a, -af, p));
   if (pf.length() > pfn.length()) {
      return false;
   }

   // Check the position of P relative to BC edge;
   const dt::Vectorf3 bg = dt::crossProduct(ad, bc).normalized();
   const dt::Vectorf3 pg(_VEC_P3P2(b, bg, p));
   const dt::Vectorf3 pgn(_VEC_P3P2(b, -bg, p));
   if (pg.length() > pgn.length()) {
      return false;
   }

   return true;

   #undef _VEC_P3P2
}


bool doesPointAbovePlane(
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c,
   const dt::Pointf3 & p
)
{
   #define _VEC_P3P2(p1, p1p2, p3) \
      p1.x + (p1p2.x) - p3.x, \
      p1.y + (p1p2.y) - p3.y, \
      p1.z + (p1p2.z) - p3.z

   const dt::Vectorf3 ab(a, b);
   const dt::Vectorf3 ac(a, c);

   // Check the position of P relative to ABC plane;
   const dt::Vectorf3 ad = dt::crossProduct(ab, ac).normalized();
   const dt::Vectorf3 pd(_VEC_P3P2(a, ad, p));
   const dt::Vectorf3 pdn(_VEC_P3P2(a, -ad, p));
   if (pd.length() > pdn.length()) {
      return false;
   }

   return true;

   #undef _VEC_P3P2
}


dt::Float getTetrahedronVolume(
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3,
   const dt::Pointf3 & p4
)
{
   //           |1 x1 y1 z1|
   //           |1 x2 y2 z2|
   // V = 1/6 * |1 x3 y3 z3|
   //           |1 x4 y4 z4|

   return fabs(
      ((p2.x - p1.x) * (p3.y * p4.z - p4.y * p3.z)) -
      ((p2.y - p1.y) * (p3.x * p4.z - p4.x * p3.z)) +
      ((p2.z - p1.z) * (p3.x * p4.y - p4.x * p3.y)) +
      ((p4.x - p3.x) * (p1.y * p2.z - p2.y * p1.z)) -
      ((p4.y - p3.y) * (p1.x * p2.z - p2.x * p1.z)) +
      ((p4.z - p3.z) * (p1.x * p2.y - p2.x * p1.y))
   ) / 6.0f;
}


}
