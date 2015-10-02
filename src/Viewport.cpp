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


#include <QtGui/QMouseEvent>


#include "utils3d/projection.hpp"


#include "Arcball.hpp"
#include "Scene.hpp"
#include "SharedGLWidget.hpp"
#include "Viewport.hpp"


/***************************************************************************
 *   Viewport class implementation                                         *
 ***************************************************************************/


Viewport::Viewport(QWidget *parent)
   : QGLWidget(parent, SharedGLWidget::instance()),
   m_translation(0.0f, 0.0f, -7.0f),
   m_modelView(std::move(utils3d::identityMatrix()))
{
   m_arcball.reset(new Arcball(0.5f, 0.5f, 0.0f, 0.5f));
   m_scene.reset(new Scene());
}


Viewport::~Viewport()
{
}


void Viewport::mousePressEvent(QMouseEvent * event)
{
   QSize sz(size());
   int x = event->x();
   int y = event->y();
   int width = sz.width();
   int height = sz.height();

   if (event->button() == Qt::LeftButton)
   {
      handleLeftMouseButtonPress(x, y);
      m_lastPos = event->pos();
   }
   else if (event->button() == Qt::RightButton)
   {
      m_arcball->startDrag(
         ((dt::Float) x) / ((dt::Float) width),
         1.0f - ((dt::Float) y) / ((dt::Float) height)
      );
   }
}


void Viewport::mouseReleaseEvent(QMouseEvent * event)
{
   if (event->button() == Qt::RightButton)
   {
      m_arcball->commit();
   }

   const dt::Matrixf mt = boost::numeric::ublas::prod(
      m_arcball->matrix(),
      m_modelView
   );
   m_modelView = boost::numeric::ublas::prod(
      utils3d::translationMatrix(m_translation.x, m_translation.y, 0.0f),
      mt
   );

   m_translation.x = 0.0f;
   m_translation.y = 0.0f;
   m_arcball->reset();
   updateGL();
}


void Viewport::mouseMoveEvent(QMouseEvent * event)
{
   if (event->buttons() & Qt::LeftButton)
   {
      m_translation.x += 0.005f * (event->x() - m_lastPos.x());
      m_translation.y -= 0.005f * (event->y() - m_lastPos.y());
      m_lastPos = event->pos();
      updateGL();
   }
   else if ((event->buttons() & Qt::RightButton) && m_arcball->isDragging())
   {
      const QSize sz(size());
      m_arcball->drag(
         ((dt::Float) event->x()) / ((dt::Float) sz.width()),
         1.0f - ((dt::Float) event->y()) / ((dt::Float) sz.height())
      );
      updateGL();
   }
}


void Viewport::wheelEvent(QWheelEvent * event)
{
   dt::Float value = m_translation.z + (event->delta() * 0.005f);
   if (value > -1.0f)
   {
      m_translation.z = -1.0f;
      updateGL();
   }
   else if (value < -1000.0f)
   {
      m_translation.z = -1000.0f;
      updateGL();
   }
   else
   {
      m_translation.z = value;
      updateGL();
   }
}


void Viewport::initializeGL()
{
   setupProjectionAndViewport(size().width(), size().height());

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);
}


void Viewport::paintGL()
{
   // No scaling allowed due to simplified normal matrix;
   dt::Matrixf mt = boost::numeric::ublas::prod(
      m_arcball->matrix(),
      m_modelView
   );
   m_scene->setModelViewMatrix(boost::numeric::ublas::prod(
      utils3d::translationMatrix(
         m_translation.x,
         m_translation.y,
         m_translation.z
      ), mt
   ));

   glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(0.1f, 0.7f, 0.2f);
}


void Viewport::resizeGL(int width, int height)
{
   setupProjectionAndViewport(width, height);
}


void Viewport::handleLeftMouseButtonPress(int x, int y)
{
}


void Viewport::setupProjectionAndViewport(int width, int height)
{
   m_scene->setProjectionMatrix(
      utils3d::perspectiveProjectionMatrix(30.0f, width, height, 1.0f, 200.0f)
   );
   glViewport(0, 0, width, height);
}
