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


#ifndef FIGURE3D_HPP
#define FIGURE3D_HPP


#include <cstdlib>
#include <GL/gl.h>


#include <boost/shared_ptr.hpp>

#include "datatypes/geometry.hpp"
#include "datatypes/mesh.hpp"

namespace mesh {
class Figure;
}


class Scene;


/***************************************************************************
 *   Figure3D class declaration                                            *
 ***************************************************************************/


class Figure3D
{
   public:
      enum ATTRIBUTE_TYPE
      {
         AT_POSITION = 0,
         AT_NORMAL
      };

      explicit Figure3D(boost::shared_ptr<const mesh::Figure> figure);
      virtual ~Figure3D();

      void paintGL(const Scene * scene) const;

      boost::shared_ptr<const mesh::Figure> figure() const;

   private:
      enum BUFFER_TYPE
      {
         BT_VERTEX = 0,
         BT_TRIANGLE,
         BufferCount
      };

      GLuint m_buffers[BufferCount];
      boost::shared_ptr<const mesh::Figure> m_figure;
};


#endif
