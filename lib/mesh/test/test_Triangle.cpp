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


#include <boost/test/unit_test.hpp>


#include "Triangle.hpp"


using namespace mesh;


/***************************************************************************
 *   Triangle structure test                                               *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libmesh_Triangle)


BOOST_AUTO_TEST_CASE(test_constructor)
{
   Triangle t(1, 2, 3);
   BOOST_REQUIRE(t.a == 1 && t.b == 2 && t.c == 3);
}


BOOST_AUTO_TEST_CASE(test_replaced)
{
   Triangle t0(1, 2, 3);

   Triangle t1(t0.replaced(1, 10));
   BOOST_REQUIRE(t1 == Triangle(10, 2, 3));

   Triangle t2(t0.replaced(2, 20));
   BOOST_REQUIRE(t2 == Triangle(1, 20, 3));

   Triangle t3(t0.replaced(3, 30));
   BOOST_REQUIRE(t3 == Triangle(1, 2, 30));

   Triangle t4(t0.replaced(4, 40));
   BOOST_REQUIRE(t4 == t0);
}


BOOST_AUTO_TEST_CASE(test_combination)
{
   Triangle t0(1, 2, 3);

   Triangle t1(t0.combination(Triangle::C_ABC));
   BOOST_REQUIRE(t1 == t0);

   Triangle t2(t0.combination(Triangle::C_ACB));
   BOOST_REQUIRE(t2 == Triangle(1, 3, 2));

   Triangle t3(t0.combination(Triangle::C_BAC));
   BOOST_REQUIRE(t3 == Triangle(2, 1, 3));

   Triangle t4(t0.combination(Triangle::C_BCA));
   BOOST_REQUIRE(t4 == Triangle(2, 3, 1));

   Triangle t5(t0.combination(Triangle::C_CAB));
   BOOST_REQUIRE(t5 == Triangle(3, 1, 2));

   Triangle t6(t0.combination(Triangle::C_CBA));
   BOOST_REQUIRE(t6 == Triangle(3, 2, 1));
}


BOOST_AUTO_TEST_CASE(test_contains)
{
   Triangle t(1, 2, 3);
   BOOST_REQUIRE(t.contains(1) && t.contains(2) && t.contains(3));
   BOOST_REQUIRE(!t.contains(0) && !t.contains(4) && !t.contains(5));
}


BOOST_AUTO_TEST_CASE(test_third)
{
   Triangle t(1, 2, 3);

   boost::optional<GLuint> v1 = t.third(2, 3);
   BOOST_REQUIRE(v1 && *v1 == 1);

   boost::optional<GLuint> v2 = t.third(1, 3);
   BOOST_REQUIRE(v2 && *v2 == 2);

   boost::optional<GLuint> v3 = t.third(1, 2);
   BOOST_REQUIRE(v3 && *v3 == 3);

   BOOST_REQUIRE(!t.third(0, 1));
   BOOST_REQUIRE(!t.third(4, 0));
}


BOOST_AUTO_TEST_CASE(test_vertexOrder)
{
   Triangle t(1, 2, 3);

   boost::optional<Triangle::VERTEX_ORDER> vo12 = t.vertexOrder(1, 2);
   BOOST_REQUIRE(vo12 && *vo12 == Triangle::VO_DIRECT);

   boost::optional<Triangle::VERTEX_ORDER> vo23 = t.vertexOrder(2, 3);
   BOOST_REQUIRE(vo23 && *vo23 == Triangle::VO_DIRECT);

   boost::optional<Triangle::VERTEX_ORDER> vo31 = t.vertexOrder(3, 1);
   BOOST_REQUIRE(vo31 && *vo31 == Triangle::VO_DIRECT);

   boost::optional<Triangle::VERTEX_ORDER> vo13 = t.vertexOrder(1, 3);
   BOOST_REQUIRE(vo13 && *vo13 == Triangle::VO_INVERSE);

   boost::optional<Triangle::VERTEX_ORDER> vo32 = t.vertexOrder(3, 2);
   BOOST_REQUIRE(vo32 && *vo32 == Triangle::VO_INVERSE);

   boost::optional<Triangle::VERTEX_ORDER> vo21 = t.vertexOrder(2, 1);
   BOOST_REQUIRE(vo21 && *vo21 == Triangle::VO_INVERSE);

   BOOST_REQUIRE(!t.vertexOrder(0, 1));
   BOOST_REQUIRE(!t.vertexOrder(4, 0));
}


BOOST_AUTO_TEST_CASE(test_operatorEqualTo)
{
   BOOST_REQUIRE(Triangle(1, 2, 3) == Triangle(1, 2, 3));
   BOOST_REQUIRE(!(Triangle(1, 2, 3) == Triangle(2, 1, 3)));
   BOOST_REQUIRE(!(Triangle(1, 2, 3) == Triangle(0, 1, 2)));
}


BOOST_AUTO_TEST_CASE(test_operatorLessThan)
{
   BOOST_REQUIRE(Triangle(1, 2, 3) < Triangle(10, 2, 3));
   BOOST_REQUIRE(Triangle(1, 2, 3) < Triangle(1, 20, 3));
   BOOST_REQUIRE(Triangle(1, 2, 3) < Triangle(1, 2, 30));

   BOOST_REQUIRE(!(Triangle(10, 2, 3) < Triangle(1, 2, 3)));
   BOOST_REQUIRE(!(Triangle(1, 20, 3) < Triangle(1, 2, 3)));
   BOOST_REQUIRE(!(Triangle(1, 2, 30) < Triangle(1, 2, 3)));

   BOOST_REQUIRE(!(Triangle(1, 2, 3) < Triangle(1, 2, 3)));
}


BOOST_AUTO_TEST_SUITE_END()
