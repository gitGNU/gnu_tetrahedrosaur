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


#include <boost/test/unit_test.hpp>


#include "BuddingHelper.hpp"
#include "BuddingParams.hpp"
#include "Connections.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"


using namespace mesh;


namespace {


class _TestMesh : public Mesh
{
   public:
      explicit _TestMesh() : Mesh(
         dt::Pointf3(0.0f, 0.5f, 0.0f),
         dt::Pointf3(-0.5f, -0.5f, -0.5f),
         dt::Pointf3(0.0f, -0.5f, 0.5f),
         dt::Pointf3(0.5f, -0.5f, -0.5f)
      ) {}

      inline const MemoryModification & dynamicVertexMods() const;
      inline const MemoryModification & staticVertexMods() const;
      inline const MemoryModification & triangleMods() const;
      inline const MemoryModification & edgeMods() const;
};


inline const MemoryModification & _TestMesh::dynamicVertexMods() const
{
   return Mesh::dynamicVertexMods();
}


inline const MemoryModification & _TestMesh::staticVertexMods() const
{
   return Mesh::staticVertexMods();
}


inline const MemoryModification & _TestMesh::triangleMods() const
{
   return Mesh::triangleMods();
}


inline const MemoryModification & _TestMesh::edgeMods() const
{
   return Mesh::edgeMods();
}


}


/***************************************************************************
 *   BuddingHelper struct test                                             *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libmesh_BuddingHelper)


BOOST_AUTO_TEST_CASE(test_helpBudding)
{
   _TestMesh mesh;

   {
      const boost::optional<BuddingHelper> hlp = helpBudding(
         mesh,
         dt::VertexId(1),
         dt::VertexId(2),
         dt::VertexId(3),
         dt::VertexId(0),
         1.0f, 0.0f, 0.0f
      );
      BOOST_REQUIRE(hlp.is_initialized());
      BOOST_REQUIRE(hlp->verticesToBeAdded() == 1);
      BOOST_REQUIRE(hlp->edgesToBeAdded() == 3);
      BOOST_REQUIRE(hlp->trianglesToBeAdded() == 2);
      BOOST_REQUIRE(!hlp->vNew);
      BOOST_REQUIRE(!hlp->v0vNewEdge);
      BOOST_REQUIRE(!hlp->v1vNewEdge);
      BOOST_REQUIRE(!hlp->v2vNewEdge);
      BOOST_REQUIRE(hlp->v0v1Edge == BuddingEdgeHelper(dt::VertexId(1), dt::VertexId(3), 4, false));
      BOOST_REQUIRE(hlp->v0v2Edge == BuddingEdgeHelper(dt::VertexId(1), dt::VertexId(2), 3, false));
      BOOST_REQUIRE(hlp->v1v2Edge == BuddingEdgeHelper(dt::VertexId(3), dt::VertexId(2), 5, false));
      BOOST_REQUIRE(hlp->v0v1v2TriangleToBeDeleted.get() == 3);
      BOOST_REQUIRE(!hlp->v0v1vNTriangleToBeDeleted);
      BOOST_REQUIRE(!hlp->v0vNv2TriangleToBeDeleted);
      BOOST_REQUIRE(!hlp->v1v2vNTriangleToBeDeleted);
      BOOST_REQUIRE(hlp->triangleHelpers.empty());
   }

   mesh.makeTetrahedronBud(Tetrahedron(0, 1, 2, 3), BuddingParams(dt::TF_BCD));
   mesh.setVertexPosX(dt::VertexId(0), 4.0f);
   mesh.setVertexPosX(dt::VertexId(4), 4.0f);

   {
      const boost::optional<BuddingHelper> hlp = helpBudding(
         mesh,
         dt::VertexId(2),
         dt::VertexId(3),
         dt::VertexId(0),
         dt::VertexId(1),
         1.0f, 0.0f, 0.0f
      );
      BOOST_REQUIRE(hlp.is_initialized());
      BOOST_REQUIRE(hlp->verticesToBeAdded() == 0);
      BOOST_REQUIRE(hlp->edgesToBeAdded() == 1);
      BOOST_REQUIRE(hlp->trianglesToBeAdded() == 0);
      BOOST_REQUIRE(hlp->vNew == dt::VertexId(4));
      BOOST_REQUIRE(!hlp->v0vNewEdge);
      BOOST_REQUIRE(hlp->v1vNewEdge == BuddingEdgeHelper(dt::VertexId(2), dt::VertexId(4), 8, false));
      BOOST_REQUIRE(hlp->v2vNewEdge == BuddingEdgeHelper(dt::VertexId(3), dt::VertexId(4), 7, false));
      BOOST_REQUIRE(hlp->v0v1Edge == BuddingEdgeHelper(dt::VertexId(0), dt::VertexId(2), 1, false));
      BOOST_REQUIRE(hlp->v0v2Edge == BuddingEdgeHelper(dt::VertexId(0), dt::VertexId(3), 2, false));
      BOOST_REQUIRE(hlp->v1v2Edge == BuddingEdgeHelper(dt::VertexId(2), dt::VertexId(3), 5, true));
      BOOST_REQUIRE(hlp->v0v1v2TriangleToBeDeleted.get() == 1);
      BOOST_REQUIRE(!hlp->v0v1vNTriangleToBeDeleted);
      BOOST_REQUIRE(!hlp->v0vNv2TriangleToBeDeleted);
      BOOST_REQUIRE(hlp->v1v2vNTriangleToBeDeleted == dt::TriangleId(5));
      BOOST_REQUIRE(hlp->triangleHelpers.size() == 4);
      BOOST_REQUIRE(hlp->triangleHelpers[0] == BuddingTriangleHelper(dt::VertexId(0), dt::VertexId(2), dt::VertexId(4)));
      BOOST_REQUIRE(hlp->triangleHelpers[1] == BuddingTriangleHelper(dt::VertexId(0), dt::VertexId(4), dt::VertexId(3)));
      BOOST_REQUIRE(hlp->triangleHelpers[2] == BuddingTriangleHelper(dt::TriangleId(1)));
      BOOST_REQUIRE(hlp->triangleHelpers[3] == BuddingTriangleHelper(dt::TriangleId(5)));
   }
}


BOOST_AUTO_TEST_SUITE_END()
