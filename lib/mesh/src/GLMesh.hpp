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


#ifndef MESH_GLMESH_H
#define MESH_GLMESH_H


#include <cstdlib>
#include <GL/gl.h>


#include <boost/optional.hpp>


#include "Mesh.hpp"


#include "shader/descriptors.hpp"


namespace mesh {


/***************************************************************************
 *   GLMesh class declaration                                              *
 ***************************************************************************/


class GLMesh : public Mesh
{
   public:
      enum ATTRIBUTE_TYPE
      {
         AT_POSITION = 0,
         AT_NORMAL,
         AT_VELOCITY
      };

      explicit GLMesh(
         const dt::Pointf3 & a,
         const dt::Pointf3 & b,
         const dt::Pointf3 & c,
         const dt::Pointf3 & d,
         const shader::FeedbackShaderDesc &feedbackShader
      );
      virtual ~GLMesh();

      virtual boost::optional<Tetrahedron> makeTetrahedronBud(
         const Tetrahedron & t,
         const BuddingParams & params
      );

      virtual void resizeEdge(GLint edge, dt::Float equilibriumLength);
      virtual void setVertexPos(
         const dt::VertexId & v,
         dt::Float x,
         dt::Float y,
         dt::Float z
      );
      virtual void setVertexPosX(const dt::VertexId & v, dt::Float x);
      virtual void setVertexPosY(const dt::VertexId & v, dt::Float y);
      virtual void setVertexPosZ(const dt::VertexId & v, dt::Float z);
      virtual void setVertexMass(const dt::VertexId & v, dt::Float mass);
      virtual void selectVertex(
         const boost::optional<dt::VertexId> & vertex,
         dt::SelectionMode selectionMode = dt::SM_Vertex
      );

      virtual const DynamicVertex * dynamicVertices() const;

      inline GLuint dynamicVertexBuffer() const;
      inline GLuint triangleBuffer() const;

      inline GLuint dynamicVertexTexture() const;
      inline GLuint staticVertexTexture() const;
      inline GLuint connectionTexture() const;

   private:
      enum BUFFER_TYPE
      {
         BT_DYNAMIC_VERTEX_1 = 0,
         BT_DYNAMIC_VERTEX_2,
         BT_TRIANGLE,
         BT_STATIC_VERTEX,
         BT_EDGE,
         BT_CONNECTION,
         BufferCount
      };

      enum TEXTURE_TYPE
      {
         TT_DYNAMIC_VERTEX = 0,
         TT_STATIC_VERTEX,
         TT_EDGE,
         TT_CONNECTION,
         TextureCount
      };

      void updateNormals();
      void doTransformFeedback(bool applyForces);

      void pushModifications();

      shader::FeedbackShaderDesc m_feedbackShader;
      GLuint m_buffers[BufferCount];
      GLuint m_textures[TextureCount];
      int m_activeVertexBuffer;
      int m_backVertexBuffer;
      mutable bool m_dynamicVerticesSynced;
};


inline GLuint GLMesh::dynamicVertexBuffer() const
{
   return m_buffers[m_activeVertexBuffer];
}


inline GLuint GLMesh::triangleBuffer() const
{
   return m_buffers[BT_TRIANGLE];
}


inline GLuint GLMesh::dynamicVertexTexture() const
{
   return m_textures[TT_DYNAMIC_VERTEX];
}


inline GLuint GLMesh::staticVertexTexture() const
{
   return m_textures[TT_STATIC_VERTEX];
}


inline GLuint GLMesh::connectionTexture() const
{
   return m_textures[TT_CONNECTION];
}


}


#endif
