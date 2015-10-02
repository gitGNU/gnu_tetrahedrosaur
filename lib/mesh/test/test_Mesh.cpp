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


#include <cstring>
#include <boost/test/unit_test.hpp>


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
 *   Mesh class test                                                       *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libmesh_Mesh)


BOOST_AUTO_TEST_CASE(test_constructor)
{
   _TestMesh mesh;

   BOOST_REQUIRE(mesh.dynamicVertices());
   BOOST_REQUIRE(mesh.staticVertices());
   BOOST_REQUIRE(mesh.vertexCount() == 4);

   BOOST_REQUIRE(mesh.triangles());
   BOOST_REQUIRE(mesh.triangleCount() == 4);
   BOOST_REQUIRE(mesh.triangles()[0] == Triangle(0, 1, 2));
   BOOST_REQUIRE(mesh.triangles()[1] == Triangle(0, 2, 3));
   BOOST_REQUIRE(mesh.triangles()[2] == Triangle(0, 3, 1));
   BOOST_REQUIRE(mesh.triangles()[3] == Triangle(1, 3, 2));

   BOOST_REQUIRE(mesh.edges());
   BOOST_REQUIRE(mesh.edgeCount() == 6);

   ColorWrappedLists::const_iterator it;

   it = mesh.connections().begin(mesh.staticVertices()[0].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 1);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 0);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 2);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 1);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 3);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 2);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   it = mesh.connections().begin(mesh.staticVertices()[1].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 2);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 3);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 0);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 0);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 3);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 4);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   it = mesh.connections().begin(mesh.staticVertices()[2].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 3);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 5);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 0);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 1);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 1);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 3);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   it = mesh.connections().begin(mesh.staticVertices()[3].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 0);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 2);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 2);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 5);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 1);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 4);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   BOOST_REQUIRE(mesh.trianglesMap().size() == 4);
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(0, 1, 2)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(0, 1, 3)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(0, 2, 3)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(1, 2, 3)));
}


BOOST_AUTO_TEST_CASE(test_divideEdge)
{
   /*_TestMesh mesh;
   boost::optional<EdgeDivisionResult> result(mesh.divideEdge(3));
   BOOST_REQUIRE(result);
   BOOST_REQUIRE(result->originalEdge == 3);
   BOOST_REQUIRE(result->newEdges.size() == 3);
   BOOST_REQUIRE(result->newEdges[0] == 6);
   BOOST_REQUIRE(result->newEdges[1] == 7);
   BOOST_REQUIRE(result->newEdges[2] == 8);

   BOOST_REQUIRE(mesh.vertexCount() == 5);
   BOOST_REQUIRE(mesh.triangleCount() == 6);
   BOOST_REQUIRE(mesh.edgeCount() == 9);

   BOOST_REQUIRE(mesh.edgeConnection1(0) == 0);
   BOOST_REQUIRE(mesh.edgeConnection2(0) == 1);
   BOOST_REQUIRE(mesh.edgeConnection1(1) == 0);
   BOOST_REQUIRE(mesh.edgeConnection2(1) == 2);
   BOOST_REQUIRE(mesh.edgeConnection1(2) == 0);
   BOOST_REQUIRE(mesh.edgeConnection2(2) == 3);
   BOOST_REQUIRE(mesh.edgeConnection1(3) == 1);
   BOOST_REQUIRE(mesh.edgeConnection2(3) == 4);
   BOOST_REQUIRE(mesh.edgeConnection1(4) == 1);
   BOOST_REQUIRE(mesh.edgeConnection2(4) == 3);
   BOOST_REQUIRE(mesh.edgeConnection1(5) == 2);
   BOOST_REQUIRE(mesh.edgeConnection2(5) == 3);
   BOOST_REQUIRE(mesh.edgeConnection1(6) == 4);
   BOOST_REQUIRE(mesh.edgeConnection2(6) == 2);
   BOOST_REQUIRE(mesh.edgeConnection1(7) == 4);
   BOOST_REQUIRE(mesh.edgeConnection2(7) == 0);
   BOOST_REQUIRE(mesh.edgeConnection1(8) == 4);
   BOOST_REQUIRE(mesh.edgeConnection2(8) == 3);

   ColorWrappedLists::const_iterator it;

   it = mesh.connections().begin(mesh.staticVertices()[0].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 1);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 0);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 4);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 7);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 2);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 1);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 3);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 2);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   it = mesh.connections().begin(mesh.staticVertices()[1].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 4);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 3);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 0);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 0);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 3);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 4);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   it = mesh.connections().begin(mesh.staticVertices()[2].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 3);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 5);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 0);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 1);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 4);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 6);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   it = mesh.connections().begin(mesh.staticVertices()[3].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 0);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 2);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 2);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 5);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 4);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 8);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 1);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 4);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   it = mesh.connections().begin(mesh.staticVertices()[4].connection);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 1);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 3);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 3);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 8);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 2);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 6);
   ++ it;
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 0);
   BOOST_REQUIRE(it.get(Connections::VT_EDGE) == 7);
   ++ it;
   BOOST_REQUIRE(!it.isValid());

   BOOST_REQUIRE(mesh.trianglesMap().size() == 6);
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(0, 1, 4)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(0, 2, 4)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(0, 1, 3)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(0, 2, 3)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(1, 4, 3)));
   BOOST_REQUIRE(mesh.trianglesMap().findAny(Triangle(2, 3, 4)));

   MemoryModification dynamicVertexMods;
   dynamicVertexMods.insertArrayElement(4, sizeof(DynamicVertex));
   BOOST_REQUIRE(mesh.dynamicVertexMods() == dynamicVertexMods);

   MemoryModification staticVertexMods;
   staticVertexMods.insertArrayElement(4, sizeof(StaticVertex));
   BOOST_REQUIRE(mesh.staticVertexMods() == staticVertexMods);

   MemoryModification triangleMods;
   triangleMods.insertArrayElement(0, sizeof(Triangle));
   triangleMods.insertArrayElement(4, sizeof(Triangle));
   triangleMods.insertArrayElement(3, sizeof(Triangle));
   triangleMods.insertArrayElement(5, sizeof(Triangle));
   BOOST_REQUIRE(mesh.triangleMods() == triangleMods);

   StructureModification structMods;
   structMods.insertEdgePushBack();
   structMods.insertEdgePushBack();
   structMods.insertEdgePushBack();
   BOOST_REQUIRE(mesh.structureMods() == structMods);*/

   // TODO: check connection modifications;
}


BOOST_AUTO_TEST_CASE(test_adjacentTetrahedron)
{
   _TestMesh mesh;

   {
      auto ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_ABC));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_ACD));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_ADB));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_BCD));
      BOOST_REQUIRE(ts.empty());
   }

   mesh.makeTetrahedronBud(Tetrahedron(0, 1, 2, 3), BuddingParams(dt::TF_BCD));
   mesh.setVertexPosX(dt::VertexId(0), 4.0f);
   mesh.setVertexPosX(dt::VertexId(4), 4.0f);

   {
      auto ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_ABC));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_ACD));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_ADB));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 1, 2, 3), dt::TF_BCD));
      BOOST_REQUIRE(ts.size() == 1);
      BOOST_REQUIRE(ts[0] == Tetrahedron(1, 2, 3, 4));

      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(1, 2, 3, 4), dt::TF_ABC));
      BOOST_REQUIRE(ts.size() == 1);
      BOOST_REQUIRE(ts[0] == Tetrahedron(1, 2, 3, 0));
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(1, 2, 3, 4), dt::TF_ACD));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(1, 2, 3, 4), dt::TF_ADB));
      BOOST_REQUIRE(ts.empty());
      ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(1, 2, 3, 4), dt::TF_BCD));
      BOOST_REQUIRE(ts.empty());
   }

   mesh.makeTetrahedronBud(Tetrahedron(0, 1, 2, 3), BuddingParams(dt::TF_ACD));
   mesh.makeTetrahedronBud(Tetrahedron(0, 3, 4, 2), BuddingParams(dt::TF_ABC));

   {
      auto ts = std::move(mesh.adjacentTetrahedrons(Tetrahedron(0, 3, 4, 5), dt::TF_ABC));
      BOOST_REQUIRE(ts.size() == 2);
      BOOST_REQUIRE(ts[0] == Tetrahedron(0, 3, 4, 1));
      BOOST_REQUIRE(ts[1] == Tetrahedron(0, 3, 4, 2));
   }
}


BOOST_AUTO_TEST_SUITE_END()
