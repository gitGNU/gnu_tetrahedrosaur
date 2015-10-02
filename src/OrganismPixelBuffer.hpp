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


#ifndef ORGANISMPIXELBUFFER_H
#define ORGANISMPIXELBUFFER_H


#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>



#include <QtOpenGL/QGLPixelBuffer>


class Scene;


namespace mesh {
class Figure;
}


/***************************************************************************
 *   OrganismPixelBuffer class declaration                                 *
 ***************************************************************************/


class OrganismPixelBuffer : public QGLPixelBuffer
{
   public:
      explicit OrganismPixelBuffer(int width, int height);
      virtual ~OrganismPixelBuffer();

      QImage paintFigure(boost::shared_ptr<const mesh::Figure> figure);

   private:
      boost::scoped_ptr<Scene> m_scene;
};


#endif
