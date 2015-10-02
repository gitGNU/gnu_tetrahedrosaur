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


#include "BuddingParams.hpp"


namespace mesh {


/***************************************************************************
 *   BuddingParams structure implementation                                *
 ***************************************************************************/


BuddingParams::BuddingParams(dt::TetrahedronFace face)
   : face(face), radius(1.0f), polarAngle(0.0f), azimuthalAngle(0.0f)
{
}


BuddingParams::BuddingParams(
   dt::TetrahedronFace face,
   dt::Float radius,
   dt::Float polarAngle,
   dt::Float azimuthalAngle
) : face(face),
   radius(radius),
   polarAngle(polarAngle),
   azimuthalAngle(azimuthalAngle)
{
}


bool BuddingParams::operator==(const BuddingParams & other) const
{
   return face == other.face &&
      radius == other.radius &&
      polarAngle == other.polarAngle &&
      azimuthalAngle == other.azimuthalAngle;
}


}
