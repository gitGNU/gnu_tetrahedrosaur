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


#include "Connections.hpp"
#include "Mesh.hpp"
#include "SelectionHelper.hpp"
#include "TrianglesMap.hpp"
#include "Vertex.hpp"


namespace {


boost::optional<size_t> _findVertexId(
   const std::vector<dt::VertexId> & ids,
   const dt::VertexId & v
)
{
   for (size_t i = 0, count = ids.size(); i < count; ++i)
   {
      if (ids[i] == v)
      {
         return i;
      }
   }
   return boost::optional<size_t>();
}


}


namespace mesh {


/***************************************************************************
 *   SelectionHelper struct implementation                                 *
 ***************************************************************************/


SelectionHelper::SelectionHelper()
   : action(A_DoNothing)
{
}


SelectionHelper::SelectionHelper(
   Action action,
   const boost::optional<size_t> &vIndex
) : action(action), vIndex(vIndex)
{
}


std::ostream & operator<<(std::ostream & os, const SelectionHelper & h)
{
   os << "{";
   switch (h.action)
   {
      case SelectionHelper::A_DoNothing:
         os << "DoNothing";
         break;
      case SelectionHelper::A_Deselect:
         os << "Deselect";
         break;
      case SelectionHelper::A_Select:
         os << "Select";
         break;
      case SelectionHelper::A_ClearAndSelect:
         os << "ClearAndSelect";
         break;
   }

   if (h.vIndex)
   {
      os << ", " << *h.vIndex;
   }

   os << "}";
   return os;
}


SelectionHelper helpVertexSelection(
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
)
{
   switch (selection.size())
   {
      case 0:
         return SelectionHelper(SelectionHelper::A_Select);
      case 1:
      {
         const boost::optional<size_t> vIndex = _findVertexId(selection, v);
         if (vIndex)
         {
            return SelectionHelper(SelectionHelper::A_Deselect, vIndex);
         }
         break;
      }
   }
   return SelectionHelper(SelectionHelper::A_ClearAndSelect);
}


SelectionHelper helpEdgeSelection(
   const Mesh & mesh,
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
)
{
   switch (selection.size())
   {
      case 0:
         return SelectionHelper(SelectionHelper::A_Select);
      case 1:
      {
         const Connections & conns = mesh.connections();
         const GLint vcn = mesh.staticVertices()[v.get()].connection;
         const boost::optional<size_t> vIndex = _findVertexId(selection, v);
         if (vIndex)
         {
            return SelectionHelper(SelectionHelper::A_Deselect, vIndex);
         }
         else if (conns.findEdge(vcn, selection[0]) >= 0)
         {
            return SelectionHelper(SelectionHelper::A_Select);
         }
         break;
      }
      case 2:
      {
         const boost::optional<size_t> vIndex = _findVertexId(selection, v);
         if (vIndex)
         {
            return SelectionHelper(SelectionHelper::A_Deselect, vIndex);
         }
         break;
      }
   }
   return SelectionHelper(SelectionHelper::A_ClearAndSelect);
}


SelectionHelper helpTriangleSelection(
   const Mesh & mesh,
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
)
{
   const size_t count = selection.size();
   switch (count)
   {
      case 0:
         return SelectionHelper(SelectionHelper::A_Select);
      case 1:
      case 2:
      {
         const Connections & conns = mesh.connections();
         const GLint vcn = mesh.staticVertices()[v.get()].connection;
         const boost::optional<size_t> vIndex = _findVertexId(selection, v);
         if (vIndex)
         {
            return SelectionHelper(SelectionHelper::A_Deselect, vIndex);
         }
         else if (conns.findExternalEdge(vcn, selection[0]) >= 0)
         {
            assert(count == 1 || mesh.trianglesMap().findAny(
               Triangle(selection[0].get(), selection[1].get(), v.get())));
            assert(count == 1 || conns.findExternalEdge(
               vcn, selection[1]) >= 0);
            return SelectionHelper(SelectionHelper::A_Select);
         }
         break;
      }
      case 3:
      {
         const boost::optional<size_t> vIndex = _findVertexId(selection, v);
         if (vIndex)
         {
            return SelectionHelper(SelectionHelper::A_Deselect, vIndex);
         }
         break;
      }
   }
   return SelectionHelper(SelectionHelper::A_ClearAndSelect);
}


SelectionHelper helpTetrahedronSelection(
   const Mesh & mesh,
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
)
{
   const size_t count = selection.size();
   switch (count)
   {
      case 0:
         return SelectionHelper(SelectionHelper::A_Select);
      case 1:
      case 2:
      case 3:
      {
         const Connections & conns = mesh.connections();
         const GLint vcn = mesh.staticVertices()[v.get()].connection;
         const boost::optional<size_t> vIndex = _findVertexId(selection, v);
         if (vIndex)
         {
            return SelectionHelper(SelectionHelper::A_Deselect, vIndex);
         }
         else if ((conns.findEdge(vcn, selection[0]) >= 0) &&
            (count < 2 || conns.findEdge(vcn, selection[1]) >= 0) &&
            (count < 3 || conns.findEdge(vcn, selection[2]) >= 0))
         {
            return SelectionHelper(SelectionHelper::A_Select);
         }
         break;
      }
      case 4:
      {
         const boost::optional<size_t> vIndex = _findVertexId(selection, v);
         if (vIndex)
         {
            return SelectionHelper(SelectionHelper::A_Deselect, vIndex);
         }
         break;
      }
   }
   return SelectionHelper(SelectionHelper::A_ClearAndSelect);
}


}
