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


#include <GL/glew.h>


#include <QtCore/QTimerEvent>
#include <QtGui/QWheelEvent>


#include "Organism3D.hpp"
#include "OrganismViewport.hpp"
#include "tools3D.hpp"


#include "bio/OrganismDesc.hpp"


/***************************************************************************
 *   OrganismViewport class implementation                                 *
 ***************************************************************************/


OrganismViewport::OrganismViewport(
   boost::shared_ptr<const bio::OrganismDesc> desc,
   QWidget *parent
) : Viewport(parent),
   m_pause(true),
   m_selectionMode(dt::SM_Tetrahedron),
   m_showInterior(false),
   m_showNormals(false),
   m_idleTimerId(0),
   m_desc(desc),
   m_framebufferForPicking(0),
   m_renderbufferForPicking(0)
{
   m_edgeTool.reset(new EdgeTool3D());
}


OrganismViewport::~OrganismViewport()
{
   if (m_renderbufferForPicking)
   {
      glDeleteRenderbuffers(1, &m_renderbufferForPicking);
   }

   if (m_framebufferForPicking)
   {
      glDeleteFramebuffers(1, &m_framebufferForPicking);
   }
}


void OrganismViewport::pause(bool paused)
{
   m_pause = paused;
   if (m_pause)
   {
      killTimer(m_idleTimerId);
      m_idleTimerId = 0;
   }
   else
   {
      if (m_organism)
      {
         m_organism->selectVertex(boost::none);
         emit selectionChanged();
      }
      m_idleTimerId = startTimer(500);
   }
   updateGL();
}


void OrganismViewport::showInterior(bool shown)
{
   m_showInterior = shown;
   updateGL();
}


void OrganismViewport::showNormals(bool shown)
{
   m_showNormals = shown;
   updateGL();
}


boost::shared_ptr<bio::Organism> OrganismViewport::organism() const
{
   return m_organism->organism();
}


void OrganismViewport::setSelectionMode(dt::SelectionMode selectionMode)
{
   m_selectionMode = selectionMode;
   m_organism->selectVertex(boost::none);
   emit selectionChanged();
}


void OrganismViewport::timerEvent(QTimerEvent * event)
{
   if (m_idleTimerId && event->timerId() == m_idleTimerId)
   {
      m_organism->stepOver();
      updateGL();
   }
}


void OrganismViewport::wheelEvent(QWheelEvent * event)
{
   Viewport::wheelEvent(event);
   if (m_pause && m_organism &&
      m_organism->selectedPart() == Organism3D::SP_EDGE
   )
   {
      emit edgeResized(event->delta() * 0.001);
   }
}


void OrganismViewport::initializeGL()
{
   Viewport::initializeGL();

   glGenFramebuffers(1, &m_framebufferForPicking);
   glGenRenderbuffers(1, &m_renderbufferForPicking);

   glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferForPicking);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_R32I, 1, 1);
   glBindRenderbuffer(GL_RENDERBUFFER, 0);

   glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferForPicking);
   glFramebufferRenderbuffer(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_RENDERBUFFER,
      m_renderbufferForPicking
   );
   GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
   assert(status == GL_FRAMEBUFFER_COMPLETE);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   assert(!m_organism);
   m_organism.reset(new Organism3D(m_desc));
   m_edgeTool->initializeGL();
   emit selectionChanged();
}


void OrganismViewport::paintGL()
{
   Viewport::paintGL();
   m_organism->paintGL(
      m_scene.get(),
      m_pause ? Organism3D::PM_EDIT : Organism3D::PM_VIEW,
      m_showInterior,
      m_showNormals
   );
   m_organism->paintEdgeTool(m_scene.get(), m_edgeTool.get());
}


void OrganismViewport::handleLeftMouseButtonPress(int x, int y)
{
   // Try to pick a vertex;
   if (m_pause && m_organism)
   {
      const boost::optional<dt::VertexId> v = pickVertex(x, y);
      if (v) {
         m_organism->selectVertex(v, m_selectionMode);
         emit selectionChanged();
         updateGL();
      }
   }
}


boost::optional<dt::VertexId> OrganismViewport::pickVertex(int x, int y) const
{
   if (m_framebufferForPicking) {
      QSize sz(size());
      glViewport(-x, -(sz.height() - y), sz.width(), sz.height());
      glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferForPicking);
      GLenum bufferToBeDrawnInto = GL_COLOR_ATTACHMENT0;
      glDrawBuffers(1, &bufferToBeDrawnInto);

      GLint pseudoColor[4] = {-1, 0, 0, 0};
      glClearBufferiv(
         GL_COLOR,
         0, // index of color buffers in glDrawBuffers();
         pseudoColor
      );
      m_organism->paintGL(
         m_scene.get(),
         Organism3D::PM_PICK_VERTEX,
         m_showInterior,
         false
      );

      GLint vertexId = -1;
      glReadBuffer(GL_COLOR_ATTACHMENT0);
      glReadPixels(0, 0, 1, 1, GL_RED_INTEGER, GL_INT, &vertexId);
      glViewport(0, 0, sz.width(), sz.height());
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      if (vertexId >= 0)
      {
         return dt::VertexId(static_cast<dt::VertexId::WeakType>(vertexId));
      }
   }
   return boost::none;
}
