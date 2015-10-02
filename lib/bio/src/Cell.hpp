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


#ifndef BIO_CELL_HPP
#define BIO_CELL_HPP


#include <cstdint>
#include <ostream>


#include <boost/optional.hpp>


#include "datatypes/mesh.hpp"
#include "datatypes/numeric.hpp"


#include "mesh/BuddingParams.hpp"
#include "mesh/Tetrahedron.hpp"


namespace bio {


class Cell;
struct Gene;


/***************************************************************************
 *   AdjacentCell class declaration                                        *
 ***************************************************************************/


struct AdjacentCell
{
   explicit AdjacentCell(const Cell * cell, dt::TetrahedronFace face);

   const Cell * cell;
   dt::TetrahedronFace face;
};


bool operator==(const AdjacentCell & lhs, const AdjacentCell & rhs);


/***************************************************************************
 *   Cell class declaration                                                *
 ***************************************************************************/


#pragma pack(push)
#pragma pack(1)


class Cell
{
   public:
      explicit Cell(const mesh::Tetrahedron & ttr, int16_t x, int16_t y);

      bool doesMeetConditions(const Gene & gene) const;

      inline mesh::Tetrahedron tetrahedron() const {return m_tetrahedron;}
      void setTetrahedron(const mesh::Tetrahedron & ttr);

      inline unsigned int generation() const {return m_generation;}
      inline int16_t x() const {return m_x;}
      inline int16_t y() const {return m_y;}

      Cell * makeBud(
         const mesh::Tetrahedron & budTtr,
         dt::TetrahedronFace face,
         const std::vector<AdjacentCell> & adjacentCells
      );

      bool operator==(const Cell & other) const;

   private:
      mesh::Tetrahedron m_tetrahedron;
      unsigned int m_generation;
      int16_t m_x;
      int16_t m_y;
};


#pragma pack(pop)


std::ostream & operator<<(std::ostream & os, const Cell & cell);


}


#endif
