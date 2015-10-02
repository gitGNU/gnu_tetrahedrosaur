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


#include "Cell.hpp"
#include "Gene.hpp"
#include "InstructionSet.hpp"


namespace {


struct _BuddingOption
{
   explicit _BuddingOption(const mesh::Tetrahedron & bud);

   int16_t x() const;
   int16_t y() const;

   mesh::Tetrahedron bud;
   const bio::Cell *cellX;
   const bio::Cell *cellY;
   int16_t dx;
   int16_t dy;
   uint8_t properlyOrientedFaceCount;
};


_BuddingOption::_BuddingOption(const mesh::Tetrahedron & bud)
   : bud(bud), cellX(0), cellY(0), dx(0), dy(0), properlyOrientedFaceCount(0)
{
}


int16_t _BuddingOption::x() const
{
   return (cellX ? cellX->x() + dx : 0);
}


int16_t _BuddingOption::y() const
{
   return (cellY ? cellY->y() + dy : 0);
}


bool operator>(const _BuddingOption & lhs, const _BuddingOption & rhs)
{
   return (lhs.properlyOrientedFaceCount > rhs.properlyOrientedFaceCount);
}


} // anonymous namespace;


namespace bio {


/***************************************************************************
 *   AdjacentCell class implementation                                     *
 ***************************************************************************/


AdjacentCell::AdjacentCell(const Cell * cell, dt::TetrahedronFace face)
   : cell(cell), face(face)
{
}


bool operator==(const AdjacentCell & lhs, const AdjacentCell & rhs)
{
   return (lhs.cell == rhs.cell && lhs.face == rhs.face);
}


/***************************************************************************
 *   Cell class implementation                                             *
 ***************************************************************************/


Cell::Cell(const mesh::Tetrahedron & ttr, int16_t x, int16_t y)
   : m_tetrahedron(ttr), m_generation(0), m_x(x), m_y(y)
{
}


bool Cell::doesMeetConditions(const Gene & gene) const
{
   if (!gene.generationCondition.isAcceptable(dt::UInt16(m_generation)))
   {
      return false;
   }
   if (!gene.xCondition.isAcceptable(dt::Int16(m_x)))
   {
      return false;
   }
   if (!gene.yCondition.isAcceptable(dt::Int16(m_y)))
   {
      return false;
   }
   return true;
}


void Cell::setTetrahedron(const mesh::Tetrahedron & ttr)
{
   m_tetrahedron = ttr;
}


Cell * Cell::makeBud(
   const mesh::Tetrahedron & budTtr,
   dt::TetrahedronFace face,
   const std::vector<AdjacentCell> & adjacentCells
)
{
   const mesh::Tetrahedron & oct = tetrahedron();
   boost::optional<_BuddingOption> bestOption;
   for (int c = mesh::Tetrahedron::C_FIRST; c <= mesh::Tetrahedron::C_LAST; ++c)
   {
      enum {_X, _Y} fixed = _X;
      _BuddingOption option(
         budTtr.combination(static_cast<mesh::Tetrahedron::COMBINATION>(c))
      );
      switch (face) {
         case dt::TF_ABC:
            if (!option.bud.faceContains(dt::TF_ACD, oct.a, oct.b, oct.c))
            {
               continue;
            }
            option.cellX = this;
            option.cellY = this;
            option.dx = -1;
            option.dy = 0;
            fixed = _X;
            break;
         case dt::TF_ACD:
            if (!option.bud.faceContains(dt::TF_ABC, oct.a, oct.c, oct.d))
            {
               continue;
            }
            option.cellX = this;
            option.cellY = this;
            option.dx = 1;
            option.dy = 0;
            fixed = _X;
            break;
         case dt::TF_ADB:
            if (!option.bud.faceContains(dt::TF_BCD, oct.a, oct.d, oct.b))
            {
               continue;
            }
            option.cellX = this;
            option.cellY = this;
            option.dx = 0;
            option.dy = -1;
            fixed = _Y;
            break;
         case dt::TF_BCD:
            if (!option.bud.faceContains(dt::TF_ADB, oct.b, oct.c, oct.d))
            {
               continue;
            }
            option.cellX = this;
            option.cellY = this;
            option.dx = 0;
            option.dy = 1;
            fixed = _Y;
            break;
      }
      option.properlyOrientedFaceCount = 1;

      for (const AdjacentCell & adjCell : adjacentCells)
      {
         if (adjCell.cell == this)
         {
            assert(adjCell.face == face);
            continue;
         }
         assert(adjCell.cell);
         assert(adjCell.cell->tetrahedron().borderFace(option.bud) ==
            adjCell.face);
         const boost::optional<dt::TetrahedronFace> borderFace =
            option.bud.borderFace(adjCell.cell->tetrahedron());
         assert(borderFace);
         switch (adjCell.face) {
            case dt::TF_ABC:
               if (*borderFace == dt::TF_ACD)
               {
                  option.properlyOrientedFaceCount++;
                  if (fixed != _X)
                  {
                     option.cellX = adjCell.cell;
                     option.dx = -1;
                  }
               }
               break;
            case dt::TF_ACD:
               if (*borderFace == dt::TF_ABC)
               {
                  option.properlyOrientedFaceCount++;
                  if (fixed != _X)
                  {
                     option.cellX = adjCell.cell;
                     option.dx = 1;
                  }
               }
               break;
            case dt::TF_ADB:
               if (*borderFace == dt::TF_BCD)
               {
                  option.properlyOrientedFaceCount++;
                  if (fixed != _Y)
                  {
                     option.cellY = adjCell.cell;
                     option.dy = -1;
                  }
               }
               break;
            case dt::TF_BCD:
               if (*borderFace == dt::TF_ADB)
               {
                  option.properlyOrientedFaceCount++;
                  if (fixed != _Y)
                  {
                     option.cellY = adjCell.cell;
                     option.dy = 1;
                  }
               }
               break;
         }
      }

      if (!bestOption || option > *bestOption)
      {
         bestOption = option;
      }
   }
   assert(bestOption);

   Cell * bud = new Cell(bestOption->bud, bestOption->x(), bestOption->y());
   bud->m_generation = m_generation + 1;
   return bud;
}


bool Cell::operator==(const Cell & other) const
{
   return (m_tetrahedron == other.m_tetrahedron &&
      m_generation == other.m_generation &&
      m_x == other.m_x &&
      m_y == other.m_y
   );
}


std::ostream & operator<<(std::ostream & os, const Cell & cell)
{
   /*os << "{" << cell.v0() << ", " << cell.v1() << ", " <<
      cell.v2() << ", " << cell.v3() << "}";*/
   return os;
}


}
