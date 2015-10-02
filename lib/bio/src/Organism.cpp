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


#include <cassert>


#include "mesh/BuddingParams.hpp"
#include "mesh/Mesh.hpp"


#include "Cell.hpp"
#include "Config.hpp"
#include "GeneInitializer.hpp"
#include "Genome.hpp"
#include "Organism.hpp"
#include "OrganismDesc.hpp"


namespace bio {


/***************************************************************************
 *   Organism class implementation                                         *
 ***************************************************************************/


Organism::Organism(
   mesh::Mesh * mesh,
   boost::shared_ptr<const OrganismDesc> desc
) : m_mesh(mesh),
   m_desc(desc),
   m_atLeastOneBuddingHasOccured(false),
   m_geneExpressionCount(0),
   m_isFinished(false),
   m_progress(0)
{
   assert(m_mesh);
   assert(m_mesh->vertexCount() == 4);
   assert(m_desc);
   assert(m_desc->genome);

   const mesh::Tetrahedron ttr(0, 1, 2, 3);
   Cell * cell = new Cell(
      ttr,
      m_desc->initialConditions.x,
      m_desc->initialConditions.y
   );
   m_tetrahedronsMap.insert(ttr, cell);
}


Organism::~Organism()
{
   delete m_mesh;
}


void Organism::stepOver()
{
   if (m_isFinished)
   {
      return;
   }

   bool buddingOccurred = false;
   const std::vector<Gene> & genes = m_desc->genome->genes();
   for (const auto & ttrCellPair : m_tetrahedronsMap)
   {
      Cell * activeCell = ttrCellPair.second;

      // Apply genes to the active cell;
      GeneInitializer init;
      const Config & config = *m_desc->genome->config();
      for (const Gene & gene : genes)
      {
         if (activeCell->doesMeetConditions(gene))
         {
            for (const Instruction & instr : gene.responses)
            {
               init.append(instr, config);
            }
         }
      }

      std::pair<dt::TetrahedronFace, const AverageGeneI8x3Param *> avp[4] = {
         {dt::TF_ABC, &init.abcBud},
         {dt::TF_ACD, &init.acdBud},
         {dt::TF_ADB, &init.adbBud},
         {dt::TF_BCD, &init.bcdBud}
      };
      for (size_t i = 0; i < 4; ++i)
      {
         if (const auto bud = avp[i].second->value())
         {
            if (m_tetrahedronsMap.size() >= m_desc->initialConditions.cellLimit)
            {
               finish();
               return;
            }

            const mesh::BuddingParams bp(
               avp[i].first,
               config.budTopRadius.convert(bud->a),
               config.budTopPolarAngle.convert(bud->b),
               config.budTopAzimuthalAngle.convert(bud->c)
            );
            if (Cell * budCell = makeCellBud(*activeCell, bp))
            {
               m_tetrahedronsMap.insert(budCell->tetrahedron(), budCell);
               buddingOccurred = true;
            }
         }
      }

      if (buddingOccurred)
      {
         const float progress =
            (static_cast<float>(m_tetrahedronsMap.size()) * 100.0f) /
            static_cast<float>(m_desc->initialConditions.cellLimit);
         m_progress = static_cast<uint8_t>(progress);
         break;
      }
   }

   if (!buddingOccurred)
   {
      finish();
   }
}


bool Organism::makeCellBud(
   const mesh::Tetrahedron & t,
   dt::TetrahedronFace face
)
{
   if (Cell * cell = m_tetrahedronsMap.findAny(t))
   {
      if (const auto cellFace = cell->tetrahedron().relativeFace(t, face))
      {
         Cell * budCell = makeCellBud(*cell, mesh::BuddingParams(*cellFace));
         if (budCell)
         {
            m_tetrahedronsMap.insert(budCell->tetrahedron(), budCell);
            return true;
         }
      }
   }
   return false;
}


void Organism::setVertexPos(const dt::VertexId & v, float x, float y, float z)
{
   assert(m_mesh->structureMods().empty());
   m_mesh->setVertexPos(v, x, y, z);
}


void Organism::setVertexPosX(const dt::VertexId & v, float x)
{
   assert(m_mesh->structureMods().empty());
   m_mesh->setVertexPosX(v, x);
}


void Organism::setVertexPosY(const dt::VertexId & v, float y)
{
   assert(m_mesh->structureMods().empty());
   m_mesh->setVertexPosY(v, y);
}


void Organism::setVertexPosZ(const dt::VertexId & v, float z)
{
   assert(m_mesh->structureMods().empty());
   m_mesh->setVertexPosZ(v, z);
}


void Organism::setVertexMass(const dt::VertexId & v, float mass)
{
   assert(m_mesh->structureMods().empty());
   m_mesh->setVertexMass(v, mass);
}


void Organism::selectVertex(
   const boost::optional<dt::VertexId> & v,
   dt::SelectionMode selectionMode
)
{
   assert(m_mesh->structureMods().empty());
   m_mesh->selectVertex(v, selectionMode);
}


boost::optional<mesh::Tetrahedron> Organism::selectedCell() const
{
   const boost::optional<mesh::Tetrahedron> t = m_mesh->selectedTetrahedron();
   if (t)
   {
      const Cell * cell = m_tetrahedronsMap.findAny(*t);
      if (cell)
      {
         return cell->tetrahedron();
      }
   }
   return boost::none;
}


void Organism::resizeEdge(size_t edge, float equilibriumLength)
{
   assert(m_mesh->structureMods().empty());
   m_mesh->resizeEdge(edge, equilibriumLength);
}


void Organism::finish()
{
   m_isFinished = true;
   m_progress = 100;
}


Cell * Organism::makeCellBud(Cell & cell, const mesh::BuddingParams & params)
{
   const mesh::Tetrahedron t = cell.tetrahedron();
   if (m_mesh->tetrahedronVolume(t) > 0.0f) {
      boost::optional<mesh::Tetrahedron> bud =
         m_mesh->makeTetrahedronBud(t, params);
      if (bud)
      {
         // Find the adjacent tetrahedrons;
         const std::vector<AdjacentCell> adj = std::move(adjacentCells(*bud));
         assert(!adj.empty());
         assert(std::find(
            adj.begin(),
            adj.end(),
            AdjacentCell(&cell, params.face)
         ) != adj.end());
         return cell.makeBud(*bud, params.face, adj);
      }
   }
   return 0;
}


std::vector<AdjacentCell> Organism::adjacentCells(
   const mesh::Tetrahedron & t
) const
{
   std::vector<AdjacentCell> result;
   result.reserve(4);

   const auto adjTtrs = m_mesh->adjacentTetrahedrons(t);
   for (const mesh::Tetrahedron & adjTtr : adjTtrs)
   {
      // Not all tetrahedrons returned by adjacentTetrahedrons are cells;
      if (const Cell * adjCell = m_tetrahedronsMap.findAny(adjTtr))
      {
         const mesh::Tetrahedron actualAdjTtr = adjCell->tetrahedron();
         auto borderFace = actualAdjTtr.borderFace(t);
         assert(borderFace);
         result.push_back(AdjacentCell(adjCell, *borderFace));
      }
   }
   return result;
}


void Organism::applyStructureModifications()
{
   const mesh::StructureModification & structureMods = m_mesh->structureMods();
   mesh::StructureModification::const_iterator it = structureMods.begin();
   mesh::StructureModification::const_iterator ite = structureMods.end();
   for (; it != ite; ++it)
   {
      switch (it->type)
      {
         case mesh::StructureModification::T_VERTEX_MOVE:
            m_tetrahedronsMap.replaceVertexIndex(it->oldIndex, it->newIndex);
            break;
      }
   }
   m_mesh->clearStructureModifications();
}


}
