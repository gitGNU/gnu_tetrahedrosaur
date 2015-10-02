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


#include "TrianglesMap.hpp"


using namespace mesh;


/***************************************************************************
 *   TrianglesMap class test                                               *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libmesh_TrianglesMap)


BOOST_AUTO_TEST_CASE(test_findAny)
{
   TrianglesMap map;
   map.insert(Triangle(1, 2, 3), dt::TriangleId(0));
   map.insert(Triangle(2, 3, 4), dt::TriangleId(1));

   boost::optional<dt::TriangleId> i;

   i = map.findAny(Triangle(1, 2, 3));
   BOOST_REQUIRE(i && i->get() == 0);
   i = map.findAny(Triangle(1, 3, 2));
   BOOST_REQUIRE(i && i->get() == 0);
   i = map.findAny(Triangle(2, 1, 3));
   BOOST_REQUIRE(i && i->get() == 0);
   i = map.findAny(Triangle(2, 3, 1));
   BOOST_REQUIRE(i && i->get() == 0);
   i = map.findAny(Triangle(3, 1, 2));
   BOOST_REQUIRE(i && i->get() == 0);
   i = map.findAny(Triangle(3, 2, 1));
   BOOST_REQUIRE(i && i->get() == 0);

   i = map.findAny(Triangle(2, 3, 4));
   BOOST_REQUIRE(i && i->get() == 1);
   i = map.findAny(Triangle(2, 4, 3));
   BOOST_REQUIRE(i && i->get() == 1);
   i = map.findAny(Triangle(3, 2, 4));
   BOOST_REQUIRE(i && i->get() == 1);
   i = map.findAny(Triangle(3, 4, 2));
   BOOST_REQUIRE(i && i->get() == 1);
   i = map.findAny(Triangle(4, 2, 3));
   BOOST_REQUIRE(i && i->get() == 1);
   i = map.findAny(Triangle(4, 3, 2));
   BOOST_REQUIRE(i && i->get() == 1);
}


BOOST_AUTO_TEST_CASE(test_replace_index)
{
   TrianglesMap map;
   map.insert(Triangle(1, 2, 3), dt::TriangleId(0));
   map.insert(Triangle(4, 5, 6), dt::TriangleId(1));
   map.insert(Triangle(9, 7, 1), dt::TriangleId(2));
   map.insert(Triangle(8, 5, 4), dt::TriangleId(3));

   map.replace(Triangle(1, 2, 3), dt::TriangleId(4));
   map.replace(Triangle(4, 6, 5), dt::TriangleId(5));
   map.replace(Triangle(9, 7, 1), dt::TriangleId(6));

   BOOST_REQUIRE(map.size() == 4);

   TrianglesMap::const_iterator it;
   TrianglesMap::const_iterator ite = map.end();

   it = map.find(Triangle(1, 2, 3));
   BOOST_REQUIRE(it != ite && it->second.get() == 4);
   it = map.find(Triangle(4, 5, 6));
   BOOST_REQUIRE(it != ite && it->second.get() == 1);
   it = map.find(Triangle(9, 7, 1));
   BOOST_REQUIRE(it != ite && it->second.get() == 6);
   it = map.find(Triangle(8, 5, 4));
   BOOST_REQUIRE(it != ite && it->second.get() == 3);
}


BOOST_AUTO_TEST_CASE(test_remove)
{
   TrianglesMap map;
   map.insert(Triangle(1, 2, 3), dt::TriangleId(0));
   map.insert(Triangle(4, 5, 6), dt::TriangleId(1));
   map.insert(Triangle(9, 7, 1), dt::TriangleId(2));
   map.insert(Triangle(8, 5, 4), dt::TriangleId(3));

   bool ok = false;
   ok = map.remove(Triangle(1, 2, 3));
   BOOST_REQUIRE(ok);
   ok = map.remove(Triangle(4, 6, 5));
   BOOST_REQUIRE(!ok);
   ok = map.remove(Triangle(9, 7, 1));
   BOOST_REQUIRE(ok);

   BOOST_REQUIRE(map.size() == 2);

   TrianglesMap::const_iterator it;
   TrianglesMap::const_iterator ite = map.end();

   it = map.find(Triangle(4, 5, 6));
   BOOST_REQUIRE(it != ite && it->second.get() == 1);
   it = map.find(Triangle(8, 5, 4));
   BOOST_REQUIRE(it != ite && it->second.get() == 3);
}


BOOST_AUTO_TEST_CASE(test_replaceVertexIndex)
{
   TrianglesMap map;
   map.insert(Triangle(1, 2, 3), dt::TriangleId(0));
   map.insert(Triangle(4, 5, 6), dt::TriangleId(1));
   map.insert(Triangle(9, 7, 1), dt::TriangleId(2));
   map.insert(Triangle(8, 5, 4), dt::TriangleId(3));

   // Replace existing vertex;

   TrianglesMap replacedTriangles = std::move(map.replaceVertexIndex(1, 10));

   BOOST_REQUIRE(map.size() == 4);

   TrianglesMap::const_iterator it;
   TrianglesMap::const_iterator ite = map.end();

   it = map.find(Triangle(10, 2, 3));
   BOOST_REQUIRE(it != ite && it->second.get() == 0);
   it = map.find(Triangle(4, 5, 6));
   BOOST_REQUIRE(it != ite && it->second.get() == 1);
   it = map.find(Triangle(9, 7, 10));
   BOOST_REQUIRE(it != ite && it->second.get() == 2);
   it = map.find(Triangle(8, 5, 4));
   BOOST_REQUIRE(it != ite && it->second.get() == 3);

   BOOST_REQUIRE(replacedTriangles.size() == 2);

   ite = replacedTriangles.end();

   it = replacedTriangles.find(Triangle(10, 2, 3));
   BOOST_REQUIRE(it != ite && it->second.get() == 0);
   it = replacedTriangles.find(Triangle(9, 7, 10));
   BOOST_REQUIRE(it != ite && it->second.get() == 2);

   // Replace missing vertex;

   replacedTriangles = std::move(map.replaceVertexIndex(20, 30));

   BOOST_REQUIRE(map.size() == 4);

   ite = map.end();

   it = map.find(Triangle(10, 2, 3));
   BOOST_REQUIRE(it != ite && it->second.get() == 0);
   it = map.find(Triangle(4, 5, 6));
   BOOST_REQUIRE(it != ite && it->second.get() == 1);
   it = map.find(Triangle(9, 7, 10));
   BOOST_REQUIRE(it != ite && it->second.get() == 2);
   it = map.find(Triangle(8, 5, 4));
   BOOST_REQUIRE(it != ite && it->second.get() == 3);

   BOOST_REQUIRE(replacedTriangles.empty());
}


BOOST_AUTO_TEST_SUITE_END()
