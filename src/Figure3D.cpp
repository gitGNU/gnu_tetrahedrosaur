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
#include <cstring>
#include <GL/glew.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>


#include "datatypes/geometry.hpp"
#include "mesh/Figure.hpp"
#include "mesh/Triangle.hpp"
#include "mesh/Vertex.hpp"
#include "shader/descriptors.hpp"
#include "shader/Collection.hpp"


#include "Figure3D.hpp"
#include "Scene.hpp"
#include "SharedGLWidget.hpp"


/***************************************************************************
 *   Figure3D class implementation                                         *
 ***************************************************************************/


Figure3D::Figure3D(boost::shared_ptr<const mesh::Figure> figure)
   : m_figure(figure)
{
   assert(figure);

   memset(m_buffers, 0, sizeof(m_buffers));

   glGenBuffers(sizeof(m_buffers) / sizeof(GLuint), m_buffers);

   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BT_VERTEX]);
   glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(mesh::Vertex) * figure->vertexCount(),
      figure->vertices(),
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[BT_TRIANGLE]);
   glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(mesh::Triangle) * figure->triangleCount(),
      figure->triangles(),
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


Figure3D::~Figure3D()
{
   glDeleteBuffers(sizeof(m_buffers) / sizeof(GLuint), m_buffers);
}


void Figure3D::paintGL(const Scene * scene) const
{
   const shader::Collection * shaders = SharedGLWidget::instance()->shaders();

   dt::Matrixf mv = scene->modelViewMatrix();
   dt::Matrixf mvp = scene->modelViewProjectionMatrix();

   GLuint currentProgram = 0;
   glGetIntegerv(
      GL_CURRENT_PROGRAM,
      reinterpret_cast<GLint *>(&currentProgram)
   );

   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BT_VERTEX]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[BT_TRIANGLE]);

   const shader::MainShaderDesc * mainShader(shaders->mainShader());
   assert(mainShader);

   glUseProgram(mainShader->program);

   glVertexAttribPointer(
      AT_POSITION, // FIXME:;
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(mesh::Vertex),
      (const GLvoid *) offsetof(mesh::Vertex, x)
   );
   glVertexAttribPointer(
      AT_NORMAL, // FIXME:;
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(mesh::Vertex),
      (const GLvoid *) offsetof(mesh::Vertex, nx)
   );

   glUniformMatrix4fv(mainShader->mvMatrix, 1, GL_FALSE, &(mv.data()[0]));
   glUniformMatrix4fv(
      mainShader->mvpMatrix,
      1,
      GL_FALSE,
      &(mvp.data()[0])
   );
   glUniform3f(mainShader->lightSourcePosition, 0.0f, 0.0f, 10.0f);

   glEnableVertexAttribArray(AT_POSITION); // FIXME:;
   glEnableVertexAttribArray(AT_NORMAL); // FIXME:;

   glDrawElements(
      GL_TRIANGLES,
      m_figure->triangleCount() * 3,
      GL_UNSIGNED_INT,
      (const GLvoid *) 0
   );

   glDisableVertexAttribArray(AT_NORMAL); // FIXME:;
   glDisableVertexAttribArray(AT_POSITION); // FIXME:;

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glUseProgram(currentProgram);
}


boost::shared_ptr<const mesh::Figure> Figure3D::figure() const
{
   return m_figure;
}
