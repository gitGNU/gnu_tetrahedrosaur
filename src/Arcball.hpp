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


#ifndef ARCBALL_H
#define ARCBALL_H


#include "datatypes/geometry.hpp"


/***************************************************************************
 *   Arcball class declaration                                             *
 ***************************************************************************/


class Arcball
{
   public:
      Arcball(dt::Float x, dt::Float y, dt::Float z, dt::Float r);

      void reset();

      void startDrag(dt::Float x, dt::Float y);
      void startRotate(dt::Float x, dt::Float y);

      void drag(dt::Float x, dt::Float y);
      void rotate(dt::Float x, dt::Float y);

      void commit();

      bool isDragging() const;
      bool isRotating() const;

      dt::Matrixf matrix() const;

   private:
      bool mapToSphere(dt::Float vec[3], dt::Float x, dt::Float y);
      void mapToSphere1(dt::Float vec[3], dt::Float x, dt::Float y);

      bool dragging;
      bool rotating;
      dt::Float x0, y0, z0;
      dt::Float r;
      dt::Float from[3];
      dt::Float rotation[4];
      dt::Float tmpRotation[4];
};


#endif
