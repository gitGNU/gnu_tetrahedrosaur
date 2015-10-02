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


#ifndef MESH_SELECTIONHELPER_H
#define MESH_SELECTIONHELPER_H


#include <cstddef>
#include <ostream>
#include <vector>


#include <boost/optional.hpp>


#include "datatypes/mesh.hpp"


namespace mesh {


class Mesh;


/***************************************************************************
 *   SelectionHelper struct declaration                                    *
 ***************************************************************************/


struct SelectionHelper
{
   enum Action {
      A_DoNothing = 0,
      A_Deselect,
      A_Select,
      A_ClearAndSelect
   };

   explicit SelectionHelper();
   explicit SelectionHelper(
      Action action,
      const boost::optional<size_t> &vIndex = boost::optional<size_t>()
   );

   Action action;
   boost::optional<size_t> vIndex;
};


std::ostream & operator<<(std::ostream & os, const SelectionHelper & h);


SelectionHelper helpVertexSelection(
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
);


SelectionHelper helpEdgeSelection(
   const Mesh & mesh,
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
);


SelectionHelper helpTriangleSelection(
   const Mesh & mesh,
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
);


SelectionHelper helpTetrahedronSelection(
   const Mesh & mesh,
   const std::vector<dt::VertexId> & selection,
   const dt::VertexId & v
);


}


#endif
