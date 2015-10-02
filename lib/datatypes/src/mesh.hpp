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


#ifndef DATATYPES_MESH_HPP
#define DATATYPES_MESH_HPP


#include <cstdint>
#include <cstdlib>


#include "strongtype.hpp"


namespace dt {


enum TetrahedronFace
{
   TF_ABC = 0,
   TF_ACD,
   TF_ADB,
   TF_BCD
};


TetrahedronFace tetrahedronFaceFromInt(int n);


enum SelectionMode
{
   SM_Vertex = 0,
   SM_Edge,
   SM_Triangle,
   SM_Tetrahedron
};


STRONGTYPE_DECLARATION(size_t, VertexId)
STRONGTYPE_DECLARATION(size_t, EdgeId)
STRONGTYPE_DECLARATION(size_t, TriangleId)


}


#endif
