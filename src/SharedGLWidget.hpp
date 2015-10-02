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


#ifndef SHAREDGLWIDGET_HPP
#define SHAREDGLWIDGET_HPP


#include <QtOpenGL/QGLWidget>


namespace shader {
class Collection;
}


/***************************************************************************
 *   SharedGLWidget class declaration                                      *
 ***************************************************************************/


class SharedGLWidget : public QGLWidget
{
   public:
      static SharedGLWidget * instance();
      static void deleteInstance();

      bool hasError() const;
      inline QString errorText() const {return m_errorText;}

      inline const shader::Collection * shaders() const {return m_shaders;}

   protected:
      virtual void initializeGL();

   private:
      explicit SharedGLWidget();
      virtual ~SharedGLWidget();

      enum class State {
         Initial = 0,
         Initialized,
         HasError
      };

      State m_state;
      QString m_errorText;
      shader::Collection * m_shaders;
};


#endif
