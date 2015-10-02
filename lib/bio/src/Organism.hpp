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


#ifndef BIO_ORGANISM_H
#define BIO_ORGANISM_H


#include <cstdint>


#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>


#include "datatypes/mesh.hpp"
#include "mesh/Tetrahedron.hpp"


#include "TetrahedronsMap.hpp"


namespace mesh {
class Mesh;
struct BuddingParams;
}


namespace bio {


class Cell;
struct AdjacentCell;
struct OrganismDesc;


/***************************************************************************
 *   Organism class declaration                                            *
 ***************************************************************************/


class Organism
{
   public:
      explicit Organism(
         mesh::Mesh * mesh,
         boost::shared_ptr<const OrganismDesc> desc
      );
      virtual ~Organism();

      void stepOver();

      bool makeCellBud(const mesh::Tetrahedron & t, dt::TetrahedronFace face);
      void setVertexPos(const dt::VertexId & v, float x, float y, float z);
      void setVertexPosX(const dt::VertexId & v, float x);
      void setVertexPosY(const dt::VertexId & v, float y);
      void setVertexPosZ(const dt::VertexId & v, float z);
      void setVertexMass(const dt::VertexId & v, float mass);
      void selectVertex(
         const boost::optional<dt::VertexId> & v,
         dt::SelectionMode selectionMode = dt::SM_Vertex
      );
      boost::optional<mesh::Tetrahedron> selectedCell() const;
      void resizeEdge(size_t edge, float equilibriumLength);

      inline bool isFinished() const {return m_isFinished;}
      inline uint8_t progress() const {return m_progress;}

      inline const mesh::Mesh & mesh() const;
      inline const TetrahedronsMap & tetrahedronsMap() const;

   private:
      void finish();
      Cell * makeCellBud(Cell & cell, const mesh::BuddingParams & params);
      std::vector<AdjacentCell> adjacentCells(
         const mesh::Tetrahedron & t
      ) const;
      void applyStructureModifications();

      mesh::Mesh * m_mesh;
      boost::shared_ptr<const OrganismDesc> m_desc;
      TetrahedronsMap m_tetrahedronsMap;
      bool m_atLeastOneBuddingHasOccured;
      int32_t m_geneExpressionCount;
      bool m_isFinished;
      uint8_t m_progress;
};


inline const mesh::Mesh & Organism::mesh() const
{
   return *m_mesh;
}


inline const TetrahedronsMap & Organism::tetrahedronsMap() const
{
   return m_tetrahedronsMap;
}


}


#endif
