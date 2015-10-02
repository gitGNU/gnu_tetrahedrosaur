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


#include "descriptors.hpp"


namespace shader {


/***************************************************************************
 *   InteriorShaderDesc structure implementation                           *
 ***************************************************************************/


InteriorShaderDesc::InteriorShaderDesc(
   GLuint program,
   GLint mvpMatrix,
   GLint dynamicVertexSampler,
   GLint staticVertexSampler,
   GLint connectionSampler
) : program(program),
   mvpMatrix(mvpMatrix),
   dynamicVertexSampler(dynamicVertexSampler),
   staticVertexSampler(staticVertexSampler),
   connectionSampler(connectionSampler)
{
}


/***************************************************************************
 *   MainShaderDesc structure implementation                               *
 ***************************************************************************/


MainShaderDesc::MainShaderDesc(
   GLuint program,
   GLint mvMatrix,
   GLint mvpMatrix,
   GLint lightSourcePosition
) : program(program),
   mvMatrix(mvMatrix),
   mvpMatrix(mvpMatrix),
   lightSourcePosition(lightSourcePosition)
{
}


/***************************************************************************
 *   PickingShaderDesc structure implementation                            *
 ***************************************************************************/


PickingShaderDesc::PickingShaderDesc(GLuint program, GLint mvpMatrix)
   : program(program), mvpMatrix(mvpMatrix)
{
}


/***************************************************************************
 *   PointShaderDesc structure implementation                              *
 ***************************************************************************/


PointShaderDesc::PointShaderDesc(
   GLuint program,
   GLint mvpMatrix,
   GLint pointSize,
   GLint pointColor,
   GLint selectionColor
) : program(program),
   mvpMatrix(mvpMatrix),
   pointSize(pointSize),
   pointColor(pointColor),
   selectionColor(selectionColor)
{
}


/***************************************************************************
 *   ToolShaderDesc structure implementation                               *
 ***************************************************************************/


ToolShaderDesc::ToolShaderDesc(
   GLuint program,
   GLint mvpMatrix,
   GLint edgeToolLength
) : program(program),
   mvpMatrix(mvpMatrix),
   edgeToolLength(edgeToolLength)
{
}


/***************************************************************************
 *   NormalShaderDesc structure implementation                             *
 ***************************************************************************/


NormalShaderDesc::NormalShaderDesc(GLuint program, GLint mvpMatrix)
   : program(program), mvpMatrix(mvpMatrix)
{
}


/***************************************************************************
 *   FeedbackShaderDesc structure implementation                           *
 ***************************************************************************/


FeedbackShaderDesc::FeedbackShaderDesc(
   GLuint program,
   GLint applyForces,
   GLint dynamicVertexSampler,
   GLint staticVertexSampler,
   GLint edgeSampler,
   GLint connectionSampler
) : program(program),
   applyForces(applyForces),
   dynamicVertexSampler(dynamicVertexSampler),
   staticVertexSampler(staticVertexSampler),
   edgeSampler(edgeSampler),
   connectionSampler(connectionSampler)
{
}


}
