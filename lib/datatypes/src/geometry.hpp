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


#ifndef DATATYPES_GEOMETRY_HPP
#define DATATYPES_GEOMETRY_HPP


#include <GL/gl.h>
#include <boost/numeric/ublas/matrix.hpp>


namespace dt {


typedef GLfloat Float;

typedef boost::numeric::ublas::matrix<Float,
   boost::numeric::ublas::column_major> Matrixf;


#pragma pack(push)
#pragma pack(1)


/***************************************************************************
 *   Pointf3 structure declaration                                         *
 ***************************************************************************/


struct Vectorf3;


struct Pointf3
{
   explicit Pointf3(Float x, Float y, Float z);

   Pointf3 & operator+=(const Vectorf3 & vector);
   const Pointf3 operator+(const Vectorf3 & vector) const;

   bool operator==(const Pointf3 &other) const;

   Float x;
   Float y;
   Float z;
};


/***************************************************************************
 *   Vectorf3 structure declaration                                        *
 ***************************************************************************/


struct Vectorf3
{
   explicit Vectorf3(Float x, Float y, Float z);
   explicit Vectorf3(const Pointf3 & begin, const Pointf3 & end);

   Float length() const;
   Vectorf3 normalized() const;

   Vectorf3 & operator*=(Float value);
   const Vectorf3 operator*(Float value) const;

   bool operator==(const Vectorf3 &other) const;

   Float x;
   Float y;
   Float z;
};


Vectorf3 crossProduct(const Vectorf3 &v1, const Vectorf3 &v2);
Float dotProduct(const Vectorf3 &v1, const Vectorf3 &v2);


/***************************************************************************
 *   LineSegmentf3 structure declaration                                   *
 ***************************************************************************/


struct LineSegmentf3
{
   explicit LineSegmentf3(const Pointf3 & a, const Pointf3 & b);
   explicit LineSegmentf3(const Pointf3 & p, const Vectorf3 & vector);

   Float length() const;

   Pointf3 a;
   Pointf3 b;
};


#pragma pack(pop)


}


#endif
