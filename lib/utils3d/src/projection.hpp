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


#ifndef UTILS3D_PROJECTION_HPP
#define UTILS3D_PROJECTION_HPP


#include "datatypes/geometry.hpp"


namespace utils3d {


/***************************************************************************
 *   Projection utils declaration                                          *
 ***************************************************************************/


void initPerspectiveProjMatrix(
   dt::Float m[16],
   dt::Float fovy,
   int width,
   int height,
   dt::Float zNear,
   dt::Float zFar
);


dt::Matrixf perspectiveProjectionMatrix(
   dt::Float fovy,
   int width,
   int height,
   dt::Float zNear,
   dt::Float zFar
);


dt::Matrixf figureThumbnailMatrix(
   dt::Float fovy,
   int width,
   int height,
   dt::Float zNear,
   dt::Float zFar,
   const dt::Pointf3 &center,
   const dt::Vectorf3 &dimensions
);


dt::Matrixf identityMatrix();


dt::Matrixf translationMatrix(dt::Float x, dt::Float y, dt::Float z);
dt::Matrixf scaleMatrix(dt::Float x, dt::Float y, dt::Float z);


dt::Matrixf edgeToolMatrix(
   dt::Float x1,
   dt::Float y1,
   dt::Float z1,
   dt::Float x2,
   dt::Float y2,
   dt::Float z2
);


dt::LineSegmentf3 unprojectMouse(
   int x,
   int y,
   int width,
   int height,
   const dt::Matrixf & projectionMatrix,
   const dt::Matrixf & modelviewMatrix
);


}


#endif
