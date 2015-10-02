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


#include <cassert>


#include "Connections.hpp"


namespace mesh {


/***************************************************************************
 *   Connections::Entry class implementation                               *
 ***************************************************************************/


Connections::Entry::Entry(
   const dt::VertexId & vertex,
   const dt::EdgeId & edge,
   GLint internal
) : vertex(vertex), edge(edge), internal(internal)
{
}


/***************************************************************************
 *   Connections class implementation                                      *
 ***************************************************************************/


Connections::Connections(GLint connectionCount)
   : ColorWrappedLists(connectionCount, ColorWrappedLists::CC_RGBA)
{
}


Connections::~Connections()
{
}


GLint Connections::create(const Entry *entries, size_t count)
{
   assert(count);

   iterator it = createFirstColor();
   assert(it.isValid());

   GLint firstColor = it.color();

   it.set(VT_VERTEX, entries[0].vertex.get());
   it.set(VT_EDGE, entries[0].edge.get());
   it.set(VT_INTERNAL, entries[0].internal);

   for (size_t i = 1; i < count; ++ i)
   {
      it = insertAfter(it);
      assert(it.isValid());
      it.set(VT_VERTEX, entries[i].vertex.get());
      it.set(VT_EDGE, entries[i].edge.get());
      it.set(VT_INTERNAL, entries[i].internal);
   }

   return firstColor;
}


bool Connections::insertExternal(
   GLint firstColor,
   const dt::VertexId & v1,
   const dt::VertexId & v2,
   const dt::VertexId & vertex,
   const dt::EdgeId & edge,
   bool exactOrder
)
{
   iterator prevIt;
   iterator firstIt = begin(firstColor);
   for (iterator it = firstIt; it.isValid() && !it.get(VT_INTERNAL); ++ it)
   {
      if (prevIt.isValid() &&
         ((dt::VertexId(prevIt.get(VT_VERTEX)) == v1 &&
            dt::VertexId(it.get(VT_VERTEX)) == v2) ||
         (!exactOrder && dt::VertexId(prevIt.get(VT_VERTEX)) == v2 &&
            dt::VertexId(it.get(VT_VERTEX)) == v1))
      )
      {
         iterator resIt = insertAfter(prevIt);
         assert(resIt.isValid());
         resIt.set(VT_VERTEX, vertex.get());
         resIt.set(VT_EDGE, edge.get());
         resIt.set(VT_INTERNAL, 0);
         return true;
      }
      prevIt = it;
   }

   if (prevIt.isValid() &&
      ((dt::VertexId(prevIt.get(VT_VERTEX)) == v1 &&
         dt::VertexId(firstIt.get(VT_VERTEX)) == v2) ||
      (!exactOrder && dt::VertexId(prevIt.get(VT_VERTEX)) == v2 &&
         dt::VertexId(firstIt.get(VT_VERTEX)) == v1))
   )
   {
      iterator resIt = insertAfter(prevIt);
      assert(resIt.isValid());
      resIt.set(VT_VERTEX, vertex.get());
      resIt.set(VT_EDGE, edge.get());
      resIt.set(VT_INTERNAL, 0);
      return true;
   }

   return false;
}


bool Connections::insertInternal(
   GLint firstColor,
   const dt::VertexId & vertex,
   const dt::EdgeId & edge
)
{
   iterator prevIt;
   iterator it = begin(firstColor);
   while (it.isValid() && !it.get(VT_INTERNAL))
   {
      prevIt = it;
      ++ it;
   }

   iterator itToInsert;
   if (prevIt.isValid())
   {
      itToInsert = prevIt;
   }
   else if (it.isValid())
   {
      itToInsert = it;
   }

   if (itToInsert.isValid())
   {
      it = insertAfter(itToInsert);
      if (it.isValid())
      {
         it.set(VT_VERTEX, vertex.get());
         it.set(VT_EDGE, edge.get());
         it.set(VT_INTERNAL, 1);
         return true;
      }
   }
   return false;
}


bool Connections::replace(
   GLint firstColor,
   const dt::VertexId & vOld,
   const dt::VertexId & vNew
)
{
   for (iterator it = begin(firstColor); it.isValid(); ++ it)
   {
      if (dt::VertexId(it.get(VT_VERTEX)) == vOld)
      {
         it.set(VT_VERTEX, vNew.get());
         return true;
      }
   }

   return false;
}


bool Connections::replaceEdge(GLint firstColor, GLint eOld, GLint eNew)
{
   bool found = false;
   for (iterator it = begin(firstColor); it.isValid(); ++ it)
   {
      if (it.get(VT_EDGE) == eOld)
      {
         it.set(VT_EDGE, eNew);
         found = true;
      }
   }

   return found;
}


boost::optional<GLint> Connections::convertToInternal(
   GLint firstColor,
   const dt::VertexId & v
)
{
   iterator prevIt;
   iterator it = begin(firstColor);
   for (; it.isValid() && !it.get(VT_INTERNAL); ++ it)
   {
      if (dt::VertexId(it.get(VT_VERTEX)) == v)
      {
         GLint newFirstColor = firstColor;
         dt::EdgeId edge(it.get(VT_EDGE));
         if (prevIt.isValid())
         {
            removeAfter(prevIt);
         }
         else
         {
            newFirstColor = removeFirst(firstColor);
         }
         bool ok = insertInternal(newFirstColor, v, edge);
         assert(ok);
         return newFirstColor;
      }
      prevIt = it;
   }
   return boost::optional<GLint>();
}


GLint Connections::findEdge(GLint firstColor, const dt::VertexId & vertex) const
{
   return findFirst(firstColor, vertex.get(), VT_VERTEX).get(VT_EDGE);
}


GLint Connections::findExternalEdge(
   GLint firstColor,
   const dt::VertexId & vertex
) const
{
   const_iterator it = begin(firstColor);
   for (; it.isValid() && !it.get(VT_INTERNAL); ++ it)
   {
      if (dt::VertexId(it.get(VT_VERTEX)) == vertex)
      {
         return it.get(VT_EDGE);
      }
   }
   return -1;
}


Connections::iterator Connections::findFirst(
   GLint firstColor,
   GLint value,
   VALUE_TYPE type)
{
   for (iterator it = begin(firstColor); it.isValid(); ++ it)
   {
      if (it.get(type) == value)
      {
         return it;
      }
   }
   return iterator();
}


Connections::const_iterator Connections::findFirst(
   GLint firstColor,
   GLint value,
   VALUE_TYPE type
) const
{
   return const_cast<Connections *>(this)->findFirst(firstColor, value, type);
}


Connections::iterator Connections::getPrevExternalVertex(
   GLint firstColor,
   const dt::VertexId & v)
{
   bool found = false;
   iterator prevIt;
   iterator it = begin(firstColor);
   for (; it.isValid() && !it.get(VT_INTERNAL); ++ it)
   {
      if (!found && dt::VertexId(it.get(VT_VERTEX)) == v)
      {
         found = true;
         if (prevIt.isValid())
         {
            return prevIt;
         }
         else // First element is the desired vertex;
         {
            continue; // Skip prevIt assignment;
         }
      }

      prevIt = it;
   }

   if (found && prevIt.isValid())
   {
      return prevIt;
   }
   return iterator();
}


Connections::const_iterator Connections::getPrevExternalVertex(
   GLint firstColor,
   const dt::VertexId & v) const
{
   return const_cast<Connections *>(this)->getPrevExternalVertex(firstColor, v);
}


bool Connections::areExternalVerticesNextToEachOther(
   GLint firstColor,
   const dt::VertexId & v1,
   const dt::VertexId & v2
) const
{
   const_iterator prevIt;
   const_iterator firstIt = begin(firstColor);
   for (const_iterator it = firstIt; it.isValid() && !it.get(VT_INTERNAL); ++ it)
   {
      if (prevIt.isValid() &&
         ((dt::VertexId(prevIt.get(VT_VERTEX)) == v1 &&
            dt::VertexId(it.get(VT_VERTEX)) == v2) ||
         (dt::VertexId(prevIt.get(VT_VERTEX)) == v2 &&
            dt::VertexId(it.get(VT_VERTEX)) == v1))
      )
      {
         return true;
      }
      prevIt = it;
   }

   if (prevIt.isValid() &&
      ((dt::VertexId(prevIt.get(VT_VERTEX)) == v1 &&
         dt::VertexId(firstIt.get(VT_VERTEX)) == v2) ||
      (dt::VertexId(prevIt.get(VT_VERTEX)) == v2 &&
         dt::VertexId(firstIt.get(VT_VERTEX)) == v1))
   )
   {
      return true;
   }

   return false;
}


}
