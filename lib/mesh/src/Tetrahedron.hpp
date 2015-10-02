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


#ifndef MESH_TETRAHEDRON_H
#define MESH_TETRAHEDRON_H


#include <ostream>


#include <boost/optional.hpp>


#include "datatypes/mesh.hpp"


namespace mesh {


/***************************************************************************
 *   Tetrahedron structure declaration                                     *
 ***************************************************************************/


#pragma pack(push)
#pragma pack(1)


struct Tetrahedron
{
   enum COMBINATION
   {
      C_ABCD = 0, C_ABDC, C_ACBD, C_ACDB, C_ADBC, C_ADCB,
      C_BACD, C_BADC, C_BCAD, C_BCDA, C_BDAC, C_BDCA,
      C_CABD, C_CADB, C_CBAD, C_CBDA, C_CDAB, C_CDBA,
      C_DABC, C_DACB, C_DBAC, C_DBCA, C_DCAB, C_DCBA,
      C_FIRST = C_ABCD,
      C_LAST = C_DCBA
   };

   explicit Tetrahedron(size_t a, size_t b, size_t c, size_t d);

   Tetrahedron replaced(size_t x, size_t y) const;
   Tetrahedron combination(COMBINATION combination) const;
   boost::optional<dt::TetrahedronFace> faceAny(
      size_t v1,
      size_t v2,
      size_t v3
   ) const;
   boost::optional<dt::TetrahedronFace> borderFace(
      const Tetrahedron & other
   ) const;
   boost::optional<dt::TetrahedronFace> relativeFace(
      const Tetrahedron & other,
      dt::TetrahedronFace otherFace
   ) const;

   bool contains(size_t x) const;
   bool faceContains(dt::TetrahedronFace f, size_t x, size_t y, size_t z) const;

   bool operator==(const Tetrahedron & other) const;
   bool operator<(const Tetrahedron & other) const;

   size_t a, b, c, d;
};


#pragma pack(pop)


std::ostream & operator<<(std::ostream & os, const Tetrahedron & t);


}


#endif
