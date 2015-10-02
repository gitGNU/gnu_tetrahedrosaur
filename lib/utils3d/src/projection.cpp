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
#include <cmath>
#include <GL/glu.h>


#include "geometry.hpp"
#include "projection.hpp"


namespace utils3d {


/***************************************************************************
 *   Projection utils implementation                                       *
 ***************************************************************************/


void initPerspectiveProjMatrix(
   dt::Float m[16],
   dt::Float fovy,
   int width,
   int height,
   dt::Float zNear,
   dt::Float zFar
)
{
   assert(m);
   dt::Float f = 1.0f / tan(fovy * M_PI / 360.0f);
   dt::Float dz = zNear - zFar;

   m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = 0.0f;
   m[8] = m[9] = m[12] = m[13] = m[15] = 0.0f;

   m[0] = f * ((dt::Float) height) / ((dt::Float) width);
   m[5] = f;
   m[10] = (zFar + zNear) / dz;
   m[11] = -1.0f;
   m[14] = 2.0f * zFar * zNear / dz;
}


dt::Matrixf perspectiveProjectionMatrix(
   dt::Float fovy,
   int width,
   int height,
   dt::Float zNear,
   dt::Float zFar
)
{
   const dt::Float f = 1.0f / tan(fovy * M_PI / 360.0f);
   const dt::Float dz = zNear - zFar;

   dt::Matrixf m(4, 4);

   // Column 0;
   m(0, 0) = f * ((dt::Float) height) / ((dt::Float) width);
   m(1, 0) = 0.0f;
   m(2, 0) = 0.0f;
   m(3, 0) = 0.0f;

   // Column 1;
   m(0, 1) = 0.0f;
   m(1, 1) = f;
   m(2, 1) = 0.0f;
   m(3, 1) = 0.0f;

   // Column 2;
   m(0, 2) = 0.0f;
   m(1, 2) = 0.0f;
   m(2, 2) = (zFar + zNear) / dz;
   m(3, 2) = -1.0f;

   // Column 3;
   m(0, 3) = 0.0f;
   m(1, 3) = 0.0f;
   m(2, 3) = 2.0f * zFar * zNear / dz;
   m(3, 3) = 0.0f;

   return m;
}


dt::Matrixf figureThumbnailMatrix(
   dt::Float fovy,
   int width,
   int height,
   dt::Float zNear,
   dt::Float zFar,
   const dt::Pointf3 &center,
   const dt::Vectorf3 &dimensions
)
{
   /*                                             |
                           _______________________|
            /             |           avZ         |
           /          |   | avY                   |
   camera /\__________|___|_______________________|
          \/        zNear                       zFar
      fovy \
            \
   */
   assert(zNear <= zFar);
   assert(dimensions.x >= 0.0f && dimensions.y >= 0.0f && dimensions.z >= 0.0f);

   const dt::Float zNearMargin = zNear + (0.01f * (zFar - zNear));
   const dt::Float avZ = zFar - zNearMargin;
   const dt::Float avY = 2.0f * zNearMargin * tan(fovy * M_PI / 360.0f);
   const dt::Float avX = avY * ((dt::Float) width) / ((dt::Float) height);

   const dt::Float scaleX = (dimensions.x ? (avX / dimensions.x) : 1.0f);
   const dt::Float scaleY = (dimensions.y ? (avY / dimensions.y) : 1.0f);
   const dt::Float scaleZ = (dimensions.z ? (avZ / dimensions.z) : 1.0f);
   const dt::Float scale = std::min(scaleX, std::min(scaleY, scaleZ));

   const dt::Matrixf m = utils3d::translationMatrix(
      -(center.x * scale),
      -(center.y * scale),
      -(center.z * scale) - zNearMargin - (dimensions.z * 0.5f * scale)
   );
   return boost::numeric::ublas::prod(m, scaleMatrix(scale, scale, scale));
}


dt::Matrixf identityMatrix()
{
   return translationMatrix(0.0f, 0.0f, 0.0f);
}


dt::Matrixf translationMatrix(dt::Float x, dt::Float y, dt::Float z)
{
   dt::Matrixf m(4, 4);

   // Column 0;
   m(0, 0) = 1.0f;
   m(1, 0) = 0.0f;
   m(2, 0) = 0.0f;
   m(3, 0) = 0.0f;

   // Column 1;
   m(0, 1) = 0.0f;
   m(1, 1) = 1.0f;
   m(2, 1) = 0.0f;
   m(3, 1) = 0.0f;

   // Column 2;
   m(0, 2) = 0.0f;
   m(1, 2) = 0.0f;
   m(2, 2) = 1.0f;
   m(3, 2) = 0.0f;

   // Column 3;
   m(0, 3) = x;
   m(1, 3) = y;
   m(2, 3) = z;
   m(3, 3) = 1.0f;

   return m;
}


dt::Matrixf scaleMatrix(dt::Float x, dt::Float y, dt::Float z)
{
   dt::Matrixf m(4, 4);

   // Column 0;
   m(0, 0) = x;
   m(1, 0) = 0.0f;
   m(2, 0) = 0.0f;
   m(3, 0) = 0.0f;

   // Column 1;
   m(0, 1) = 0.0f;
   m(1, 1) = y;
   m(2, 1) = 0.0f;
   m(3, 1) = 0.0f;

   // Column 2;
   m(0, 2) = 0.0f;
   m(1, 2) = 0.0f;
   m(2, 2) = z;
   m(3, 2) = 0.0f;

   // Column 3;
   m(0, 3) = 0.0f;
   m(1, 3) = 0.0f;
   m(2, 3) = 0.0f;
   m(3, 3) = 1.0f;

   return m;
}


dt::Matrixf edgeToolMatrix(
   dt::Float x1,
   dt::Float y1,
   dt::Float z1,
   dt::Float x2,
   dt::Float y2,
   dt::Float z2
)
{
   dt::Float yx = x2 - x1;
   dt::Float yy = y2 - y1;
   dt::Float yz = z2 - z1;
   dt::Float yl = sqrt(yx * yx + yy * yy + yz * yz);

   dt::Float xx = 1.0f;
   dt::Float xy = 0.0f;
   dt::Float xz = 0.0f;
   dt::Float tmp = sqrt(yx * yx + yy * yy);
   if (tmp)
   {
      xx = yy / tmp;
      xy = -yx / tmp;
      xz = 0.0f;
   }
   dt::Float xl = sqrt(xx * xx + xy * xy + xz * xz);

   dt::Float zx = xy * yz - xz * yy;
   dt::Float zy = xz * yx - xx * yz;
   dt::Float zz = xx * yy - xy * yx;
   dt::Float zl = sqrt(zx * zx + zy * zy + zz * zz);

   dt::Matrixf m(4, 4);
   if (xl && yl && zl)
   {
      // Column 0;
      m(0, 0) = xx / xl;
      m(1, 0) = xy / xl;
      m(2, 0) = xz / xl;
      m(3, 0) = 0.0f;

      // Column 1;
      m(0, 1) = yx / yl;
      m(1, 1) = yy / yl;
      m(2, 1) = yz / yl;
      m(3, 1) = 0.0f;

      // Column 2;
      m(0, 2) = zx / zl;
      m(1, 2) = zy / zl;
      m(2, 2) = zz / zl;
      m(3, 2) = 0.0f;
   }
   else
   {
      // Column 0;
      m(0, 0) = 1.0f;
      m(1, 0) = 0.0f;
      m(2, 0) = 0.0f;
      m(3, 0) = 0.0f;

      // Column 1;
      m(0, 1) = 0.0f;
      m(1, 1) = 1.0f;
      m(2, 1) = 0.0f;
      m(3, 1) = 0.0f;

      // Column 2;
      m(0, 2) = 0.0f;
      m(1, 2) = 0.0f;
      m(2, 2) = 1.0f;
      m(3, 2) = 0.0f;
   }

   // Column 3;
   m(0, 3) = 0.5f * (x1 + x2);
   m(1, 3) = 0.5f * (y1 + y2);
   m(2, 3) = 0.5f * (z1 + z2);
   m(3, 3) = 1.0f;

   return m;
}


dt::LineSegmentf3 unprojectMouse(
   int x,
   int y,
   int width,
   int height,
   const dt::Matrixf & projectionMatrix,
   const dt::Matrixf & modelviewMatrix
)
{
   GLint viewport[4] = {0, 0, width, height};

   GLdouble projection[16];
   GLdouble modelview[16];
   for (size_t i = 0; i < 16; ++ i)
   {
      projection[i] = projectionMatrix.data()[i];
      modelview[i] = modelviewMatrix.data()[i];
   }

   GLdouble p[3] = {0.0f, 0.0f, 0.0f};

   gluUnProject(
      x, height - y, 0.0, modelview, projection, viewport, &p[0], &p[1], &p[2]
   );
   const dt::Pointf3 p1(
      static_cast<dt::Float>(p[0]),
      static_cast<dt::Float>(p[1]),
      static_cast<dt::Float>(p[2])
   );

   gluUnProject(
      x, height - y, 1.0, modelview, projection, viewport, &p[0], &p[1], &p[2]
   );
   const dt::Pointf3 p2(
      static_cast<dt::Float>(p[0]),
      static_cast<dt::Float>(p[1]),
      static_cast<dt::Float>(p[2])
   );

   return dt::LineSegmentf3(p1, p2);
}


}
