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


#ifndef MESH_BUDDINGHELPER_H
#define MESH_BUDDINGHELPER_H


#include <cstddef>
#include <ostream>
#include <vector>


#include <boost/optional.hpp>
#include <boost/variant.hpp>


#include "datatypes/geometry.hpp"
#include "datatypes/mesh.hpp"


#include "Triangle.hpp"


namespace mesh {


class Mesh;


/***************************************************************************
 *   BuddingEdgeHelper struct declaration                                  *
 ***************************************************************************/


struct BuddingEdgeHelper
{
   explicit BuddingEdgeHelper(
      const dt::VertexId & v0,
      const dt::VertexId & v1,
      size_t edge,
      bool convertToInternal = false
   );

   bool operator==(const BuddingEdgeHelper & other) const;

   dt::VertexId v0;
   dt::VertexId v1;
   size_t edge;
   bool convertToInternal;
};


/***************************************************************************
 *   BuddingTriangleHelper struct declaration                              *
 ***************************************************************************/


struct BuddingTriangleHelper
{
   explicit BuddingTriangleHelper(const dt::TriangleId & triangleToBeDeleted);
   explicit BuddingTriangleHelper(
      const dt::VertexId & a,
      const dt::VertexId & b,
      const dt::VertexId & c
   );

   bool operator<(const BuddingTriangleHelper & other) const;
   bool operator==(const BuddingTriangleHelper & other) const;

   boost::variant<dt::TriangleId, Triangle> variant;
};


/***************************************************************************
 *   BuddingHelper struct declaration                                      *
 ***************************************************************************/


struct BuddingHelper
{
   explicit BuddingHelper(
      const dt::Pointf3 & budTop,
      const BuddingEdgeHelper & v0v1Edge,
      const BuddingEdgeHelper & v0v2Edge,
      const BuddingEdgeHelper & v1v2Edge,
      const dt::TriangleId & v0v1v2TriangleToBeDeleted
   );

   size_t verticesToBeAdded() const;
   size_t edgesToBeAdded() const;
   size_t trianglesToBeAdded() const;

   boost::optional<dt::VertexId> vNew;
   dt::Pointf3 budTop;
   boost::optional<BuddingEdgeHelper> v0vNewEdge;
   boost::optional<BuddingEdgeHelper> v1vNewEdge;
   boost::optional<BuddingEdgeHelper> v2vNewEdge;
   BuddingEdgeHelper v0v1Edge;
   BuddingEdgeHelper v0v2Edge;
   BuddingEdgeHelper v1v2Edge;
   dt::TriangleId v0v1v2TriangleToBeDeleted;
   boost::optional<dt::TriangleId> v0v1vNTriangleToBeDeleted;
   boost::optional<dt::TriangleId> v0vNv2TriangleToBeDeleted;
   boost::optional<dt::TriangleId> v1v2vNTriangleToBeDeleted;
   std::vector<BuddingTriangleHelper> triangleHelpers;
};


std::ostream & operator<<(std::ostream & os, const BuddingHelper & t);


boost::optional<BuddingHelper> helpBudding(
   const Mesh &mesh,
   const dt::VertexId & vertex0,
   const dt::VertexId & vertex1,
   const dt::VertexId & vertex2,
   const dt::VertexId & vertex3,
   dt::Float budTopRadius,
   dt::Float budTopPolarAngle,
   dt::Float budTopAzimuthalAngle
);


}


#endif
