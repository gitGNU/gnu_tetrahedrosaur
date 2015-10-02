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


#include <iostream>


#include <QtCore/QTimerEvent>


#include "DevelopmentEngine.hpp"
#include "GuiOrganismDesc.hpp"
#include "OrganismPixelBuffer.hpp"
#include "SharedGLWidget.hpp"


#include "bio/Organism.hpp"
#include "mesh/Figure.hpp"
#include "mesh/GLMesh.hpp"
#include "shader/Collection.hpp"


/***************************************************************************
 *   DevelopmentEngine class implementation                                *
 ***************************************************************************/


DevelopmentEngine::DevelopmentEngine(QObject * parent)
   : QObject(parent),
   m_isReady(true),
   m_processingDesc(0),
   m_lastProgress(0),
   m_timerId(0)
{
}


DevelopmentEngine::~DevelopmentEngine()
{
}


void DevelopmentEngine::start(
   const std::vector<boost::shared_ptr<GuiOrganismDesc> > & descs
)
{
   if (m_isReady)
   {
      Q_ASSERT(!m_buffer);
      Q_ASSERT(!m_processingOrganism);
      Q_ASSERT(!m_timerId);
      if (!descs.empty())
      {
         m_isReady = false;
         m_descs = descs;
         m_processingDesc = 0;
         m_lastProgress = 0;
         m_buffer.reset(new OrganismPixelBuffer(512, 512));
         m_timerId = startTimer(0);
      }
      else
      {
         emit allFinished();
      }
   }
}


void DevelopmentEngine::abort()
{
   if (m_timerId)
   {
      killTimer(m_timerId);
      m_timerId = 0;
      m_processingOrganism.reset(0);
      m_buffer.reset(0);
      m_processingDesc = 0;
      m_lastProgress = 0;
      m_descs.clear();
      m_isReady = true;
   }
}


void DevelopmentEngine::timerEvent(QTimerEvent * event)
{
   if (m_timerId && event->timerId() == m_timerId)
   {
      if (m_processingOrganism)
      {
         if (m_processingOrganism->isFinished())
         {
            boost::shared_ptr<mesh::Figure> figure(
               new mesh::Figure(m_processingOrganism->mesh())
            );
            const QImage image = m_buffer->paintFigure(figure).scaled(
               128, 128, Qt::IgnoreAspectRatio, Qt::SmoothTransformation
            );
            emit descProgressChanged(m_processingDesc, 100);
            emit descFinished(
               m_processingDesc, figure, QPixmap::fromImage(image)
            );

            m_processingOrganism.reset(0);
            if ((m_processingDesc + 1) < m_descs.size())
            {
               ++m_processingDesc;
            }
            else
            {
               killTimer(m_timerId);
               m_timerId = 0;
               m_buffer.reset(0);
               m_processingDesc = 0;
               m_lastProgress = 0;
               m_descs.clear();
               m_isReady = true;
               emit allFinished();
            }
         }
         else
         {
            m_buffer->makeCurrent();
            m_processingOrganism->stepOver();
            const int progress = m_processingOrganism->progress();
            if (m_lastProgress != progress)
            {
               m_lastProgress = progress;
               emit descProgressChanged(m_processingDesc, progress);
            }
         }
      }
      else
      {
         const shader::Collection * sh = SharedGLWidget::instance()->shaders();
         m_buffer->makeCurrent();
         std::cout << *m_descs[m_processingDesc] << std::endl << std::flush;
         m_processingOrganism.reset(new bio::Organism(
            new mesh::GLMesh(
               dt::Pointf3(0.0f, 0.5f, 0.0f),
               dt::Pointf3(-0.5f, -0.5f, -0.5f),
               dt::Pointf3(0.0f, -0.5f, 0.5f),
               dt::Pointf3(0.5f, -0.5f, -0.5f),
               *sh->feedbackShader()),
            m_descs[m_processingDesc]
         ));
         m_lastProgress = 0;
         emit descStarted(m_processingDesc);
         emit descProgressChanged(m_processingDesc, 0);
      }
   }
}
