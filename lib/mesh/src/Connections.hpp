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


#ifndef CONNECTIONS_H
#define CONNECTIONS_H


#include <GL/gl.h>


#include <boost/optional.hpp>


#include "datatypes/mesh.hpp"


#include "ColorWrappedLists.hpp"


namespace mesh {


/***************************************************************************
 *   Connections class declaration                                         *
 ***************************************************************************/


class Connections : public ColorWrappedLists
{
   public:
      enum VALUE_TYPE
      {
         VT_VERTEX = 0,
         VT_EDGE = 1,
         VT_INTERNAL = 2
      };

      struct Entry
      {
         explicit Entry(
            const dt::VertexId & vertex,
            const dt::EdgeId & edge,
            GLint internal
         );

         dt::VertexId vertex;
         dt::EdgeId edge;
         GLint internal;
      };

      explicit Connections(GLint connectionCount);
      virtual ~Connections();

      GLint create(const Entry *entries, size_t count);

      bool insertExternal(
         GLint firstColor,
         const dt::VertexId & v1,
         const dt::VertexId & v2,
         const dt::VertexId & vertex,
         const dt::EdgeId & edge,
         bool exactOrder
      );

      bool insertInternal(
         GLint firstColor,
         const dt::VertexId & vertex,
         const dt::EdgeId & edge
      );

      bool replace(
         GLint firstColor,
         const dt::VertexId & vOld,
         const dt::VertexId & vNew
      );

      bool replaceEdge(GLint firstColor, GLint eOld, GLint eNew);

      boost::optional<GLint> convertToInternal(
         GLint firstColor,
         const dt::VertexId & v
      );

      GLint findEdge(GLint firstColor, const dt::VertexId & vertex) const;
      GLint findExternalEdge(
         GLint firstColor,
         const dt::VertexId & vertex
      ) const;

      iterator findFirst(GLint firstColor, GLint value, VALUE_TYPE type);
      const_iterator findFirst(
         GLint firstColor,
         GLint value,
         VALUE_TYPE type
      ) const;

      iterator getPrevExternalVertex(GLint firstColor, const dt::VertexId & v);
      const_iterator getPrevExternalVertex(
         GLint firstColor,
         const dt::VertexId & v
      ) const;

      bool areExternalVerticesNextToEachOther(
         GLint firstColor,
         const dt::VertexId & v1,
         const dt::VertexId & v2
      ) const;
};


}


#endif
