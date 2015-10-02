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


#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP


#include <boost/scoped_ptr.hpp>
#include <QtCore/QPoint>
#include <QtOpenGL/QGLWidget>


#include "datatypes/geometry.hpp"


class Arcball;
class Scene;


/***************************************************************************
 *   Viewport class declaration                                            *
 ***************************************************************************/


class Viewport : public QGLWidget
{
   public:
      explicit Viewport(QWidget *parent = 0);
      virtual ~Viewport();

   protected:
      virtual void mousePressEvent(QMouseEvent * event);
      virtual void mouseReleaseEvent(QMouseEvent * event);
      virtual void mouseMoveEvent(QMouseEvent * event);
      virtual void wheelEvent(QWheelEvent * event);
      virtual void initializeGL();
      virtual void paintGL();
      virtual void resizeGL(int width, int height);

   private:
      virtual void handleLeftMouseButtonPress(int x, int y);
      void setupProjectionAndViewport(int width, int height);

   protected:
      boost::scoped_ptr<Scene> m_scene;

   private:
      dt::Vectorf3 m_translation;
      dt::Matrixf m_modelView;
      QPoint m_lastPos;
      boost::scoped_ptr<Arcball> m_arcball;
};


#endif
