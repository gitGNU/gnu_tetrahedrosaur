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


#ifndef TOOLS3D_H
#define TOOLS3D_H


#include <GL/gl.h>


namespace shader {
class Collection;
}


#include "datatypes/geometry.hpp"


class Scene;


/***************************************************************************
 *   EdgeTool3D class declaration                                          *
 ***************************************************************************/


class EdgeTool3D
{
   public:
      EdgeTool3D();
      virtual ~EdgeTool3D();

      void initializeGL();
      void paintGL(
         const shader::Collection * shaders,
         const Scene * scene,
         dt::Float x1,
         dt::Float y1,
         dt::Float z1,
         dt::Float x2,
         dt::Float y2,
         dt::Float z2,
         dt::Float length
      ) const;

   private:
      bool m_initialized;
      GLuint m_buffers[2];
};


#endif
