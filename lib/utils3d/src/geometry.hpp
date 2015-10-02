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


#ifndef UTILS3D_GEOMETRY_HPP
#define UTILS3D_GEOMETRY_HPP


#include <boost/optional.hpp>

#include "datatypes/geometry.hpp"


namespace utils3d {


/***************************************************************************
 *   Geometry utils declaration                                            *
 ***************************************************************************/


dt::Float distance(const dt::Pointf3 & p1, const dt::Pointf3 & p2);


dt::Vectorf3 getNormalToPlane(
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3
);


dt::Pointf3 getTetrahedronTopPoint(
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3,
   dt::Float budTopRadius,
   dt::Float budTopPolarAngle,
   dt::Float budTopAzimuthalAngle
);


boost::optional<dt::Pointf3> getLinePlaneIntersection(
   const dt::LineSegmentf3 &ls,
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3
);


boost::optional<dt::Pointf3> getLineSegmentPlaneIntersection(
   const dt::LineSegmentf3 &ls,
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3
);


boost::optional<dt::Pointf3> getLineSegmentTriangleIntersection(
   const dt::LineSegmentf3 &ls,
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c
);


bool doTrianglesIntersect(
   const dt::Pointf3 & x,
   const dt::Pointf3 & y,
   const dt::Pointf3 & z,
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c
);


bool doesPointBelongToRightTriangularPrismWithInfinitHeight(
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c,
   const dt::Pointf3 & p,
   bool * above = 0
);


bool doesPointAbovePlane(
   const dt::Pointf3 & a,
   const dt::Pointf3 & b,
   const dt::Pointf3 & c,
   const dt::Pointf3 & p
);


dt::Float getTetrahedronVolume(
   const dt::Pointf3 & p1,
   const dt::Pointf3 & p2,
   const dt::Pointf3 & p3,
   const dt::Pointf3 & p4
);


}


#endif
