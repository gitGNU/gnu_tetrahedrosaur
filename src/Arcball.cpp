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
#include <cmath>


#include "Arcball.hpp"


static void _quatInit(
   dt::Float q[4],
   dt::Float x,
   dt::Float y,
   dt::Float z,
   dt::Float w
)
{
   q[0] = x;
   q[1] = y;
   q[2] = z;
   q[3] = w;
}


static void _quatMul(
   dt::Float prod[4],
   const dt::Float q1[4],
   const dt::Float q2[4]
)
{
   prod[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
   prod[1] = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
   prod[2] = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
   prod[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
}


static void _quatToMatrix(dt::Matrixf &m, const dt::Float q[4])
{
   dt::Float l = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
   dt::Float s = ((l > 0.0f) ? (2.0f / l) : 0.0f);

   dt::Float xs = q[0] * s;
   dt::Float ys = q[1] * s;
   dt::Float zs = q[2] * s;

   dt::Float wx = q[3] * xs;
   dt::Float wy = q[3] * ys;
   dt::Float wz = q[3] * zs;

   dt::Float xx = q[0] * xs;
   dt::Float xy = q[0] * ys;
   dt::Float xz = q[0] * zs;

   dt::Float yy = q[1] * ys;
   dt::Float yz = q[1] * zs;

   dt::Float zz = q[2] * zs;

   m(0, 0) = 1.0f - (yy + zz);
   m(1, 0) = xy + wz;
   m(2, 0) = xz - wy;
   m(3, 0) = 0.0f;

   m(0, 1) = xy - wz;
   m(1, 1) = 1.0f - (xx + zz);
   m(2, 1) = yz + wx;
   m(3, 1) = 0.0f;

   m(0, 2) = xz + wy;
   m(1, 2) = yz - wx;
   m(2, 2) = 1.0f - (xx + yy);
   m(3, 2) = 0.0f;

   m(0, 3) = 0.0f;
   m(1, 3) = 0.0f;
   m(2, 3) = 0.0f;
   m(3, 3) = 1.0f;
}


/***************************************************************************
 *   ArcBall class implementation                                          *
 ***************************************************************************/


Arcball::Arcball(dt::Float x, dt::Float y, dt::Float z, dt::Float r)
   : dragging(false), rotating(false), x0(x), y0(y), z0(z), r(r)
{
   reset();
}


void Arcball::reset()
{
   memset(from, 0, sizeof(from));
   _quatInit(rotation, 0.0f, 0.0f, 0.0f, 1.0f);
   _quatInit(tmpRotation, 0.0f, 0.0f, 0.0f, 1.0f);
}


void Arcball::startDrag(dt::Float x, dt::Float y)
{
   mapToSphere(from, x, y);
   dragging = true;
}


void Arcball::startRotate(dt::Float x, dt::Float y)
{
   mapToSphere1(from, x, y);
   rotating = true;
}


void Arcball::drag(dt::Float x, dt::Float y)
{
   dt::Float to[3];
   if (!mapToSphere(to, x, y)) return;

   dt::Float q[4];
   _quatInit(
      q,
      from[1] * to[2] - from[2] * to[1],
      from[2] * to[0] - from[0] * to[2],
      from[0] * to[1] - from[1] * to[0],
      from[0] * to[0] + from[1] * to[1] + from[2] * to[2]
   );

   _quatMul(tmpRotation, q, rotation);
}


void Arcball::rotate(dt::Float x, dt::Float y)
{
   dt::Float to[3];
   mapToSphere1(to, x, y);

   dt::Float q[4];
   _quatInit(
      q,
      from[1] * to[2] - from[2] * to[1],
      from[2] * to[0] - from[0] * to[2],
      from[0] * to[1] - from[1] * to[0],
      from[0] * to[0] + from[1] * to[1] + from[2] * to[2]
   );

   _quatMul(tmpRotation, q, rotation);
}


void Arcball::commit()
{
   memcpy(rotation, tmpRotation, sizeof(rotation));
   dragging = rotating = false;
}


bool Arcball::isDragging() const
{
   return dragging;
}


bool Arcball::isRotating() const
{
   return rotating;
}


dt::Matrixf Arcball::matrix() const
{
   dt::Matrixf m(4, 4);
   _quatToMatrix(m, tmpRotation);
   return m;
}


bool Arcball::mapToSphere(dt::Float vec[3], dt::Float x, dt::Float y)
{
   // Из центра сферы строим вектор в направлении точки (x, y, 0);
   vec[0] = (x - x0) / r;
   vec[1] = (y - y0) / r;
   vec[2] = (- z0) / r;

   // Определяем квадрат длины вектора;
   dt::Float length2 = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];

   if (length2 > 1.0f) return false;

   // Наклоняем вектор по оси z до пересечения с поверхностью сферы;
   vec[2] = sqrt(1.0f - length2);
   return true;
}


void Arcball::mapToSphere1(dt::Float vec[3], dt::Float x, dt::Float y)
{
   // Из центра сферы строим вектор в направлении точки (x, y, 0);
   vec[0] = (x - x0) / r;
   vec[1] = (y - y0) / r;
   vec[2] = (- z0) / r;

   // Определяем квадрат длины вектора;
   dt::Float length2 = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];

   // Проецируем вектор на плоскость экрана и нормируем;
   dt::Float n = 1.0f / sqrt(length2);
   vec[0] *= n; vec[1] *= n; vec[2] = 0.0f;
}
