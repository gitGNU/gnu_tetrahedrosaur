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


#ifndef MESH_BUDDINGPARAMS_H
#define MESH_BUDDINGPARAMS_H


#include "datatypes/geometry.hpp"
#include "datatypes/mesh.hpp"


namespace mesh {


/***************************************************************************
 *   BuddingParams structure declaration                                   *
 ***************************************************************************/


struct BuddingParams
{
   explicit BuddingParams(dt::TetrahedronFace face);
   explicit BuddingParams(
      dt::TetrahedronFace face,
      dt::Float radius,
      dt::Float polarAngle,
      dt::Float azimuthalAngle
   );

   bool operator==(const BuddingParams & other) const;

   dt::TetrahedronFace face;
   dt::Float radius;
   dt::Float polarAngle;
   dt::Float azimuthalAngle;
};


}


#endif
