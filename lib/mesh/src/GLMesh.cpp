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


#include <algorithm>
#include <cassert>
#include <cstring>
#include <GL/glew.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>


#include "Connections.hpp"
#include "Edge.hpp"
#include "GLMesh.hpp"
#include "Vertex.hpp"


namespace mesh {


static void _updateBuffer(
   GLenum target,
   GLuint buffer,
   const MemoryModification & memMod,
   const GLvoid * data
)
{
   if (!memMod.empty())
   {
      MemoryModification::const_iterator it = memMod.begin();
      MemoryModification::const_iterator ite = memMod.end();
      glBindBuffer(target, buffer);
      for (; it != ite; ++ it)
      {
         const char * bytes = reinterpret_cast<const char *>(data);
         const size_t offset = it->lower();
         const size_t size = it->upper() - offset;
         glBufferSubData(target, offset, size, &bytes[offset]);
      }
      glBindBuffer(target, 0);
   }
}


/***************************************************************************
 *   GLMesh class implementation                                           *
 ***************************************************************************/


GLMesh::GLMesh(
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c,
   const dt::Pointf3 & d,
   const shader::FeedbackShaderDesc &feedbackShader
) : Mesh(a, b, c, d),
   m_feedbackShader(feedbackShader),
   m_activeVertexBuffer(BT_DYNAMIC_VERTEX_1),
   m_backVertexBuffer(BT_DYNAMIC_VERTEX_2),
   m_dynamicVerticesSynced(true)
{
   memset(m_buffers, 0, sizeof(m_buffers));
   memset(m_textures, 0, sizeof(m_textures));

   glGenBuffers(sizeof(m_buffers) / sizeof(GLuint), m_buffers);

   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BT_DYNAMIC_VERTEX_1]);
   glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(DynamicVertex) * Mesh::MBS_MAX_VERTEX_COUNT,
      Mesh::dynamicVertices(),
      GL_STREAM_DRAW
   );
   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BT_DYNAMIC_VERTEX_2]);
   glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(DynamicVertex) * Mesh::MBS_MAX_VERTEX_COUNT,
      0,
      GL_STREAM_DRAW
   );
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[BT_TRIANGLE]);
   glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(Triangle) * Mesh::MBS_MAX_TRIANGLE_COUNT,
      triangles(),
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glBindBuffer(GL_TEXTURE_BUFFER, m_buffers[BT_STATIC_VERTEX]);
   glBufferData(
      GL_TEXTURE_BUFFER,
      sizeof(StaticVertex) * Mesh::MBS_MAX_VERTEX_COUNT,
      staticVertices(),
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_TEXTURE_BUFFER, m_buffers[BT_EDGE]);
   glBufferData(
      GL_TEXTURE_BUFFER,
      sizeof(Edge) * Mesh::MBS_MAX_EDGE_COUNT,
      edges(),
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_TEXTURE_BUFFER, m_buffers[BT_CONNECTION]);
   glBufferData(
      GL_TEXTURE_BUFFER,
      sizeof(GLint) * Mesh::MBS_MAX_CONNECTION_COUNT * 4,
      connections().data(),
      GL_STATIC_DRAW
   );
   glBindBuffer(GL_TEXTURE_BUFFER, 0);

   glGenTextures(sizeof(m_textures) / sizeof(GLuint), m_textures);

   glBindTexture(GL_TEXTURE_BUFFER, m_textures[TT_STATIC_VERTEX]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, m_buffers[BT_STATIC_VERTEX]);

   glBindTexture(GL_TEXTURE_BUFFER, m_textures[TT_EDGE]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, m_buffers[BT_EDGE]);

   glBindTexture(GL_TEXTURE_BUFFER, m_textures[TT_CONNECTION]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32I, m_buffers[BT_CONNECTION]);

   glBindTexture(GL_TEXTURE_BUFFER, 0);

   updateNormals();
}


GLMesh::~GLMesh()
{
   glDeleteTextures(sizeof(m_textures) / sizeof(GLuint), m_textures);
   glDeleteBuffers(sizeof(m_buffers) / sizeof(GLuint), m_buffers);
}


boost::optional<Tetrahedron> GLMesh::makeTetrahedronBud(
   const Tetrahedron & t,
   const BuddingParams & params
)
{
   boost::optional<Tetrahedron> result = Mesh::makeTetrahedronBud(t, params);
   if (result)
   {
      pushModifications();
      updateNormals();
   }
   return result;
}


void GLMesh::resizeEdge(GLint edge, dt::Float equilibriumLength)
{
   Mesh::resizeEdge(edge, equilibriumLength);
   pushModifications();
   updateNormals();
}


void GLMesh::setVertexPos(
   const dt::VertexId & v,
   dt::Float x,
   dt::Float y,
   dt::Float z
)
{
   Mesh::setVertexPos(v, x, y, z);
   pushModifications();
   updateNormals();
}


void GLMesh::setVertexPosX(const dt::VertexId & v, dt::Float x)
{
   Mesh::setVertexPosX(v, x);
   pushModifications();
   updateNormals();
}


void GLMesh::setVertexPosY(const dt::VertexId & v, dt::Float y)
{
   Mesh::setVertexPosY(v, y);
   pushModifications();
   updateNormals();
}


void GLMesh::setVertexPosZ(const dt::VertexId & v, dt::Float z)
{
   Mesh::setVertexPosZ(v, z);
   pushModifications();
   updateNormals();
}


void GLMesh::setVertexMass(const dt::VertexId & v, dt::Float mass)
{
   Mesh::setVertexMass(v, mass);
   pushModifications();
   updateNormals();
}


void GLMesh::selectVertex(
   const boost::optional<dt::VertexId> & vertex,
   dt::SelectionMode selectionMode
)
{
   Mesh::selectVertex(vertex, selectionMode);
   pushModifications();
}


const DynamicVertex * GLMesh::dynamicVertices() const
{
   const DynamicVertex * dv = Mesh::dynamicVertices();
   if (!m_dynamicVerticesSynced)
   {
      glBindBuffer(GL_ARRAY_BUFFER, m_buffers[m_activeVertexBuffer]);
      glGetBufferSubData(
         GL_ARRAY_BUFFER, 0,
         sizeof(DynamicVertex) * vertexCount(),
         const_cast<DynamicVertex *>(dv)
      );
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      m_dynamicVerticesSynced = true;
   }
   return dv;
}


void GLMesh::updateNormals()
{
   doTransformFeedback(false);
}


void GLMesh::doTransformFeedback(bool applyForces)
{
   GLuint currentProgram = 0;
   glGetIntegerv(
      GL_CURRENT_PROGRAM,
      reinterpret_cast<GLint *>(&currentProgram)
   );

   glUseProgram(m_feedbackShader.program);
   glEnable(GL_RASTERIZER_DISCARD);

   glBindBuffer(GL_ARRAY_BUFFER, m_buffers[m_activeVertexBuffer]);
   glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_buffers[m_backVertexBuffer]);

   glVertexAttribPointer(
      AT_POSITION,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(DynamicVertex),
      (const GLvoid *) offsetof(DynamicVertex, x)
   );
   glVertexAttribPointer(
      AT_NORMAL,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(DynamicVertex),
      (const GLvoid *) offsetof(DynamicVertex, nx)
   );
   glVertexAttribPointer(
      AT_VELOCITY,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(DynamicVertex),
      (const GLvoid *) offsetof(DynamicVertex, vx)
   );

   glUniform1i(m_feedbackShader.applyForces, applyForces);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_BUFFER, m_textures[TT_STATIC_VERTEX]);
   glUniform1i(m_feedbackShader.staticVertexSampler, 0 /* active texture */);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_BUFFER, m_textures[TT_EDGE]);
   glUniform1i(m_feedbackShader.edgeSampler, 1 /* active texture */);

   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_BUFFER, m_textures[TT_CONNECTION]);
   glUniform1i(m_feedbackShader.connectionSampler, 2 /* active texture */);

   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_BUFFER, m_textures[TT_DYNAMIC_VERTEX]);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32I, m_buffers[m_activeVertexBuffer]);
   glUniform1i(m_feedbackShader.dynamicVertexSampler, 3 /* active texture */);

   glEnableVertexAttribArray(AT_POSITION);
   glEnableVertexAttribArray(AT_NORMAL);
   glEnableVertexAttribArray(AT_VELOCITY);

   glBeginTransformFeedback(GL_POINTS);
   glDrawArrays(GL_POINTS, 0, vertexCount());
   glEndTransformFeedback();

   glDisableVertexAttribArray(AT_VELOCITY);
   glDisableVertexAttribArray(AT_NORMAL);
   glDisableVertexAttribArray(AT_POSITION);

   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE_BUFFER, 0);

   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_BUFFER, 0);

   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_BUFFER, 0);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_BUFFER, 0);

   glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glDisable(GL_RASTERIZER_DISCARD);
   glUseProgram(currentProgram);

   std::swap(m_activeVertexBuffer, m_backVertexBuffer);

   m_dynamicVerticesSynced = false;
}


void GLMesh::pushModifications()
{
   _updateBuffer(
      GL_ARRAY_BUFFER,
      m_buffers[m_activeVertexBuffer],
      dynamicVertexMods(),
      Mesh::dynamicVertices()
   );

   _updateBuffer(
      GL_TEXTURE_BUFFER,
      m_buffers[BT_STATIC_VERTEX],
      staticVertexMods(),
      staticVertices()
   );

   _updateBuffer(
      GL_ELEMENT_ARRAY_BUFFER,
      m_buffers[BT_TRIANGLE],
      triangleMods(),
      triangles()
   );

   _updateBuffer(
      GL_TEXTURE_BUFFER,
      m_buffers[BT_EDGE],
      edgeMods(),
      edges()
   );

   _updateBuffer(
      GL_TEXTURE_BUFFER,
      m_buffers[BT_CONNECTION],
      *connections().memoryModification(),
      connections().data()
   );

   clearMemoryModifications();
}


}
