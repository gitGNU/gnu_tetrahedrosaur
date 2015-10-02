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


#include <cmath>
#include <cstring>
#include <limits>


#include <boost/test/unit_test.hpp>


#include "geometry.hpp"


using namespace utils3d;


/***************************************************************************
 *   Geometry utils test                                                   *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libutils3d_geometry)


BOOST_AUTO_TEST_CASE(test_getLineSegmentPlaneIntersection)
{
   #define _EQUAL(_a, _b) (fabs((_a) - (_b)) <= 0.00001)

   {
      dt::LineSegmentf3 ls(dt::Pointf3(-0.5f, -0.5f, -1.0f), dt::Pointf3(-0.5f, -0.5f, 1.0f));
      dt::Pointf3 a(-1.0f, -1.0f, 0.5f);
      dt::Pointf3 b(-1.0f, 1.0f, 0.5f);
      dt::Pointf3 c(1.0f, -1.0f, 0.5f);

      auto p = utils3d::getLineSegmentPlaneIntersection(ls, a, b, c);
      BOOST_REQUIRE(p);
      BOOST_REQUIRE(_EQUAL(p->x, -0.5f));
      BOOST_REQUIRE(_EQUAL(p->y, -0.5f));
      BOOST_REQUIRE(_EQUAL(p->z, 0.5f));
   }

   #undef _EQUAL
}


BOOST_AUTO_TEST_CASE(test_getLineSegmentTriangleIntersection)
{
   #define _EQUAL(_a, _b) (fabs((_a) - (_b)) <= 0.00001)

   {
      dt::LineSegmentf3 ls(dt::Pointf3(-0.5f, -0.5f, -1.0f), dt::Pointf3(-0.5f, -0.5f, 1.0f));
      dt::Pointf3 a(-1.0f, -1.0f, 0.5f);
      dt::Pointf3 b(-1.0f, 1.0f, 0.5f);
      dt::Pointf3 c(1.0f, -1.0f, 0.5f);

      auto p = utils3d::getLineSegmentTriangleIntersection(ls, a, b, c);
      BOOST_REQUIRE(p);
      BOOST_REQUIRE(_EQUAL(p->x, -0.5f));
      BOOST_REQUIRE(_EQUAL(p->y, -0.5f));
      BOOST_REQUIRE(_EQUAL(p->z, 0.5f));
   }

   {
      dt::LineSegmentf3 ls(dt::Pointf3(5.0f, 5.0f, -1.0f), dt::Pointf3(5.0f, 5.0f, 1.0f));
      dt::Pointf3 a(-1.0f, -1.0f, 0.5f);
      dt::Pointf3 b(-1.0f, 1.0f, 0.5f);
      dt::Pointf3 c(1.0f, -1.0f, 0.5f);

      BOOST_REQUIRE(!utils3d::getLineSegmentTriangleIntersection(ls, a, b, c));
   }

   #undef _EQUAL
}



BOOST_AUTO_TEST_CASE(test_doesPointBelongToRightTriangularPrismWithInfinitHeight)
{
   {
      const dt::Pointf3 a(5.0f, 1.0f, 0.0f);
      const dt::Pointf3 b(5.0f, -1.0f, -1.0f);
      const dt::Pointf3 c(5.0f, -1.0f, 1.0f);

      const dt::Pointf3 p1no(6.0f, 0.0f, 0.0f);
      const dt::Pointf3 p1yes(4.5f, 0.0f, 0.0f);
      BOOST_REQUIRE(!doesPointBelongToRightTriangularPrismWithInfinitHeight(
         a, b, c, p1no)
      );
      BOOST_REQUIRE(doesPointBelongToRightTriangularPrismWithInfinitHeight(
         a, b, c, p1yes)
      );

      const dt::Pointf3 p2no(0.0f, 10.0f, 0.0f);
      const dt::Pointf3 p2yes(0.0f, -0.2f, 0.2f);
      BOOST_REQUIRE(!doesPointBelongToRightTriangularPrismWithInfinitHeight(
         a, b, c, p2no)
      );
      BOOST_REQUIRE(doesPointBelongToRightTriangularPrismWithInfinitHeight(
         a, b, c, p2yes)
      );
   }

   {
      const dt::Pointf3 a(-0.806522012f, 1.25050914f, 0.519089997f);
      const dt::Pointf3 b(0.0f, 0.5f, 0.0f);
      const dt::Pointf3 c(-1.0281533f, 0.592432618f, -0.656711996f);

      const dt::Pointf3 pyes(-0.947010934f, 0.867071152f, -0.653006077f);
      BOOST_REQUIRE(!doesPointBelongToRightTriangularPrismWithInfinitHeight(
         a, b, c, pyes)
      );
   }
}


BOOST_AUTO_TEST_SUITE_END()
