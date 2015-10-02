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


#include <GL/gl.h>


#include "mesh/Figure.hpp"
#include "utils3d/projection.hpp"


#include "Figure3D.hpp"
#include "OrganismPixelBuffer.hpp"
#include "Scene.hpp"
#include "SharedGLWidget.hpp"


/***************************************************************************
 *   OrganismPixelBuffer class implementation                              *
 ***************************************************************************/


OrganismPixelBuffer::OrganismPixelBuffer(int width, int height)
   : QGLPixelBuffer(
      width,
      height,
      SharedGLWidget::instance()->format(),
      SharedGLWidget::instance()
   )
{
   m_scene.reset(new Scene());
   makeCurrent();
   m_scene->setProjectionMatrix(
      utils3d::perspectiveProjectionMatrix(30.0f, width, height, 1.0f, 200.0f)
   );
   glViewport(0, 0, width, height);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);
}


OrganismPixelBuffer::~OrganismPixelBuffer()
{
}


QImage OrganismPixelBuffer::paintFigure(
   boost::shared_ptr<const mesh::Figure> figure
)
{
   makeCurrent();

   Figure3D figure3d(figure);
   const dt::Matrixf m = std::move(utils3d::figureThumbnailMatrix(
      30.0f, size().width(), size().height(),
      1.0f, 200.0f,
      figure->center(),
      figure->dimensions()
   ));
   m_scene->setModelViewMatrix(m);

   glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(0.1f, 0.7f, 0.2f);

   figure3d.paintGL(m_scene.get());
   return toImage();
}
