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


#ifndef MESH_TRIANGLE_H
#define MESH_TRIANGLE_H


#include <ostream>
#include <GL/gl.h>


#include <boost/optional.hpp>


namespace mesh {


/***************************************************************************
 *   Triangle structure declaration                                        *
 ***************************************************************************/


#pragma pack(push)
#pragma pack(1)


struct Triangle
{
   enum VERTEX_ORDER
   {
      VO_INVERSE = 0,
      VO_DIRECT
   };

   enum COMBINATION
   {
      C_ABC = 0,
      C_ACB,
      C_BAC,
      C_BCA,
      C_CAB,
      C_CBA,
      C_FIRST = C_ABC,
      C_LAST = C_CBA
   };

   explicit Triangle();
   explicit Triangle(GLuint a, GLuint b, GLuint c);

   Triangle replaced(GLuint x, GLuint y) const;
   Triangle combination(COMBINATION combination) const;

   bool contains(GLuint x) const;
   boost::optional<GLuint> third(GLuint x, GLuint y) const;
   boost::optional<VERTEX_ORDER> vertexOrder(GLuint x, GLuint y) const;

   bool operator==(const Triangle & other) const;
   bool operator<(const Triangle & other) const;

   GLuint a, b, c;
};


#pragma pack(pop)


std::ostream & operator<<(std::ostream & os, const Triangle & t);


}


#endif
