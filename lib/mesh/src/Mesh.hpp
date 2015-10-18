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


#ifndef MESH_MESH_H
#define MESH_MESH_H


#include <cstdlib>
#include <list>
#include <map>
#include <ostream>
#include <utility>
#include <vector>
#include <GL/gl.h>


#include <boost/optional.hpp>


#include "datatypes/geometry.hpp"
#include "datatypes/mesh.hpp"
#include "datatypes/numeric.hpp"


#include "MemoryModification.hpp"
#include "StructureModification.hpp"
#include "Tetrahedron.hpp"
#include "Triangle.hpp"
#include "TrianglesMap.hpp"


namespace mesh {


class Connections;
class EdgeModifier;
class VertexModifier;
struct BuddingEdgeHelper;
struct BuddingParams;
struct DynamicVertex;
struct Edge;
struct StaticVertex;


/***************************************************************************
 *   Mesh class declaration                                                *
 ***************************************************************************/


class Mesh
{
   public:
      enum MAX_BUFFER_SIZE
      {
         MBS_MAX_VERTEX_COUNT = 4 + 16380,
         MBS_MAX_TRIANGLE_COUNT = 4 + 2 * 16380,
         MBS_MAX_EDGE_COUNT = 32768,
         MBS_MAX_CONNECTION_COUNT = 2 * MBS_MAX_EDGE_COUNT
      };

      explicit Mesh(
         const dt::Pointf3 & a,
         const dt::Pointf3 & b,
         const dt::Pointf3 & c,
         const dt::Pointf3 & d
      );
      virtual ~Mesh();

      virtual boost::optional<Tetrahedron> makeTetrahedronBud(
         const Tetrahedron & t,
         const BuddingParams & params
      );

      virtual void resizeEdge(GLint edge, dt::Float equilibriumLength);
      virtual void setVertexPos(
         const dt::VertexId & v,
         dt::Float x,
         dt::Float y,
         dt::Float z
      );
      virtual void setVertexPosX(const dt::VertexId & v, dt::Float x);
      virtual void setVertexPosY(const dt::VertexId & v, dt::Float y);
      virtual void setVertexPosZ(const dt::VertexId & v, dt::Float z);
      virtual void setVertexMass(const dt::VertexId & v, dt::Float mass);
      virtual void selectVertex(
         const boost::optional<dt::VertexId> & vertex,
         dt::SelectionMode selectionMode = dt::SM_Vertex
      );

      virtual const DynamicVertex * dynamicVertices() const;
      inline const StaticVertex * staticVertices() const;
      inline const Triangle * triangles() const;
      inline const Edge * edges() const;
      inline const Connections & connections() const;

      inline size_t vertexCount() const;
      inline size_t triangleCount() const;
      inline size_t edgeCount() const;

      inline const TrianglesMap & trianglesMap() const;

      inline const StructureModification & structureMods() const;
      void clearStructureModifications();

      std::vector<Tetrahedron> adjacentTetrahedrons(
         const Tetrahedron & t,
         dt::TetrahedronFace face
      ) const;
      std::vector<Tetrahedron> adjacentTetrahedrons(
         const Tetrahedron & t
      ) const;

      boost::optional<dt::VertexId> selectedVertex() const;
      boost::optional<std::pair<size_t, size_t> > selectedEdgeVertices(
         ) const;
      boost::optional<size_t> selectedEdge() const;
      boost::optional<dt::TriangleId> selectedTriangle() const;
      boost::optional<Tetrahedron> selectedTetrahedron() const;

      dt::Float tetrahedronVolume(const Tetrahedron & t) const;

      dt::Pointf3 center() const;
      dt::Vectorf3 dimensions() const;

   protected:
      inline const MemoryModification & dynamicVertexMods() const;
      inline const MemoryModification & staticVertexMods() const;
      inline const MemoryModification & triangleMods() const;
      inline const MemoryModification & edgeMods() const;
      void clearMemoryModifications();
      void invalidateDimensions();

   private:
      void clearSelection();
      dt::Float newEdgeLength(
         const dt::VertexId & v1,
         const dt::VertexId & v2
      ) const;
      void helpEdgeBudding(
         const BuddingEdgeHelper & edge,
         const dt::VertexId & va,
         const dt::VertexId & vb
      );
      void helpEdgeBudding(
         const boost::optional<BuddingEdgeHelper> & currentEdge,
         const boost::optional<BuddingEdgeHelper> & otherEdge0,
         const boost::optional<BuddingEdgeHelper> & otherEdge1,
         const dt::VertexId & va,
         const dt::VertexId & vNew,
         const dt::VertexId & vb,
         const dt::VertexId & vc
      );
      bool ensureNoCollisionsForBud(
         const dt::TriangleId & budBase,
         const dt::Pointf3 & budTop,
         const boost::optional<dt::VertexId> & budTopIndex
      ) const;
      dt::VertexId createBudTopVertex(
         const dt::VertexId & v0,
         const dt::VertexId & v1,
         const dt::VertexId & v2,
         const dt::Pointf3 & budTop
      );
      dt::EdgeId createEdge(const dt::VertexId & v0, const dt::VertexId & v1);
      size_t createTriangle(
         const dt::VertexId & v0,
         const dt::VertexId & v1,
         const dt::VertexId & v2
      );
      void removeAllConnections(const dt::VertexId & v);
      bool removeConnection(
         const dt::VertexId & v,
         const dt::VertexId & connectedV,
         bool removeEdge
      );
      void removeVertex(const dt::VertexId & v);
      void removeEdge(GLint e);
      void removeTriangle(const dt::TriangleId & t);
      void moveLastVertex(const dt::VertexId & newIndex);
      void moveLastEdge(GLint newIndex);
      void moveLastTriangle(const dt::TriangleId & newId);
      bool isVertexSelected(const dt::VertexId & v) const;
      void setVertexSelected(const dt::VertexId & v, bool s);

      void recalculateDimensions() const;

      DynamicVertex * m_dynamicVertices;
      StaticVertex * m_staticVertices;
      Triangle * m_triangles;
      Edge * m_edges;
      Connections * m_connections;

      size_t m_vertexCount;
      size_t m_triangleCount;
      size_t m_edgeCount;

      TrianglesMap m_trianglesMap;

      MemoryModification m_dynamicVertexMods;
      MemoryModification m_staticVertexMods;
      MemoryModification m_triangleMods;
      MemoryModification m_edgeMods;
      StructureModification m_structureMods;

      std::vector<dt::VertexId> m_selection;

      mutable boost::optional<dt::Pointf3> m_center;
      mutable boost::optional<dt::Vectorf3> m_dimensions;
};


inline const StaticVertex * Mesh::staticVertices() const
{
   return m_staticVertices;
}


inline const Triangle * Mesh::triangles() const
{
   return m_triangles;
}


inline const Edge * Mesh::edges() const
{
   return m_edges;
}


inline const Connections & Mesh::connections() const
{
   return *m_connections;
}


inline size_t Mesh::vertexCount() const
{
   return m_vertexCount;
}


inline size_t Mesh::triangleCount() const
{
   return m_triangleCount;
}


inline size_t Mesh::edgeCount() const
{
   return m_edgeCount;
}


inline const TrianglesMap & Mesh::trianglesMap() const
{
   return m_trianglesMap;
}


inline const StructureModification & Mesh::structureMods() const
{
   return m_structureMods;
}


inline const MemoryModification & Mesh::dynamicVertexMods() const
{
   return m_dynamicVertexMods;
}


inline const MemoryModification & Mesh::staticVertexMods() const
{
   return m_staticVertexMods;
}


inline const MemoryModification & Mesh::triangleMods() const
{
   return m_triangleMods;
}


inline const MemoryModification & Mesh::edgeMods() const
{
   return m_edgeMods;
}


std::ostream & operator<<(std::ostream & os, const Mesh & mesh);


}


#endif
