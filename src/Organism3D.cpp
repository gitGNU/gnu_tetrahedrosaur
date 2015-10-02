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
#include <GL/glew.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>


#include "bio/Organism.hpp"
#include "bio/OrganismDesc.hpp"
#include "datatypes/geometry.hpp"
#include "mesh/Connections.hpp"
#include "mesh/Edge.hpp"
#include "mesh/GLMesh.hpp"
#include "mesh/Vertex.hpp"
#include "shader/descriptors.hpp"
#include "shader/Collection.hpp"


#include "Organism3D.hpp"
#include "Scene.hpp"
#include "SharedGLWidget.hpp"
#include "tools3D.hpp"


/***************************************************************************
 *   Organism3D class implementation                                       *
 ***************************************************************************/


Organism3D::Organism3D(boost::shared_ptr<const bio::OrganismDesc> desc)
{
   const shader::Collection * shaders = SharedGLWidget::instance()->shaders();

   m_organism.reset(
      new bio::Organism(
         new mesh::GLMesh(
            dt::Pointf3(0.0f, 0.5f, 0.0f),
            dt::Pointf3(-0.5f, -0.5f, -0.5f),
            dt::Pointf3(0.0f, -0.5f, 0.5f),
            dt::Pointf3(0.5f, -0.5f, -0.5f),
            *shaders->feedbackShader()),
         desc
      )
   );
}


Organism3D::~Organism3D()
{
}


void Organism3D::stepOver()
{
   m_organism->stepOver();
}


void Organism3D::paintGL(
   const Scene * scene,
   PAINTING_MODE mode,
   bool showInterior,
   bool showNormals
) const
{
   const shader::Collection * shaders = SharedGLWidget::instance()->shaders();

   dt::Matrixf mv = scene->modelViewMatrix();
   dt::Matrixf mvp = scene->modelViewProjectionMatrix();

   const mesh::GLMesh &mesh = static_cast<const mesh::GLMesh &>(
      m_organism->mesh()
   );

   GLuint currentProgram = 0;
   glGetIntegerv(
      GL_CURRENT_PROGRAM,
      reinterpret_cast<GLint *>(&currentProgram)
   );

   glBindBuffer(GL_ARRAY_BUFFER, mesh.dynamicVertexBuffer());
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.triangleBuffer());

   if (mode == PM_PICK_VERTEX)
   {
      const shader::PickingShaderDesc * pickingShader(shaders->pickingShader());
      assert(pickingShader);

      glUseProgram(pickingShader->program);
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      glEnable(GL_PROGRAM_POINT_SIZE);

      glUniformMatrix4fv(
         pickingShader->mvpMatrix,
         1,
         GL_FALSE,
         &(mvp.data()[0])
      );

      glVertexAttribPointer(
         mesh::GLMesh::AT_POSITION,
         4,
         GL_FLOAT,
         GL_FALSE,
         sizeof(mesh::DynamicVertex),
         (const GLvoid *) offsetof(mesh::DynamicVertex, x)
      );

      glEnableVertexAttribArray(mesh::GLMesh::AT_POSITION);

      glDrawArrays(GL_POINTS, 0, mesh.vertexCount());

      glDisableVertexAttribArray(mesh::GLMesh::AT_POSITION);

      glDisable(GL_PROGRAM_POINT_SIZE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   }
   else
   {
      if (!showInterior)
      {
         const shader::MainShaderDesc * mainShader(shaders->mainShader());
         assert(mainShader);

         glUseProgram(mainShader->program);

         glVertexAttribPointer(
            mesh::GLMesh::AT_POSITION,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(mesh::DynamicVertex),
            (const GLvoid *) offsetof(mesh::DynamicVertex, x)
         );
         glVertexAttribPointer(
            mesh::GLMesh::AT_NORMAL,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(mesh::DynamicVertex),
            (const GLvoid *) offsetof(mesh::DynamicVertex, nx)
         );

         glUniformMatrix4fv(mainShader->mvMatrix, 1, GL_FALSE, &(mv.data()[0]));
         glUniformMatrix4fv(
            mainShader->mvpMatrix,
            1,
            GL_FALSE,
            &(mvp.data()[0])
         );
         glUniform3f(mainShader->lightSourcePosition, 0.0f, 0.0f, 10.0f);

         glEnableVertexAttribArray(mesh::GLMesh::AT_POSITION);
         glEnableVertexAttribArray(mesh::GLMesh::AT_NORMAL);

         glDrawElements(
            GL_TRIANGLES,
            mesh.triangleCount() * 3,
            GL_UNSIGNED_INT,
            (const GLvoid *) 0
         );

         glDisableVertexAttribArray(mesh::GLMesh::AT_NORMAL);
         glDisableVertexAttribArray(mesh::GLMesh::AT_POSITION);
      }
      else
      {
         const shader::InteriorShaderDesc * interiorShader(
            shaders->interiorShader()
         );
         assert(interiorShader);

         glUseProgram(interiorShader->program);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

         glUniformMatrix4fv(
            interiorShader->mvpMatrix,
            1,
            GL_FALSE,
            &(mvp.data()[0])
         );

         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_BUFFER, mesh.staticVertexTexture());
         glUniform1i(
            interiorShader->staticVertexSampler,
            0 /* active texture */
         );

         glActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_BUFFER, mesh.connectionTexture());
         glUniform1i(interiorShader->connectionSampler, 1 /* active texture */);

         glActiveTexture(GL_TEXTURE2);
         glBindTexture(GL_TEXTURE_BUFFER, mesh.dynamicVertexTexture());
         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
         glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32I, mesh.dynamicVertexBuffer());
         glUniform1i(
            interiorShader->dynamicVertexSampler,
            2 /* active texture */
         );

         glVertexAttribPointer(
            mesh::GLMesh::AT_POSITION,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(mesh::DynamicVertex),
            (const GLvoid *) offsetof(mesh::DynamicVertex, x)
         );

         glEnableVertexAttribArray(mesh::GLMesh::AT_POSITION);

         glDrawArrays(GL_POINTS, 0, mesh.vertexCount());

         glDisableVertexAttribArray(mesh::GLMesh::AT_POSITION);

         glActiveTexture(GL_TEXTURE2);
         glBindTexture(GL_TEXTURE_BUFFER, 0);

         glActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_BUFFER, 0);

         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_BUFFER, 0);

         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      if (mode == PM_EDIT)
      {
         const shader::PointShaderDesc * pointShader(shaders->pointShader());
         assert(pointShader);

         glUseProgram(pointShader->program);
         glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
         glEnable(GL_PROGRAM_POINT_SIZE);

         glUniformMatrix4fv(
            pointShader->mvpMatrix,
            1,
            GL_FALSE,
            &(mvp.data()[0])
         );
         glUniform1f(pointShader->pointSize, 3.0f);
         glUniform4f(pointShader->pointColor, 0.0f, 1.0f, 0.0f, 0.0f);
         glUniform4f(pointShader->selectionColor, 1.0f, 0.0f, 0.0f, 0.0f);

         glVertexAttribPointer(
            mesh::GLMesh::AT_POSITION,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(mesh::DynamicVertex),
            (const GLvoid *) offsetof(mesh::DynamicVertex, x)
         );

         glEnableVertexAttribArray(mesh::GLMesh::AT_POSITION);

         glDrawArrays(GL_POINTS, 0, mesh.vertexCount());

         glDisableVertexAttribArray(mesh::GLMesh::AT_POSITION);

         glDisable(GL_PROGRAM_POINT_SIZE);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      if (showNormals)
      {
         const shader::NormalShaderDesc * normalShader(shaders->normalShader());
         assert(normalShader);

         glUseProgram(normalShader->program);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

         glUniformMatrix4fv(
            normalShader->mvpMatrix,
            1,
            GL_FALSE,
            &(mvp.data()[0])
         );

         glVertexAttribPointer(
            mesh::GLMesh::AT_POSITION,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(mesh::DynamicVertex),
            (const GLvoid *) offsetof(mesh::DynamicVertex, x)
         );
         glVertexAttribPointer(
            mesh::GLMesh::AT_NORMAL,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(mesh::DynamicVertex),
            (const GLvoid *) offsetof(mesh::DynamicVertex, nx)
         );

         glEnableVertexAttribArray(mesh::GLMesh::AT_POSITION);
         glEnableVertexAttribArray(mesh::GLMesh::AT_NORMAL);

         glDrawElements(
            GL_TRIANGLES,
            mesh.triangleCount() * 3,
            GL_UNSIGNED_INT,
            (const GLvoid *) 0
         );

         glDisableVertexAttribArray(mesh::GLMesh::AT_NORMAL);
         glDisableVertexAttribArray(mesh::GLMesh::AT_POSITION);

         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
   }

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glUseProgram(currentProgram);
}


void Organism3D::paintEdgeTool(
   const Scene * scene,
   const EdgeTool3D * tool
) const
{
   const shader::Collection * shaders = SharedGLWidget::instance()->shaders();

   boost::optional<size_t> selectedEdge = m_organism->mesh().selectedEdge();
   if (selectedEdge)
   {
      boost::optional<std::pair<size_t, size_t> > vertices =
         m_organism->mesh().selectedEdgeVertices();
      assert(vertices);

      const mesh::DynamicVertex * dynamicVertices =
         m_organism->mesh().dynamicVertices();
      const mesh::Edge & edge = m_organism->mesh().edges()[*selectedEdge];
      const mesh::DynamicVertex & v1 = dynamicVertices[vertices->first];
      const mesh::DynamicVertex & v2 = dynamicVertices[vertices->second];
      tool->paintGL(
         shaders,
         scene,
         v1.x, v1.y, v1.z, v2.x, v2.y, v2.z,
         edge.equilibriumLength
      );
   }
}


void Organism3D::selectVertex(
   const boost::optional<dt::VertexId> & v,
   dt::SelectionMode selectionMode
)
{
   m_organism->selectVertex(v, selectionMode);
}


Organism3D::SELECTED_PART Organism3D::selectedPart() const
{
   if (m_organism->mesh().selectedVertex()) return SP_VERTEX;
   else if (m_organism->mesh().selectedEdge()) return SP_EDGE;

   return SP_NONE;
}
