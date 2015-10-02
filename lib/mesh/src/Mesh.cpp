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


#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>


#include "utils3d/geometry.hpp"


#include "BuddingHelper.hpp"
#include "BuddingParams.hpp"
#include "Connections.hpp"
#include "Edge.hpp"
#include "Mesh.hpp"
#include "SelectionHelper.hpp"
#include "Vertex.hpp"


namespace {

void _convertToInternal(
   mesh::StaticVertex * staticVertices,
   mesh::Connections * connections,
   mesh::MemoryModification & staticVertexMods,
   const dt::VertexId & v,
   const dt::VertexId & connV,
   size_t edge
)
{
   boost::optional<GLint> newFirstColor = connections->convertToInternal(
      staticVertices[v.get()].connection,
      connV
   );
   assert(newFirstColor);
   if (staticVertices[v.get()].connection != *newFirstColor)
   {
      staticVertices[v.get()].connection = *newFirstColor;
      staticVertexMods.insertArrayElement(v.get(), sizeof(mesh::StaticVertex));
   }
   assert(connections->findEdge(staticVertices[v.get()].connection, connV) ==
      static_cast<GLint>(edge));
}

} // anonymous namespace;


namespace mesh {


/***************************************************************************
 *   Mesh class implementation                                             *
 ***************************************************************************/


Mesh::Mesh(
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c,
   const dt::Pointf3 & d
) : m_dynamicVertices(0),
   m_staticVertices(0),
   m_triangles(0),
   m_edges(0),
   m_connections(0),
   m_vertexCount(0),
   m_triangleCount(0),
   m_edgeCount(0)
{
   m_dynamicVertices = new DynamicVertex[MBS_MAX_VERTEX_COUNT];
   m_dynamicVertices[0] = DynamicVertex(a.x, a.y, a.z);
   m_dynamicVertices[1] = DynamicVertex(b.x, b.y, b.z);
   m_dynamicVertices[2] = DynamicVertex(c.x, c.y, c.z);
   m_dynamicVertices[3] = DynamicVertex(d.x, d.y, d.z);

   m_staticVertices = new StaticVertex[MBS_MAX_VERTEX_COUNT];

   m_vertexCount = 4;

   m_triangles = new Triangle[MBS_MAX_TRIANGLE_COUNT];
   m_triangles[0] = Triangle(0, 1, 2);
   m_triangles[1] = Triangle(0, 2, 3);
   m_triangles[2] = Triangle(0, 3, 1);
   m_triangles[3] = Triangle(1, 3, 2);
   m_triangleCount = 4;

   m_trianglesMap.insert(m_triangles[0], dt::TriangleId(0));
   m_trianglesMap.insert(m_triangles[1], dt::TriangleId(1));
   m_trianglesMap.insert(m_triangles[2], dt::TriangleId(2));
   m_trianglesMap.insert(m_triangles[3], dt::TriangleId(3));

   m_edges = new Edge[MBS_MAX_EDGE_COUNT];
   m_edgeCount = 6;
   {
      const DynamicVertex * dv = m_dynamicVertices;
      m_edges[0] = Edge(utils3d::distance(dv[0].point(), dv[1].point()));
      m_edges[1] = Edge(utils3d::distance(dv[0].point(), dv[2].point()));
      m_edges[2] = Edge(utils3d::distance(dv[0].point(), dv[3].point()));
      m_edges[3] = Edge(utils3d::distance(dv[1].point(), dv[2].point()));
      m_edges[4] = Edge(utils3d::distance(dv[1].point(), dv[3].point()));
      m_edges[5] = Edge(utils3d::distance(dv[2].point(), dv[3].point()));
   }

   m_connections = new Connections(MBS_MAX_CONNECTION_COUNT);

   // Order is necessary for proper normal calculation and
   // is required by vertexCanBeRemoved();
   const Connections::Entry entries0[] = {
      Connections::Entry(dt::VertexId(1), dt::EdgeId(0), 0),
      Connections::Entry(dt::VertexId(2), dt::EdgeId(1), 0),
      Connections::Entry(dt::VertexId(3), dt::EdgeId(2), 0)
   };
   const Connections::Entry entries1[] = {
      Connections::Entry(dt::VertexId(2), dt::EdgeId(3), 0),
      Connections::Entry(dt::VertexId(0), dt::EdgeId(0), 0),
      Connections::Entry(dt::VertexId(3), dt::EdgeId(4), 0)
   };
   const Connections::Entry entries2[] = {
      Connections::Entry(dt::VertexId(3), dt::EdgeId(5), 0),
      Connections::Entry(dt::VertexId(0), dt::EdgeId(1), 0),
      Connections::Entry(dt::VertexId(1), dt::EdgeId(3), 0)
   };
   const Connections::Entry entries3[] = {
      Connections::Entry(dt::VertexId(0), dt::EdgeId(2), 0),
      Connections::Entry(dt::VertexId(2), dt::EdgeId(5), 0),
      Connections::Entry(dt::VertexId(1), dt::EdgeId(4), 0)
   };

   m_staticVertices[0].connection = m_connections->create(
      entries0,
      sizeof(entries0) / sizeof(Connections::Entry)
   );
   m_staticVertices[1].connection = m_connections->create(
      entries1,
      sizeof(entries1) / sizeof(Connections::Entry)
   );
   m_staticVertices[2].connection = m_connections->create(
      entries2,
      sizeof(entries2) / sizeof(Connections::Entry)
   );
   m_staticVertices[3].connection = m_connections->create(
      entries3,
      sizeof(entries3) / sizeof(Connections::Entry)
   );
}


Mesh::~Mesh()
{
   delete[] m_dynamicVertices;
   delete[] m_staticVertices;
   delete[] m_triangles;
   delete[] m_edges;
   delete m_connections;
}


boost::optional<Tetrahedron> Mesh::makeTetrahedronBud(
   const Tetrahedron & t,
   const BuddingParams & params
)
{
   dt::VertexId v0(t.a);
   dt::VertexId v1(t.b);
   dt::VertexId v2(t.c);
   dt::VertexId v3(t.d);
   if (params.face == dt::TF_ACD)
   {
      v0 = dt::VertexId(t.a);
      v1 = dt::VertexId(t.c);
      v2 = dt::VertexId(t.d);
      v3 = dt::VertexId(t.b);
   }
   else if (params.face == dt::TF_ADB)
   {
      v0 = dt::VertexId(t.a);
      v1 = dt::VertexId(t.d);
      v2 = dt::VertexId(t.b);
      v3 = dt::VertexId(t.c);
   }
   else if (params.face == dt::TF_BCD)
   {
      v0 = dt::VertexId(t.b);
      v1 = dt::VertexId(t.c);
      v2 = dt::VertexId(t.d);
      v3 = dt::VertexId(t.a);
   }

   // TODO: check that vertices are properly connected to each other;

   boost::optional<Tetrahedron> result;
   boost::optional<BuddingHelper> helper = helpBudding(
      *this, v0, v1, v2, v3,
      params.radius, params.polarAngle, params.azimuthalAngle
   );
   if (!helper)
   {
      return result;
   }

   if (m_vertexCount + helper->verticesToBeAdded() > MBS_MAX_VERTEX_COUNT)
   {
      return result;
   }

   if (m_edgeCount + helper->edgesToBeAdded() > MBS_MAX_EDGE_COUNT)
   {
      return result;
   }

   if (m_triangleCount + helper->trianglesToBeAdded() > MBS_MAX_TRIANGLE_COUNT)
   {
      return result;
   }

   // This check will work for both states of helper->vNew;
   if (!ensureNoCollisionsForBud(
      helper->v0v1v2TriangleToBeDeleted,
      helper->budTop
   ))
   {
      return result;
   }

   clearSelection();

   // Set proper vertex order;
   v0 = dt::VertexId(m_triangles[helper->v0v1v2TriangleToBeDeleted.get()].a);
   v1 = dt::VertexId(m_triangles[helper->v0v1v2TriangleToBeDeleted.get()].b);
   v2 = dt::VertexId(m_triangles[helper->v0v1v2TriangleToBeDeleted.get()].c);

   if (helper->vNew) {
      std::cout << "makeTetrahedronBud(" << v0.get() << ", " << v1.get() <<
         ", " << v2.get() << ", " << v3.get() << "); => " <<
         helper->vNew->get() << " existing" << std::endl;
      assert(
         (helper->v0vNewEdge && helper->v1vNewEdge) ||
         (helper->v0vNewEdge && helper->v2vNewEdge) ||
         (helper->v1vNewEdge && helper->v2vNewEdge)
      );
      assert(
         helper->v0v1vNTriangleToBeDeleted ||
         helper->v0vNv2TriangleToBeDeleted ||
         helper->v1v2vNTriangleToBeDeleted
      );

      m_connections->startMemoryModificationLogging();

      dt::VertexId vNew = *helper->vNew;

      helpEdgeBudding(
         helper->v0vNewEdge, helper->v1vNewEdge, helper->v2vNewEdge,
         v0, vNew, v1, v2
      );

      helpEdgeBudding(
         helper->v1vNewEdge, helper->v0vNewEdge, helper->v2vNewEdge,
         v1, vNew, v2, v0
      );

      helpEdgeBudding(
         helper->v2vNewEdge, helper->v0vNewEdge, helper->v1vNewEdge,
         v2, vNew, v0, v1
      );

      helpEdgeBudding(helper->v0v1Edge, v0, v1);
      helpEdgeBudding(helper->v0v2Edge, v0, v2);
      helpEdgeBudding(helper->v1v2Edge, v1, v2);

      // TriangleIds are guaranteed to be sorted in ascending order. In this
      // case the removing of an id won't cause the invalidation of any
      // subsequent id;
      //
      // triangleHelpers already contains v0v1v2TriangleToBeDeleted;
      for (int i = helper->triangleHelpers.size() - 1; i >= 0; --i)
      {
         const BuddingTriangleHelper & trHlp = helper->triangleHelpers[i];
         if (const dt::TriangleId * triangleToBeDeleted =
            boost::get<dt::TriangleId>(&trHlp.variant))
         {
            removeTriangle(*triangleToBeDeleted);
         }
         else
         {
            const Triangle * triangleToBeCreated =
               boost::get<Triangle>(&trHlp.variant);
            assert(triangleToBeCreated);
            createTriangle(
               dt::VertexId(triangleToBeCreated->a),
               dt::VertexId(triangleToBeCreated->b),
               dt::VertexId(triangleToBeCreated->c)
            );
         }
      }

      m_connections->stopMemoryModificationLogging();

      result = Tetrahedron(v0.get(), v1.get(), v2.get(), vNew.get());
   } else {
      assert(!helper->v0vNewEdge);
      assert(!helper->v1vNewEdge);
      assert(!helper->v2vNewEdge);
      assert(!helper->v0v1vNTriangleToBeDeleted);
      assert(!helper->v0vNv2TriangleToBeDeleted);
      assert(!helper->v1v2vNTriangleToBeDeleted);

      m_connections->startMemoryModificationLogging();

      dt::VertexId vNew = createBudTopVertex(v0, v1, v2, helper->budTop);
      dt::EdgeId v0vNewEdge = createEdge(v0, vNew);
      dt::EdgeId v1vNewEdge = createEdge(v1, vNew);
      dt::EdgeId v2vNewEdge = createEdge(v2, vNew);

      std::cout << "makeTetrahedronBud(" << v0.get() << ", " << v1.get() <<
         ", " << v2.get() << ", " << v3.get() << "); => new " << vNew.get() <<
         std::endl;

      const Connections::Entry entries[] = {
         Connections::Entry(v0, v0vNewEdge, 0),
         Connections::Entry(v1, v1vNewEdge, 0),
         Connections::Entry(v2, v2vNewEdge, 0)
      };
      m_staticVertices[vNew.get()].connection = m_connections->create(
         entries,
         sizeof(entries) / sizeof(Connections::Entry)
      );
      m_staticVertexMods.insertArrayElement(vNew.get(), sizeof(StaticVertex));

      bool ok = false;

      ok = m_connections->insertExternal(
         m_staticVertices[v0.get()].connection,
         v1, v2,
         vNew, v0vNewEdge,
         true
      );
      assert(ok);

      ok = m_connections->insertExternal(
         m_staticVertices[v1.get()].connection,
         v2, v0,
         vNew, v1vNewEdge,
         true
      );
      assert(ok);

      ok = m_connections->insertExternal(
         m_staticVertices[v2.get()].connection,
         v0, v1,
         vNew, v2vNewEdge,
         true
      );
      assert(ok);

      removeTriangle(helper->v0v1v2TriangleToBeDeleted);
      createTriangle(v0, vNew, v2);
      createTriangle(v0, v1, vNew);
      createTriangle(v1, v2, vNew);

      m_connections->stopMemoryModificationLogging();

      result = Tetrahedron(v0.get(), v1.get(), v2.get(), vNew.get());
   }

   return result;
}


void Mesh::resizeEdge(GLint edge, dt::Float equilibriumLength)
{
   assert(static_cast<size_t>(edge) < m_edgeCount);
   if (equilibriumLength >= 0.0f)
   {
      m_edges[edge].equilibriumLength = equilibriumLength;
      m_edgeMods.insertArrayElement(edge, sizeof(Edge));
   }
}


void Mesh::setVertexPos(
   const dt::VertexId & v,
   dt::Float x,
   dt::Float y,
   dt::Float z
)
{
   assert(v.get() < m_vertexCount);
   DynamicVertex & dv = m_dynamicVertices[v.get()];
   dv.x = x;
   dv.y = y;
   dv.z = z;
   m_dynamicVertexMods.insertMemoryBlock(
      v.get() * sizeof(DynamicVertex) + offsetof(DynamicVertex, x),
      sizeof(dt::Float) * 3
   );
   invalidateDimensions();
}


void Mesh::setVertexPosX(const dt::VertexId & v, dt::Float x)
{
   assert(v.get() < m_vertexCount);
   m_dynamicVertices[v.get()].x = x;
   m_dynamicVertexMods.insertMemoryBlock(
      v.get() * sizeof(DynamicVertex) + offsetof(DynamicVertex, x),
      sizeof(dt::Float)
   );
   invalidateDimensions();
}


void Mesh::setVertexPosY(const dt::VertexId & v, dt::Float y)
{
   assert(v.get() < m_vertexCount);
   m_dynamicVertices[v.get()].y = y;
   m_dynamicVertexMods.insertMemoryBlock(
      v.get() * sizeof(DynamicVertex) + offsetof(DynamicVertex, y),
      sizeof(dt::Float)
   );
   invalidateDimensions();
}


void Mesh::setVertexPosZ(const dt::VertexId & v, dt::Float z)
{
   assert(v.get() < m_vertexCount);
   m_dynamicVertices[v.get()].z = z;
   m_dynamicVertexMods.insertMemoryBlock(
      v.get() * sizeof(DynamicVertex) + offsetof(DynamicVertex, z),
      sizeof(dt::Float)
   );
   invalidateDimensions();
}


void Mesh::setVertexMass(const dt::VertexId & v, dt::Float mass)
{
   assert(v.get() < m_vertexCount);
   m_dynamicVertices[v.get()].mass = mass;
   m_dynamicVertexMods.insertMemoryBlock(
      v.get() * sizeof(DynamicVertex) + offsetof(DynamicVertex, mass),
      sizeof(dt::Float)
   );
}


void Mesh::selectVertex(
   const boost::optional<dt::VertexId> & vertex,
   dt::SelectionMode selectionMode
)
{
   if (vertex && vertex->get() < m_vertexCount)
   {
      const dt::VertexId & v = *vertex;
      SelectionHelper helper;
      switch (selectionMode)
      {
         case dt::SM_Vertex:
            helper = helpVertexSelection(m_selection, v);
            break;
         case dt::SM_Edge:
            helper = helpEdgeSelection(*this, m_selection, v);
            break;
         case dt::SM_Triangle:
            helper = helpTriangleSelection(*this, m_selection, v);
            break;
         case dt::SM_Tetrahedron:
            helper = helpTetrahedronSelection(*this, m_selection, v);
            break;
      };

      switch (helper.action)
      {
         case SelectionHelper::A_DoNothing:
            break;
         case SelectionHelper::A_Deselect:
            assert(helper.vIndex);
            assert(isVertexSelected(v));
            m_selection.erase(m_selection.begin() + *helper.vIndex);
            setVertexSelected(v, false);
            break;
         case SelectionHelper::A_Select:
            assert(!helper.vIndex);
            assert(!isVertexSelected(v));
            m_selection.push_back(v);
            setVertexSelected(v, true);
            break;
         case SelectionHelper::A_ClearAndSelect:
            clearSelection();
            m_selection.resize(1, v);
            setVertexSelected(v, true);
            break;
      }
   }
   else
   {
      clearSelection();
   }

   switch (m_selection.size())
   {
      case 1:
      {
         std::cout << "Vertex " << m_selection[0].get() << " is selected" <<
            std::endl;
         break;
      }
      case 2:
      {
         size_t edge = *selectedEdge();
         std::cout << "Edge " << edge << " (" << m_selection[0].get() << ", " <<
            m_selection[1].get() << ") is selected" << std::endl;
         break;
      }
      default:
         std::cout << "Nothing is selected" << std::endl;
   }
}


const DynamicVertex * Mesh::dynamicVertices() const
{
   return m_dynamicVertices;
}


void Mesh::clearStructureModifications()
{
   m_structureMods.clear();
}


std::vector<Tetrahedron> Mesh::adjacentTetrahedrons(
   const Tetrahedron & t,
   dt::TetrahedronFace face
) const
{
   dt::VertexId v0(t.a);
   dt::VertexId v1(t.b);
   dt::VertexId v2(t.c);
   dt::VertexId excluded(t.d);
   if (face == dt::TF_ACD)
   {
      v0 = dt::VertexId(t.a);
      v1 = dt::VertexId(t.c);
      v2 = dt::VertexId(t.d);
      excluded = dt::VertexId(t.b);
   }
   else if (face == dt::TF_ADB)
   {
      v0 = dt::VertexId(t.a);
      v1 = dt::VertexId(t.d);
      v2 = dt::VertexId(t.b);
      excluded = dt::VertexId(t.c);
   }
   else if (face == dt::TF_BCD)
   {
      v0 = dt::VertexId(t.b);
      v1 = dt::VertexId(t.c);
      v2 = dt::VertexId(t.d);
      excluded = dt::VertexId(t.a);
   }

   std::vector<Tetrahedron> result;
   Connections::const_iterator it = m_connections->begin(
      m_staticVertices[v0.get()].connection
   );
   for (; it.isValid(); ++ it)
   {
      dt::VertexId vertex(it.get(Connections::VT_VERTEX));
      if (vertex != v1 && vertex != v2 && vertex != excluded)
      {
         bool foundByV1 = false;
         bool foundByV2 = false;
         Connections::const_iterator nestedIt = m_connections->begin(
            m_staticVertices[vertex.get()].connection
         );
         for (; nestedIt.isValid(); ++ nestedIt)
         {
            dt::VertexId nestedVertex(nestedIt.get(Connections::VT_VERTEX));
            if (nestedVertex == v1)
            {
               assert(!foundByV1);
               foundByV1 = true;
            }
            else if (nestedVertex == v2)
            {
               assert(!foundByV2);
               foundByV2 = true;
            }

            if (foundByV1 && foundByV2)
            {
               result.push_back(
                  Tetrahedron(v0.get(), v1.get(), v2.get(), vertex.get())
               );
               break;
            }
         }
      }
   }
   return result;
}


std::vector<Tetrahedron> Mesh::adjacentTetrahedrons(
   const Tetrahedron & t
) const
{
   std::vector<Tetrahedron> vec;
   vec.reserve(4);

   auto adjTtrs = std::move(adjacentTetrahedrons(t, dt::TF_ABC));
   if (!adjTtrs.empty())
   {
      vec.insert(vec.end(), adjTtrs.begin(), adjTtrs.end());
   }
   adjTtrs = std::move(adjacentTetrahedrons(t, dt::TF_ACD));
   if (!adjTtrs.empty())
   {
      vec.insert(vec.end(), adjTtrs.begin(), adjTtrs.end());
   }
   adjTtrs = std::move(adjacentTetrahedrons(t, dt::TF_ADB));
   if (!adjTtrs.empty())
   {
      vec.insert(vec.end(), adjTtrs.begin(), adjTtrs.end());
   }
   adjTtrs = std::move(adjacentTetrahedrons(t, dt::TF_BCD));
   if (!adjTtrs.empty())
   {
      vec.insert(vec.end(), adjTtrs.begin(), adjTtrs.end());
   }
   return vec;
}


boost::optional<dt::VertexId> Mesh::selectedVertex() const
{
   return ((m_selection.size() == 1) ?
      m_selection[0] :
      boost::optional<dt::VertexId>()
   );
}


boost::optional<std::pair<size_t, size_t> > Mesh::selectedEdgeVertices(
   ) const
{
   return ((m_selection.size() == 2) ?
      std::pair<size_t, size_t>(m_selection[0].get(), m_selection[1].get()) :
      boost::optional<std::pair<size_t, size_t> >()
   );
}


boost::optional<size_t> Mesh::selectedEdge() const
{
   if (m_selection.size() == 2)
   {
      GLint edge = m_connections->findEdge(
         m_staticVertices[m_selection[0].get()].connection,
         m_selection[1]
      );
      assert(edge == m_connections->findEdge(
         m_staticVertices[m_selection[1].get()].connection,
         m_selection[0]
      ));
      if (edge >= 0) {
         assert(static_cast<size_t>(edge) < m_edgeCount);
         return edge;
      }
   }
   return boost::optional<size_t>();
}


boost::optional<dt::TriangleId> Mesh::selectedTriangle() const
{
   if (m_selection.size() == 3)
   {
      return m_trianglesMap.findAny(Triangle(
         m_selection[0].get(),
         m_selection[1].get(),
         m_selection[2].get()
      ));
   }
   return boost::none;
}


boost::optional<Tetrahedron> Mesh::selectedTetrahedron() const
{
   if (m_selection.size() == 4)
   {
      return Tetrahedron(
         m_selection[0].get(),
         m_selection[1].get(),
         m_selection[2].get(),
         m_selection[3].get()
      );
   }
   return boost::optional<Tetrahedron>();
}


dt::Float Mesh::tetrahedronVolume(const Tetrahedron & t) const
{
   // Call a virtual function to ensure the data is synchronized;
   const DynamicVertex * dv = dynamicVertices();

   return utils3d::getTetrahedronVolume(
      dv[t.a].point(),
      dv[t.b].point(),
      dv[t.c].point(),
      dv[t.d].point()
   );
   // TODO: Use cache. See center() and dimensions();
}


dt::Pointf3 Mesh::center() const
{
   if (!m_center)
   {
      recalculateDimensions();
   }
   return *m_center;
}


dt::Vectorf3 Mesh::dimensions() const
{
   if (!m_dimensions)
   {
      recalculateDimensions();
   }
   return *m_dimensions;
}


void Mesh::clearMemoryModifications()
{
   m_dynamicVertexMods.clear();
   m_staticVertexMods.clear();
   m_triangleMods.clear();
   m_edgeMods.clear();
   m_connections->memoryModification()->clear();
}


void Mesh::invalidateDimensions()
{
   m_center = boost::none;
   m_dimensions = boost::none;
}


void Mesh::clearSelection()
{
   for (size_t i = 0, count = m_selection.size(); i < count; ++i)
   {
      assert(isVertexSelected(m_selection[i]));
      setVertexSelected(m_selection[i], false);
   }
   m_selection.clear();
}


dt::Float Mesh::newEdgeLength(
   const dt::VertexId & v1,
   const dt::VertexId & v2
) const
{
   const DynamicVertex & vertex1 = m_dynamicVertices[v1.get()];
   const DynamicVertex & vertex2 = m_dynamicVertices[v2.get()];
   dt::Float dx = (vertex1.x - vertex2.x);
   dt::Float dy = (vertex1.y - vertex2.y);
   dt::Float dz = (vertex1.z - vertex2.z);
   return sqrt(dx * dx + dy * dy + dz * dz);
}


void Mesh::helpEdgeBudding(
   const BuddingEdgeHelper & edge,
   const dt::VertexId & va,
   const dt::VertexId & vb
)
{
   assert(
      (edge.v0 == va && edge.v1 == vb) ||
      (edge.v0 == vb && edge.v1 == va)
   );

   if (edge.convertToInternal)
   {
      _convertToInternal(
         m_staticVertices,
         m_connections,
         m_staticVertexMods,
         va,
         vb,
         edge.edge
      );

      _convertToInternal(
         m_staticVertices,
         m_connections,
         m_staticVertexMods,
         vb,
         va,
         edge.edge
      );
   }
}


void Mesh::helpEdgeBudding(
   const boost::optional<BuddingEdgeHelper> & currentEdge,
   const boost::optional<BuddingEdgeHelper> & otherEdge0,
   const boost::optional<BuddingEdgeHelper> & otherEdge1,
   const dt::VertexId & va,
   const dt::VertexId & vNew,
   const dt::VertexId & vb,
   const dt::VertexId & vc
)
{
   if (currentEdge)
   {
      helpEdgeBudding(*currentEdge, va, vNew);
   }
   else
   {
      assert(otherEdge0);
      assert(otherEdge1);
      dt::EdgeId newEdge = createEdge(va, vNew);

      bool ok = m_connections->insertExternal(
         m_staticVertices[va.get()].connection,
         vb, vc,
         vNew, newEdge,
         true
      );
      assert(ok);

      ok = m_connections->insertExternal(
         m_staticVertices[vNew.get()].connection,
         vc, vb,
         va, newEdge,
         true
      );
      assert(ok);
   }
}


bool Mesh::ensureNoCollisionsForBud(
   const dt::TriangleId & budBase,
   const dt::Pointf3 & budTop
) const
{
   const DynamicVertex * dv = dynamicVertices();
   const size_t budBaseIndex = budBase.get();
   assert(budBaseIndex < m_triangleCount);

   const Triangle & budBaseTr = m_triangles[budBaseIndex];
   const dt::Pointf3 budA = dv[budBaseTr.a].point();
   const dt::Pointf3 budB = dv[budBaseTr.b].point();
   const dt::Pointf3 budC = dv[budBaseTr.c].point();

   for (size_t i = 0; i < m_triangleCount; ++i)
   {
      if (i != budBaseIndex)
      {
         const Triangle & t = m_triangles[i];
         const dt::Pointf3 a = dv[t.a].point();
         const dt::Pointf3 b = dv[t.b].point();
         const dt::Pointf3 c = dv[t.c].point();

         if (!t.contains(budBaseTr.a) && !t.contains(budBaseTr.b) &&
            utils3d::doTrianglesIntersect(budA, budB, budTop, a, b, c))
         {
            return false;
         }

         if (!t.contains(budBaseTr.a) && !t.contains(budBaseTr.c) &&
            utils3d::doTrianglesIntersect(budA, budC, budTop, a, b, c))
         {
            return false;
         }

         if (!t.contains(budBaseTr.b) && !t.contains(budBaseTr.c) &&
            utils3d::doTrianglesIntersect(budB, budC, budTop, a, b, c))
         {
            return false;
         }
      }
   }
   return true;
}


dt::VertexId Mesh::createBudTopVertex(
   const dt::VertexId & v0,
   const dt::VertexId & v1,
   const dt::VertexId & v2,
   const dt::Pointf3 & budTop
)
{
   assert(m_vertexCount + 1 <= MBS_MAX_VERTEX_COUNT);
   size_t vNew = m_vertexCount ++;
   m_dynamicVertices[vNew] = DynamicVertex(budTop.x, budTop.y, budTop.z);
   m_staticVertices[vNew] = StaticVertex();
   m_dynamicVertexMods.insertArrayElement(vNew, sizeof(DynamicVertex));
   m_staticVertexMods.insertArrayElement(vNew, sizeof(StaticVertex));
   invalidateDimensions();
   return dt::VertexId(vNew);
}


dt::EdgeId Mesh::createEdge(const dt::VertexId & v0, const dt::VertexId & v1)
{
   assert(m_edgeCount + 1 <= MBS_MAX_EDGE_COUNT);
   size_t newEdge = m_edgeCount ++;
   m_edges[newEdge] = Edge(newEdgeLength(v0, v1));
   m_edgeMods.insertArrayElement(newEdge, sizeof(Edge));
   return dt::EdgeId(newEdge);
}


size_t Mesh::createTriangle(
   const dt::VertexId & v0,
   const dt::VertexId & v1,
   const dt::VertexId & v2
)
{
   assert(m_triangleCount + 1 <= MBS_MAX_TRIANGLE_COUNT);
   size_t tNew = m_triangleCount ++;
   m_triangles[tNew] = Triangle(v0.get(), v1.get(), v2.get());
   m_triangleMods.insertArrayElement(tNew, sizeof(Triangle));
   m_trianglesMap.insert(m_triangles[tNew], dt::TriangleId(tNew));
   return tNew;
}


void Mesh::removeAllConnections(const dt::VertexId & v)
{
   GLint firstColor = m_staticVertices[v.get()].connection;
   do
   {
      ColorWrappedLists::iterator it = m_connections->begin(firstColor);
      bool ok = removeConnection(
         dt::VertexId(it.get(Connections::VT_VERTEX)),
         v,
         true
      );
      assert(ok);
   }
   while ((firstColor = m_connections->removeFirst(firstColor)) >= 0);

   m_staticVertices[v.get()].connection = firstColor;
   m_staticVertexMods.insertMemoryBlock(
      v.get() * sizeof(StaticVertex) + offsetof(StaticVertex, connection),
      sizeof(GLint)
   );
}


bool Mesh::removeConnection(
   const dt::VertexId & v,
   const dt::VertexId & connectedV,
   bool removeEdge
)
{
   GLint firstColor = m_staticVertices[v.get()].connection;
   ColorWrappedLists::iterator it = m_connections->begin(firstColor);

   if (it.isValid() &&
      dt::VertexId(it.get(Connections::VT_VERTEX)) == connectedV)
   {
      if (removeEdge)
      {
         this->removeEdge(it.get(Connections::VT_EDGE));
      }
      m_staticVertices[v.get()].connection =
         m_connections->removeFirst(firstColor);
      m_staticVertexMods.insertMemoryBlock(
         v.get() * sizeof(StaticVertex) + offsetof(StaticVertex, connection),
         sizeof(GLint)
      );

      return true;
   }

   ColorWrappedLists::iterator prevIt;
   for (; it.isValid(); ++ it)
   {
      if (prevIt.isValid() &&
         dt::VertexId(it.get(Connections::VT_VERTEX)) == connectedV)
      {
         if (removeEdge)
         {
            this->removeEdge(it.get(Connections::VT_EDGE));
         }
         m_connections->removeAfter(prevIt);
         return true;
      }
      prevIt = it;
   }

   return false;
}


void Mesh::removeVertex(const dt::VertexId & v)
{
   assert(m_vertexCount > 0);

   size_t vLast = m_vertexCount - 1;
   assert(v.get() <= vLast);

   removeAllConnections(v);

   if (v.get() != vLast)
   {
      // Move last vertex to v position;
      moveLastVertex(v);
   } else {
      // Clear last vertex;
      m_dynamicVertices[vLast] = DynamicVertex();
      m_dynamicVertexMods.insertArrayElement(vLast, sizeof(DynamicVertex));

      m_staticVertices[vLast] = StaticVertex();
      m_staticVertexMods.insertArrayElement(vLast, sizeof(StaticVertex));

      -- m_vertexCount;

      invalidateDimensions();
   }
}


void Mesh::removeEdge(GLint e)
{
   assert(m_edgeCount > 0);

   GLint eLast = m_edgeCount - 1;
   assert(e <= eLast);

   if (e != eLast)
   {
      // Move last edge to e position;
      moveLastEdge(e);
   } else {
      // Clear last edge;
      m_edges[eLast] = Edge();
      m_edgeMods.insertArrayElement(eLast, sizeof(Edge));

      -- m_edgeCount;
   }
}


void Mesh::removeTriangle(const dt::TriangleId & t)
{
   assert(m_triangleCount > 0);

   size_t tLast = m_triangleCount - 1;
   assert(t.get() <= tLast);

   if (t.get() != tLast)
   {
      // Move last triangle to t position;
      moveLastTriangle(t);
   } else {
      // Clear last triangle;
      bool ok = m_trianglesMap.remove(m_triangles[tLast]);
      assert(ok);
      m_triangles[tLast] = Triangle();
      m_triangleMods.insertArrayElement(tLast, sizeof(Triangle));

      -- m_triangleCount;
   }
}


void Mesh::moveLastVertex(const dt::VertexId & newIndex)
{
   size_t vLast = m_vertexCount - 1;
   assert(vLast != newIndex.get());

   // Replace vLast by newIndex in triangles;
   TrianglesMap replacedTriangles =
      std::move(m_trianglesMap.replaceVertexIndex(vLast, newIndex.get()));
   TrianglesMap::const_iterator mapIt = replacedTriangles.begin();
   TrianglesMap::const_iterator mapIte = replacedTriangles.end();
   for (; mapIt != mapIte; ++mapIt)
   {
      m_triangles[mapIt->second.get()] = mapIt->first;
      m_triangleMods.insertArrayElement(mapIt->second.get(), sizeof(Triangle));
   }

   // Replace vLast by newIndex in connections;
   GLint firstColor = m_staticVertices[vLast].connection;
   ColorWrappedLists::const_iterator it = m_connections->begin(firstColor);
   for (; it.isValid(); ++ it)
   {
      bool ok = m_connections->replace(
         m_staticVertices[it.get(Connections::VT_VERTEX)].connection,
         dt::VertexId(vLast),
         newIndex
      );
      assert(ok);
   }

   m_dynamicVertices[newIndex.get()] = m_dynamicVertices[vLast];
   m_dynamicVertices[vLast] = DynamicVertex();
   m_dynamicVertexMods.insertArrayElement(newIndex.get(), sizeof(DynamicVertex));
   m_dynamicVertexMods.insertArrayElement(vLast, sizeof(DynamicVertex));

   m_staticVertices[newIndex.get()] = m_staticVertices[vLast];
   m_staticVertices[vLast] = StaticVertex();
   m_staticVertexMods.insertArrayElement(newIndex.get(), sizeof(StaticVertex));
   m_staticVertexMods.insertArrayElement(vLast, sizeof(StaticVertex));

   -- m_vertexCount;

   invalidateDimensions();
}


void Mesh::moveLastEdge(GLint newIndex)
{
   GLint eLast = m_edgeCount - 1;
   assert(eLast != newIndex);

   for (size_t v = 0; v < m_vertexCount; ++ v)
   {
      m_connections->replaceEdge(
         m_staticVertices[v].connection,
         eLast, newIndex
      );
   }

   m_edges[newIndex] = m_edges[eLast];
   m_edges[eLast] = Edge();
   m_edgeMods.insertArrayElement(newIndex, sizeof(Edge));
   m_edgeMods.insertArrayElement(eLast, sizeof(Edge));

   -- m_edgeCount;
}


void Mesh::moveLastTriangle(const dt::TriangleId & newId)
{
   size_t tLast = m_triangleCount - 1;
   assert(tLast != newId.get());

   bool ok = m_trianglesMap.remove(m_triangles[newId.get()]);
   assert(ok);
   assert(m_trianglesMap.findAny(m_triangles[tLast]));
   m_trianglesMap.replace(m_triangles[tLast], newId);

   m_triangles[newId.get()] = m_triangles[tLast];
   m_triangles[tLast] = Triangle();
   m_triangleMods.insertArrayElement(newId.get(), sizeof(Triangle));
   m_triangleMods.insertArrayElement(tLast, sizeof(Triangle));

   -- m_triangleCount;
}


bool Mesh::isVertexSelected(const dt::VertexId & v) const
{
   return (m_dynamicVertices[v.get()].selected >= 0.5f);
}


void Mesh::setVertexSelected(const dt::VertexId & v, bool s)
{
   m_dynamicVertices[v.get()].selected = (s ? 1.0f : 0.0f);
   m_dynamicVertexMods.insertMemoryBlock(
      v.get() * sizeof(DynamicVertex) + offsetof(DynamicVertex, selected),
      sizeof(dt::Float)
   );
}


void Mesh::recalculateDimensions() const
{
   if (m_vertexCount)
   {
      const DynamicVertex * dvs = dynamicVertices(); // Pull actual vertex data;
      dt::Float xmin = dvs[0].x, xmax = dvs[0].x;
      dt::Float ymin = dvs[0].y, ymax = dvs[0].y;
      dt::Float zmin = dvs[0].z, zmax = dvs[0].z;
      for (size_t i = 1; i < m_vertexCount; ++i)
      {
         const DynamicVertex & dv = dvs[i];
         if (xmin > dv.x) {xmin = dv.x;}
         if (xmax < dv.x) {xmax = dv.x;}
         if (ymin > dv.y) {ymin = dv.y;}
         if (ymax < dv.y) {ymax = dv.y;}
         if (zmin > dv.z) {zmin = dv.z;}
         if (zmax < dv.z) {zmax = dv.z;}
      }
      m_center = dt::Pointf3(
         0.5f * (xmin + xmax),
         0.5f * (ymin + ymax),
         0.5f * (zmin + zmax)
      );
      m_dimensions = dt::Vectorf3(xmax - xmin, ymax - ymin, zmax - zmin);
   }
   else
   {
      m_center = dt::Pointf3(0.0f, 0.0f, 0.0f);
      m_dimensions = dt::Vectorf3(0.0f, 0.0f, 0.0f);
   }
}


std::ostream & operator<<(std::ostream & os, const Mesh & mesh)
{
   bool isFirst = true;
   os << "{";
   if (mesh.vertexCount()) {
      const DynamicVertex * dynamicVertices = mesh.dynamicVertices();
      const StaticVertex * staticVertices = mesh.staticVertices();

      os << "DV: [0]" << dynamicVertices[0];
      for (size_t i = 1; i < mesh.vertexCount(); ++ i) {
         os << ", [" << i << "]" << dynamicVertices[i];
      }
      os << " SV: [0]" << staticVertices[0];
      for (size_t i = 1; i < mesh.vertexCount(); ++ i) {
         os << ", [" << i << "]" << staticVertices[i];
      }
      os << " CN: ";
      for (size_t i = 0; i < mesh.vertexCount(); ++ i) {
         if (i) os << ", ";
         os << "[" << staticVertices[i].connection << "]{";
         Connections::const_iterator it = mesh.connections().begin(
            staticVertices[i].connection
         );
         for (int j = 0; it.isValid(); ++ it, ++ j)
         {
            if (j) os << ", ";
            os << "{v: " << it.get(Connections::VT_VERTEX) <<
               ", e: " << it.get(Connections::VT_EDGE) <<
               ", i: " << it.get(Connections::VT_INTERNAL) <<
               "}";
         }
         os << "}";
      }
      isFirst = false;
   }
   if (mesh.triangleCount()) {
      const Triangle * triangles = mesh.triangles();

      if (!isFirst) os << " ";
      os << "TR: [0]" << triangles[0];
      for (size_t i = 1; i < mesh.triangleCount(); ++ i) {
         os << ", [" << i << "]" << triangles[i];
      }
      isFirst = false;
   }
   if (!isFirst) os << " ";
   os << "TM: " << mesh.trianglesMap() << "}";
   return os;
}


}
