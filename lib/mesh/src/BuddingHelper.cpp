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


#include "utils3d/geometry.hpp"


#include "BuddingHelper.hpp"
#include "Connections.hpp"
#include "Mesh.hpp"
#include "TrianglesMap.hpp"
#include "Vertex.hpp"


namespace {


struct _AppropriateVertex
{
   explicit _AppropriateVertex(
      const dt::VertexId & vNew,
      const dt::Pointf3 & vNewCoords,
      const dt::VertexId & v0,
      const dt::VertexId & v1,
      size_t v0vNewEdge,
      size_t v1vNewEdge
   );

   bool operator<(const _AppropriateVertex & other) const;

   dt::VertexId vNew;
   dt::Pointf3 vNewCoords;
   mesh::BuddingEdgeHelper v0vNewEdge;
   mesh::BuddingEdgeHelper v1vNewEdge;
   boost::optional<mesh::BuddingEdgeHelper> v2vNewEdge;
   dt::Float distanceToBudTop;
   bool backup;
};


_AppropriateVertex::_AppropriateVertex(
   const dt::VertexId & vNew,
   const dt::Pointf3 & vNewCoords,
   const dt::VertexId & v0,
   const dt::VertexId & v1,
   size_t v0vNewEdge,
   size_t v1vNewEdge
) : vNew(vNew),
   vNewCoords(vNewCoords),
   v0vNewEdge(v0, vNew, v0vNewEdge),
   v1vNewEdge(v1, vNew, v1vNewEdge),
   distanceToBudTop(0.0f),
   backup(false)
{}


bool _AppropriateVertex::operator<(const _AppropriateVertex & other) const
{
   if (backup == other.backup)
   {
      return distanceToBudTop < other.distanceToBudTop;
   }
   else if (!backup && other.backup)
   {
      return true;
   }
   return false;
}


boost::optional<_AppropriateVertex> _appropriateVertex(
   const mesh::Mesh &mesh,
   const dt::VertexId & v0,
   const dt::VertexId & v1,
   const dt::VertexId & v2,
   const dt::VertexId & v3,
   const dt::Pointf3 &budTop
)
{
   using namespace mesh;

   const DynamicVertex * dynamicVertices = mesh.dynamicVertices();
   const StaticVertex * staticVertices = mesh.staticVertices();
   const Connections & connections = mesh.connections();

   Connections::const_iterator it = connections.getPrevExternalVertex(
      staticVertices[v0.get()].connection,
      v1
   );
   if (it.isValid())
   {
      assert(!it.get(Connections::VT_INTERNAL));
      dt::VertexId vNew(it.get(Connections::VT_VERTEX));
      if (vNew != v3)
      {
         assert(vNew != v2);
         const dt::Pointf3 a = dynamicVertices[v0.get()].point();
         const dt::Pointf3 b = dynamicVertices[v1.get()].point();
         const dt::Pointf3 c = dynamicVertices[v2.get()].point();
         const dt::Pointf3 n = dynamicVertices[vNew.get()].point();

         bool above = false;
         bool backup = false;
         bool vNewIsAppropriate =
            utils3d::doesPointBelongToRightTriangularPrismWithInfinitHeight(
               a, b, c, n, &above
            );
         if (!vNewIsAppropriate && above)
         {
            if (!utils3d::doesPointAbovePlane(n, b, a, budTop))
            {
               vNewIsAppropriate = true;
               backup = true;
            }
         }

         if (vNewIsAppropriate)
         {
            Connections::const_iterator it1 = connections.findFirst(
               staticVertices[vNew.get()].connection,
               v1.get(),
               Connections::VT_VERTEX
            );
            assert(it1.isValid());
            assert(!it1.get(Connections::VT_INTERNAL));

            GLint v0vNewEdge = it.get(Connections::VT_EDGE);
            GLint v1vNewEdge = it1.get(Connections::VT_EDGE);
            assert(v0vNewEdge >= 0);
            assert(v1vNewEdge >= 0);

            _AppropriateVertex av(vNew, n, v0, v1, v0vNewEdge, v1vNewEdge);

            // Try to find the third external edge;
            Connections::const_iterator it2 = connections.findFirst(
               staticVertices[vNew.get()].connection,
               v2.get(),
               Connections::VT_VERTEX
            );
            if (it2.isValid())
            {
               if (it2.get(Connections::VT_INTERNAL))
               {
                  return boost::none;
               }
               else
               {
                  // One more test of vNew;
                  if (!connections.areExternalVerticesNextToEachOther(
                     staticVertices[vNew.get()].connection, v0, v2) ||
                     !connections.areExternalVerticesNextToEachOther(
                     staticVertices[vNew.get()].connection, v1, v2)
                  )
                  {
                     return boost::none;
                  }

                  av.v2vNewEdge = BuddingEdgeHelper(
                     v2, vNew, it2.get(Connections::VT_EDGE)
                  );
               }
            }
            av.distanceToBudTop = utils3d::distance(budTop, av.vNewCoords);
            av.backup = backup;
            return av;
         }
      }
   }
   return boost::none;
}


bool _lessForOptionalAppropriateVertices(
   boost::optional<_AppropriateVertex> *a,
   boost::optional<_AppropriateVertex> *b
)
{
   if (a->is_initialized() && b->is_initialized())
   {
      return *(*a) < *(*b);
   }
   else if (a->is_initialized() && !b->is_initialized())
   {
      return true;
   }
   return false;
}


void _selectMostAppropriate(
   boost::optional<_AppropriateVertex> * a1,
   boost::optional<_AppropriateVertex> * a2,
   boost::optional<_AppropriateVertex> * a3
)
{
   boost::optional<_AppropriateVertex> *aps[3] = {a1, a2, a3};
   std::sort(aps, aps + 3, _lessForOptionalAppropriateVertices);
   if (aps[1]->is_initialized())
   {
      *aps[1] = boost::optional<_AppropriateVertex>();
   }
   if (aps[2]->is_initialized())
   {
      *aps[2] = boost::optional<_AppropriateVertex>();
   }
}


boost::optional<mesh::BuddingHelper> _helpInitialBudding(
   const mesh::Mesh & mesh,
   dt::VertexId & v0,
   dt::VertexId & v1,
   dt::VertexId & v2,
   dt::Float budTopRadius,
   dt::Float budTopPolarAngle,
   dt::Float budTopAzimuthalAngle
)
{
   using namespace mesh;

   boost::optional<dt::TriangleId> v0v1v2Triangle = mesh.trianglesMap().findAny(
      Triangle(v0.get(), v1.get(), v2.get()));
   if (v0v1v2Triangle)
   {
      const DynamicVertex * dynamicVertices = mesh.dynamicVertices();
      const StaticVertex * staticVertices = mesh.staticVertices();
      const Connections & connections = mesh.connections();

      // Set proper vertex order;
      const Triangle & triangle = mesh.triangles()[v0v1v2Triangle->get()];
      v0 = dt::VertexId(triangle.a);
      v1 = dt::VertexId(triangle.b);
      v2 = dt::VertexId(triangle.c);

      GLint edge = connections.findEdge(
         staticVertices[v0.get()].connection,
         v1
      );
      assert(edge >= 0);
      BuddingEdgeHelper v0v1Edge(v0, v1, edge);

      edge = connections.findEdge(staticVertices[v0.get()].connection, v2);
      assert(edge >= 0);
      BuddingEdgeHelper v0v2Edge(v0, v2, edge);

      edge = connections.findEdge(staticVertices[v1.get()].connection, v2);
      assert(edge >= 0);
      BuddingEdgeHelper v1v2Edge(v1, v2, edge);

      const dt::Pointf3 top = utils3d::getTetrahedronTopPoint(
         dynamicVertices[v0.get()].point(),
         dynamicVertices[v1.get()].point(),
         dynamicVertices[v2.get()].point(),
         budTopRadius,
         budTopPolarAngle,
         budTopAzimuthalAngle
      );

      return BuddingHelper(top, v0v1Edge, v0v2Edge, v1v2Edge, *v0v1v2Triangle);
   }
   return boost::none;
}


bool _isConvertionToInternalNeeded(
   const mesh::Mesh & mesh,
   const mesh::BuddingEdgeHelper & edgeHelper,
   const boost::optional<dt::TriangleId> & exceptTriangle0,
   const boost::optional<dt::TriangleId> & exceptTriangle1
)
{
   using namespace mesh;

   const StaticVertex * staticVertices = mesh.staticVertices();
   const Connections & connections = mesh.connections();
   const TrianglesMap & trianglesMap = mesh.trianglesMap();

   Connections::const_iterator it = connections.begin(
      staticVertices[edgeHelper.v0.get()].connection
   );
   for (; it.isValid() && !it.get(Connections::VT_INTERNAL); ++ it)
   {
      dt::VertexId vertex(it.get(Connections::VT_VERTEX));
      if (vertex != edgeHelper.v1)
      {
         Connections::const_iterator nestedIt = connections.begin(
            staticVertices[vertex.get()].connection
         );
         for (; nestedIt.isValid() && !it.get(Connections::VT_INTERNAL);
            ++ nestedIt)
         {
            if (dt::VertexId(nestedIt.get(Connections::VT_VERTEX)) ==
               edgeHelper.v1)
            {
               const auto t = trianglesMap.findAny(Triangle(
                  edgeHelper.v0.get(), vertex.get(), edgeHelper.v1.get())
               );
               if (t && (t != exceptTriangle0) && (t != exceptTriangle1))
               {
                  return false;
               }
               break;
            }
         }
      }
      else
      {
         assert(static_cast<size_t>(it.get(Connections::VT_EDGE)) ==
            edgeHelper.edge);
      }
   }
   return true;
}


} // anonymous namespace;


namespace mesh {


/***************************************************************************
 *   BuddingEdgeHelper struct implementation                               *
 ***************************************************************************/


BuddingEdgeHelper::BuddingEdgeHelper(
   const dt::VertexId & v0,
   const dt::VertexId & v1,
   size_t edge,
   bool convertToInternal
) : v0(v0), v1(v1), edge(edge), convertToInternal(convertToInternal)
{
}


bool BuddingEdgeHelper::operator==(const BuddingEdgeHelper & other) const
{
   return v0 == other.v0 &&
      v1 == other.v1 &&
      edge == other.edge &&
      convertToInternal == other.convertToInternal;
}


/***************************************************************************
 *   BuddingTriangleHelper struct implementation                           *
 ***************************************************************************/


BuddingTriangleHelper::BuddingTriangleHelper(
   const dt::TriangleId & triangleToBeDeleted
) : variant(triangleToBeDeleted)
{
}


BuddingTriangleHelper::BuddingTriangleHelper(
   const dt::VertexId & a,
   const dt::VertexId & b,
   const dt::VertexId & c
) : variant(Triangle(a.get(), b.get(), c.get()))
{
}


bool BuddingTriangleHelper::operator<(
   const BuddingTriangleHelper & other
) const
{
   const dt::TriangleId * triangleToBeDeleted =
      boost::get<dt::TriangleId>(&this->variant);
   const dt::TriangleId * otherTriangleToBeDeleted =
      boost::get<dt::TriangleId>(&other.variant);
   if (triangleToBeDeleted && otherTriangleToBeDeleted)
   {
      return (*triangleToBeDeleted < *otherTriangleToBeDeleted);
   }
   else if (!triangleToBeDeleted && !otherTriangleToBeDeleted)
   {
      const Triangle * t = boost::get<Triangle>(&this->variant);
      const Triangle * tOther = boost::get<Triangle>(&other.variant);
      assert(t && tOther);
      return ((*t) < (*tOther));
   }
   else if (!triangleToBeDeleted && otherTriangleToBeDeleted)
   {
      return true;
   }
   return false;
}


bool BuddingTriangleHelper::operator==(
   const BuddingTriangleHelper & other
) const
{
   return variant == other.variant;
}


/***************************************************************************
 *   BuddingHelper struct implementation                                   *
 ***************************************************************************/


BuddingHelper::BuddingHelper(
   const dt::Pointf3 & budTop,
   const BuddingEdgeHelper & v0v1Edge,
   const BuddingEdgeHelper & v0v2Edge,
   const BuddingEdgeHelper & v1v2Edge,
   const dt::TriangleId & v0v1v2TriangleToBeDeleted
) : budTop(budTop),
   v0v1Edge(v0v1Edge),
   v0v2Edge(v0v2Edge),
   v1v2Edge(v1v2Edge),
   v0v1v2TriangleToBeDeleted(v0v1v2TriangleToBeDeleted)
{
}


size_t BuddingHelper::verticesToBeAdded() const
{
   return (vNew ? 0 : 1);
}


size_t BuddingHelper::edgesToBeAdded() const
{
   size_t result = 3;
   if (v0vNewEdge) {--result;}
   if (v1vNewEdge) {--result;}
   if (v2vNewEdge) {--result;}
   return result;
}


size_t BuddingHelper::trianglesToBeAdded() const
{
   int count = -1;
   count += (v0v1vNTriangleToBeDeleted ? -1 : 1);
   count += (v0vNv2TriangleToBeDeleted ? -1 : 1);
   count += (v1v2vNTriangleToBeDeleted ? -1 : 1);
   return (count > 0 ? count : 0);
}


std::ostream & operator<<(std::ostream & os, const BuddingHelper & t)
{
   /*os << "{" << t.v0 << ", " << t.v1 << ", " << t.v2 << ", " << t.v3;
   if (t.singular)
   {
      os << ", singular";
   }
   os << "}";*/
   return os;
}


boost::optional<BuddingHelper> helpBudding(
   const Mesh &mesh,
   const dt::VertexId & vertex0,
   const dt::VertexId & vertex1,
   const dt::VertexId & vertex2,
   const dt::VertexId & vertex3,
   dt::Float budTopRadius,
   dt::Float budTopPolarAngle,
   dt::Float budTopAzimuthalAngle
)
{
   dt::VertexId v0(vertex0);
   dt::VertexId v1(vertex1);
   dt::VertexId v2(vertex2);
   dt::VertexId v3(vertex3);

   assert(v0.get() < mesh.vertexCount());
   assert(v1.get() < mesh.vertexCount());
   assert(v2.get() < mesh.vertexCount());
   assert(v3.get() < mesh.vertexCount());

   boost::optional<BuddingHelper> helper = _helpInitialBudding(
      mesh, v0, v1, v2,
      budTopRadius, budTopPolarAngle, budTopAzimuthalAngle
   );
   if (helper)
   {
      enum {NONE = 0, V0V1VN, V0VNV2, V1V2VN} foundTriangle = NONE;

      boost::optional<_AppropriateVertex> av012 =
         _appropriateVertex(mesh, v0, v1, v2, v3, helper->budTop);
      boost::optional<_AppropriateVertex> av120 =
         _appropriateVertex(mesh, v1, v2, v0, v3, helper->budTop);
      boost::optional<_AppropriateVertex> av201 =
         _appropriateVertex(mesh, v2, v0, v1, v3, helper->budTop);

      _selectMostAppropriate(&av012, &av120, &av201);

      if (av012)
      {
         foundTriangle = V0V1VN;
         helper->vNew = av012->vNew;
         helper->budTop = av012->vNewCoords;
         helper->v0vNewEdge = av012->v0vNewEdge;
         helper->v1vNewEdge = av012->v1vNewEdge;
         helper->v2vNewEdge = av012->v2vNewEdge;
      }
      else if (av120)
      {
         foundTriangle = V1V2VN;
         helper->vNew = av120->vNew;
         helper->budTop = av120->vNewCoords;
         helper->v0vNewEdge = av120->v2vNewEdge;
         helper->v1vNewEdge = av120->v0vNewEdge;
         helper->v2vNewEdge = av120->v1vNewEdge;
      }
      else if (av201)
      {
         foundTriangle = V0VNV2;
         helper->vNew = av201->vNew;
         helper->budTop = av201->vNewCoords;
         helper->v0vNewEdge = av201->v1vNewEdge;
         helper->v1vNewEdge = av201->v2vNewEdge;
         helper->v2vNewEdge = av201->v0vNewEdge;
      }

      if (helper->vNew)
      {
         assert(
            *helper->vNew != vertex0 &&
            *helper->vNew != vertex1 &&
            *helper->vNew != vertex2 &&
            *helper->vNew != vertex3
         );

         const TrianglesMap & trianglesMap = mesh.trianglesMap();
         helper->v0v1vNTriangleToBeDeleted = trianglesMap.findAny(
            Triangle(v0.get(), v1.get(), helper->vNew->get())
         );
         helper->v0vNv2TriangleToBeDeleted = trianglesMap.findAny(
            Triangle(v0.get(), helper->vNew->get(), v2.get())
         );
         helper->v1v2vNTriangleToBeDeleted = trianglesMap.findAny(
            Triangle(v1.get(), v2.get(), helper->vNew->get())
         );
         assert(
            helper->v0v1vNTriangleToBeDeleted ||
            helper->v0vNv2TriangleToBeDeleted ||
            helper->v1v2vNTriangleToBeDeleted
         );
         assert(foundTriangle != V0V1VN || helper->v0v1vNTriangleToBeDeleted);
         assert(foundTriangle != V0VNV2 || helper->v0vNv2TriangleToBeDeleted);
         assert(foundTriangle != V1V2VN || helper->v1v2vNTriangleToBeDeleted);

         helper->v0v1Edge.convertToInternal = _isConvertionToInternalNeeded(
            mesh,
            helper->v0v1Edge,
            helper->v0v1v2TriangleToBeDeleted,
            helper->v0v1vNTriangleToBeDeleted
         );

         helper->v0v2Edge.convertToInternal = _isConvertionToInternalNeeded(
            mesh,
            helper->v0v2Edge,
            helper->v0v1v2TriangleToBeDeleted,
            helper->v0vNv2TriangleToBeDeleted
         );

         helper->v1v2Edge.convertToInternal = _isConvertionToInternalNeeded(
            mesh,
            helper->v1v2Edge,
            helper->v0v1v2TriangleToBeDeleted,
            helper->v1v2vNTriangleToBeDeleted
         );

         // Fill triangle helpers;
         helper->triangleHelpers.reserve(4);
         helper->triangleHelpers.push_back(BuddingTriangleHelper(
            helper->v0v1v2TriangleToBeDeleted
         ));

         if (helper->v0v1vNTriangleToBeDeleted)
         {
            helper->triangleHelpers.push_back(BuddingTriangleHelper(
               *helper->v0v1vNTriangleToBeDeleted
            ));
         }
         else
         {
            helper->triangleHelpers.push_back(BuddingTriangleHelper(
               v0, v1, *helper->vNew
            ));
         }

         if (helper->v0vNv2TriangleToBeDeleted)
         {
            helper->triangleHelpers.push_back(BuddingTriangleHelper(
               *helper->v0vNv2TriangleToBeDeleted
            ));
         }
         else
         {
            helper->triangleHelpers.push_back(BuddingTriangleHelper(
               v0, *helper->vNew, v2
            ));
         }

         if (helper->v1v2vNTriangleToBeDeleted)
         {
            helper->triangleHelpers.push_back(BuddingTriangleHelper(
               *helper->v1v2vNTriangleToBeDeleted
            ));
         }
         else
         {
            helper->triangleHelpers.push_back(BuddingTriangleHelper(
               v1, v2, *helper->vNew
            ));
         }

         // Subsequent triangle deletion requires id vector to be sorted in
         // ascending order;
         std::sort(
            helper->triangleHelpers.begin(),
            helper->triangleHelpers.end()
         );
      }

      if (helper->v0vNewEdge)
      {
         helper->v0vNewEdge->convertToInternal = _isConvertionToInternalNeeded(
            mesh,
            *helper->v0vNewEdge,
            helper->v0v1vNTriangleToBeDeleted,
            helper->v0vNv2TriangleToBeDeleted
         );
      }
      if (helper->v1vNewEdge)
      {
         helper->v1vNewEdge->convertToInternal = _isConvertionToInternalNeeded(
            mesh,
            *helper->v1vNewEdge,
            helper->v0v1vNTriangleToBeDeleted,
            helper->v1v2vNTriangleToBeDeleted
         );
      }
      if (helper->v2vNewEdge)
      {
         helper->v2vNewEdge->convertToInternal = _isConvertionToInternalNeeded(
            mesh,
            *helper->v2vNewEdge,
            helper->v0vNv2TriangleToBeDeleted,
            helper->v1v2vNTriangleToBeDeleted
         );
      }
   }

   return helper;
}


}
