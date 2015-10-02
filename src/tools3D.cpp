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
#include <cstring>
#include <GL/glew.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>


#include "mesh/GLMesh.hpp" // FIXME
#include "shader/descriptors.hpp"
#include "shader/Collection.hpp"
#include "utils3d/geometry.hpp"
#include "utils3d/projection.hpp"


#include "Scene.hpp"
#include "tools3D.hpp"


#define _EDGE_TOOL_VERTEX_COUNT 256
#define _MOVING_TOOL_LENGTH 1.0f


/***************************************************************************
 *   EdgeTool3D class implementation                                       *
 ***************************************************************************/


EdgeTool3D::EdgeTool3D()
   : m_initialized(false)
{
   memset(m_buffers, 0, sizeof(m_buffers));
}


EdgeTool3D::~EdgeTool3D()
{
   if (m_initialized)
   {
      glDeleteBuffers(sizeof(m_buffers) / sizeof(GLuint), m_buffers);
   }
}


void EdgeTool3D::initializeGL()
{
   dt::Float verticesData[3 * _EDGE_TOOL_VERTEX_COUNT];

   GLuint indicesData[_EDGE_TOOL_VERTEX_COUNT];

   const dt::Float r = 0.025f;
   const dt::Float scale = 64.0f;
   const dt::Float delta =
      (2.0f * scale / ((dt::Float) _EDGE_TOOL_VERTEX_COUNT));
   dt::Float t = - scale;
   for (size_t i = 0; i < _EDGE_TOOL_VERTEX_COUNT; ++ i, t += delta)
   {
      size_t baseIndex = 3 * i;
      verticesData[baseIndex] = r * cos(t);
      verticesData[baseIndex + 1] = t / scale;
      verticesData[baseIndex + 2] = r * sin(t);
      indicesData[i] = i;
   }

   assert(!m_buffers[0]);
   assert(!m_buffers[1]);

   glGenBuffers(sizeof(m_buffers) / sizeof(GLuint), m_buffers);

   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(verticesData),
      verticesData,
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
   glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(indicesData),
      indicesData,
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   assert(!m_initialized);
   m_initialized = true;
}


void EdgeTool3D::paintGL(
   const shader::Collection * shaders,
   const Scene * scene,
   dt::Float x1,
   dt::Float y1,
   dt::Float z1,
   dt::Float x2,
   dt::Float y2,
   dt::Float z2,
   dt::Float length
) const
{
   dt::Matrixf mv = boost::numeric::ublas::prod(
      scene->modelViewMatrix(),
      utils3d::edgeToolMatrix(x1, y1, z1, x2, y2, z2)
   );
   dt::Matrixf mvp = boost::numeric::ublas::prod(scene->projectionMatrix(), mv);

   GLuint currentProgram = 0;
   glGetIntegerv(
      GL_CURRENT_PROGRAM,
      reinterpret_cast<GLint *>(&currentProgram)
   );

   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);

   const shader::ToolShaderDesc * toolShader(shaders->toolShader());
   assert(toolShader);

   glUseProgram(toolShader->program);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glVertexAttribPointer(
      mesh::GLMesh::AT_POSITION, // FIXME;
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(dt::Float) * 3,
      (const GLvoid *) 0
   );

   glUniformMatrix4fv(toolShader->mvpMatrix, 1, GL_FALSE, &(mvp.data()[0]));
   glUniform1f(toolShader->edgeToolLength, 0.5 * length);

   glEnableVertexAttribArray(mesh::GLMesh::AT_POSITION); // FIXME;

   glDrawElements(
      GL_LINE_STRIP,
      _EDGE_TOOL_VERTEX_COUNT,
      GL_UNSIGNED_INT,
      (const GLvoid *) 0
   );

   glDisableVertexAttribArray(mesh::GLMesh::AT_POSITION); // FIXME;

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glUseProgram(currentProgram);
}
