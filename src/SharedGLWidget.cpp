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


#include <QtCore/QtGlobal>


static const char * _context = "SharedGLWidget";
static const char * _opengl32OrHigher = QT_TRANSLATE_NOOP("SharedGLWidget", "Program requires OpenGL 3.2 or higher");
static const char * _glewInitError = QT_TRANSLATE_NOOP("SharedGLWidget", "GLEW initialization error");


#include <GL/glew.h>


#include <QtCore/QString>
#include <QtOpenGL/QGLFormat>


#include "SharedGLWidget.hpp"
#include "translation.hpp"


#include "mesh/GLMesh.hpp"
#include "shader/Collection.hpp"


namespace {


SharedGLWidget * _instance = 0;


} // anonymous namespace;


/***************************************************************************
 *   SharedGLWidget class implementation                                   *
 ***************************************************************************/


SharedGLWidget * SharedGLWidget::instance()
{
   if (!_instance)
   {
      _instance = new SharedGLWidget();
   }
   return _instance;
}


void SharedGLWidget::deleteInstance()
{
   delete _instance;
   _instance = 0;
}


bool SharedGLWidget::hasError() const
{
   return (m_state == State::HasError);
}


void SharedGLWidget::initializeGL()
{
   if (m_state != State::Initial)
   {
      return;
   }

   if (!(QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_2))
   {
      m_state = State::HasError;
      m_errorText = TSLC(_opengl32OrHigher);
      return;
   }

   GLenum glewInitResult = glewInit();
   if (glewInitResult != GLEW_OK)
   {
      m_state = State::HasError;
      m_errorText = QString("%1: %2")
         .arg(TSLC(_glewInitError))
         .arg(reinterpret_cast<const char *>(
            glewGetErrorString(glewInitResult))
         );
      return;
   }

   Q_ASSERT(!m_shaders);
   m_shaders = new shader::Collection(
      mesh::GLMesh::AT_POSITION,
      mesh::GLMesh::AT_NORMAL,
      mesh::GLMesh::AT_VELOCITY
   );
   m_shaders->initialize();

   m_state = State::Initialized;
}


SharedGLWidget::SharedGLWidget()
   : QGLWidget(), m_state(State::Initial), m_shaders(0)
{
   glInit();
}


SharedGLWidget::~SharedGLWidget()
{
   delete m_shaders;
}
