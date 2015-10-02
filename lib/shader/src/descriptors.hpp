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


#ifndef SHADER_DESCRIPTORS_HPP
#define SHADER_DESCRIPTORS_HPP


#include <GL/gl.h>


namespace shader {
// TODO: Think about smart types;


/***************************************************************************
 *   InteriorShaderDesc structure declaration                              *
 ***************************************************************************/


struct InteriorShaderDesc
{
   explicit InteriorShaderDesc(
      GLuint program,
      GLint mvpMatrix,
      GLint dynamicVertexSampler,
      GLint staticVertexSampler,
      GLint connectionSampler
   );

   GLuint program;
   GLint mvpMatrix;
   GLint dynamicVertexSampler;
   GLint staticVertexSampler;
   GLint connectionSampler;
};


/***************************************************************************
 *   MainShaderDesc structure declaration                                  *
 ***************************************************************************/


struct MainShaderDesc
{
   explicit MainShaderDesc(
      GLuint program,
      GLint mvMatrix,
      GLint mvpMatrix,
      GLint lightSourcePosition
   );

   GLuint program;
   GLint mvMatrix;
   GLint mvpMatrix;
   GLint lightSourcePosition;
};


/***************************************************************************
 *   PickingShaderDesc structure declaration                               *
 ***************************************************************************/


struct PickingShaderDesc
{
   explicit PickingShaderDesc(GLuint program, GLint mvpMatrix);

   GLuint program;
   GLint mvpMatrix;
};


/***************************************************************************
 *   PointShaderDesc structure declaration                                 *
 ***************************************************************************/


struct PointShaderDesc
{
   explicit PointShaderDesc(
      GLuint program,
      GLint mvpMatrix,
      GLint pointSize,
      GLint pointColor,
      GLint selectionColor
   );

   GLuint program;
   GLint mvpMatrix;
   GLint pointSize;
   GLint pointColor;
   GLint selectionColor;
};


/***************************************************************************
 *   ToolShaderDesc structure declaration                                  *
 ***************************************************************************/


struct ToolShaderDesc
{
   explicit ToolShaderDesc(
      GLuint program,
      GLint mvpMatrix,
      GLint edgeToolLength
   );

   GLuint program;
   GLint mvpMatrix;
   GLint edgeToolLength;
};


/***************************************************************************
 *   NormalShaderDesc structure declaration                                *
 ***************************************************************************/


struct NormalShaderDesc
{
   explicit NormalShaderDesc(GLuint program, GLint mvpMatrix);

   GLuint program;
   GLint mvpMatrix;
};


/***************************************************************************
 *   FeedbackShaderDesc structure declaration                              *
 ***************************************************************************/


struct FeedbackShaderDesc
{
   explicit FeedbackShaderDesc(
      GLuint program,
      GLint applyForces,
      GLint dynamicVertexSampler,
      GLint staticVertexSampler,
      GLint edgeSampler,
      GLint connectionSampler
   );

   GLuint program;
   GLint applyForces;
   GLint dynamicVertexSampler;
   GLint staticVertexSampler;
   GLint edgeSampler;
   GLint connectionSampler;
};


}


#endif
