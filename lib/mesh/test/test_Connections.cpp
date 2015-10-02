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


#include <cstring>


#include <boost/test/unit_test.hpp>


#include "Connections.hpp"


using namespace mesh;


/***************************************************************************
 *   Connections class test                                                *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libmesh_Connections)


BOOST_AUTO_TEST_CASE(test_create)
{
   Connections connections(5);
   GLint c;

   const Connections::Entry entries1[] = {
      Connections::Entry(dt::VertexId(0), dt::EdgeId(1), 0)
   };
   c = connections.create(entries1, 1);
   BOOST_REQUIRE(c == 0);

   int target1[] = {
      0, 1, 0, -1,
      -1, -1, -1, 2,
      -1, -1, -1, 3,
      -1, -1, -1, 4,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("Connections(5) contains wrong data after creating entry "
         "(0, 1, 0)");
   }

   const Connections::Entry entries2[] = {
      Connections::Entry(dt::VertexId(1), dt::EdgeId(3), 1),
      Connections::Entry(dt::VertexId(2), dt::EdgeId(4), 0)
   };
   c = connections.create(entries2, 2);
   BOOST_REQUIRE(c == 1);

   int target2[] = {
      0, 1, 0, -1,
      1, 3, 1, 2,
      2, 4, 0, -1,
      -1, -1, -1, 4,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("Connections(5) contains wrong data after creating entries "
         "(1, 3, 1), (2, 4, 0)");
   }

   const Connections::Entry entries3[] = {
      Connections::Entry(dt::VertexId(5), dt::EdgeId(7), 0),
      Connections::Entry(dt::VertexId(6), dt::EdgeId(8), 1)
   };
   c = connections.create(entries3, 2);
   BOOST_REQUIRE(c == 3);

   int target3[] = {
      0, 1, 0, -1,
      1, 3, 1, 2,
      2, 4, 0, -1,
      5, 7, 0, 4,
      6, 8, 1, -1
   };

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(5) contains wrong data after creating entries "
         "(5, 7, 0), (6, 8, 1)");
   }
}


BOOST_AUTO_TEST_CASE(test_insertExternal)
{
   Connections connections(9);
   bool inserted;

   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 0),
      Connections::Entry(dt::VertexId(30), dt::EdgeId(300), 0),
      Connections::Entry(dt::VertexId(40), dt::EdgeId(400), 0),
      Connections::Entry(dt::VertexId(50), dt::EdgeId(500), 1),
      Connections::Entry(dt::VertexId(60), dt::EdgeId(600), 1)
   };
   GLint c = connections.create(entries, 6);

   inserted = connections.insertExternal(
      c,
      dt::VertexId(20),
      dt::VertexId(10),
      dt::VertexId(25), dt::EdgeId(250),
      true
   );
   BOOST_REQUIRE(!inserted);

   int target1[] = {
      10, 100, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      50, 500, 1, 5,
      60, 600, 1, -1,
      -1, -1, -1, 7,
      -1, -1, -1, 8,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("Connections(9) contains wrong data after inserting "
         "25 between 20 and 10 with exact order search turned on");
   }

   inserted = connections.insertExternal(
      c,
      dt::VertexId(20),
      dt::VertexId(10),
      dt::VertexId(25), dt::EdgeId(250),
      false
   );
   BOOST_REQUIRE(inserted);

   int target2[] = {
      10, 100, 0, 6,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      50, 500, 1, 5,
      60, 600, 1, -1,
      25, 250, 0, 1,
      -1, -1, -1, 8,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("Connections(9) contains wrong data after inserting "
         "25 between 20 and 10 with exact order search turned off");
   }

   inserted = connections.insertExternal(
      c,
      dt::VertexId(40),
      dt::VertexId(10),
      dt::VertexId(45), dt::EdgeId(450),
      true
   );
   BOOST_REQUIRE(inserted);

   int target3[] = {
      10, 100, 0, 6,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 7,
      50, 500, 1, 5,
      60, 600, 1, -1,
      25, 250, 0, 1,
      45, 450, 0, 4,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(9) contains wrong data after inserting "
         "45 between 40 and 10 with exact order search turned on");
   }

   inserted = connections.insertExternal(
      c,
      dt::VertexId(10),
      dt::VertexId(30),
      dt::VertexId(15), dt::EdgeId(150),
      true
   );
   BOOST_REQUIRE(!inserted);

   inserted = connections.insertExternal(
      c,
      dt::VertexId(10),
      dt::VertexId(30),
      dt::VertexId(15), dt::EdgeId(150),
      false
   );
   BOOST_REQUIRE(!inserted);

   inserted = connections.insertExternal(
      c,
      dt::VertexId(45),
      dt::VertexId(50),
      dt::VertexId(47), dt::EdgeId(470),
      true
   );
   BOOST_REQUIRE(!inserted);

   inserted = connections.insertExternal(
      c,
      dt::VertexId(45),
      dt::VertexId(50),
      dt::VertexId(47), dt::EdgeId(470),
      false
   );
   BOOST_REQUIRE(!inserted);

   inserted = connections.insertExternal(
      c,
      dt::VertexId(50),
      dt::VertexId(60),
      dt::VertexId(55), dt::EdgeId(550),
      true
   );
   BOOST_REQUIRE(!inserted);

   inserted = connections.insertExternal(
      c,
      dt::VertexId(50),
      dt::VertexId(60),
      dt::VertexId(55), dt::EdgeId(550),
      false
   );
   BOOST_REQUIRE(!inserted);

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(9) contains wrong data after a bunch of "
         "unsuccessful insertions");
   }
}


BOOST_AUTO_TEST_CASE(test_insertInternal)
{
   Connections connections(6);
   bool inserted;

   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 0),
      Connections::Entry(dt::VertexId(30), dt::EdgeId(300), 0)
   };
   GLint c = connections.create(entries, 3);

   inserted = connections.insertInternal(c, dt::VertexId(40), dt::EdgeId(400));
   BOOST_REQUIRE(inserted);

   int target1[] = {
      10, 100, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 1, -1,
      -1, -1, -1, 5,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after inserting "
         "400 internal edge vith 40 vertex");
   }

   inserted = connections.insertInternal(c, dt::VertexId(50), dt::EdgeId(500));
   BOOST_REQUIRE(inserted);

   int target2[] = {
      10, 100, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 4,
      40, 400, 1, -1,
      50, 500, 1, 3,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after inserting "
         "500 internal edge vith 50 vertex");
   }

   inserted = connections.insertInternal(c, dt::VertexId(60), dt::EdgeId(600));
   BOOST_REQUIRE(inserted);

   int target3[] = {
      10, 100, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 5,
      40, 400, 1, -1,
      50, 500, 1, 3,
      60, 600, 1, 4
   };

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after inserting "
         "600 internal edge vith 60 vertex");
   }

   inserted = connections.insertInternal(c, dt::VertexId(70), dt::EdgeId(700));
   BOOST_REQUIRE(!inserted);

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after inserting "
         "700 internal edge vith 70 vertex");
   }

   Connections intConnections(3);

   const Connections::Entry intEntries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 1),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 1)
   };
   GLint intC = intConnections.create(intEntries, 2);

   inserted = intConnections.insertInternal(
      intC,
      dt::VertexId(30),
      dt::EdgeId(300)
   );
   BOOST_REQUIRE(inserted);

   int intTarget1[] = {
      10, 100, 1, 2,
      20, 200, 1, -1,
      30, 300, 1, 1
   };

   if (memcmp(intConnections.data(), intTarget1, sizeof(intTarget1)))
   {
      BOOST_FAIL("Connections(3) contains wrong data after inserting "
         "300 internal edge vith 30 vertex");
   }

   inserted = intConnections.insertInternal(
      intC,
      dt::VertexId(40),
      dt::EdgeId(400)
   );
   BOOST_REQUIRE(!inserted);

   if (memcmp(intConnections.data(), intTarget1, sizeof(intTarget1)))
   {
      BOOST_FAIL("Connections(3) contains wrong data after inserting "
         "400 internal edge vith 40 vertex");
   }
}


BOOST_AUTO_TEST_CASE(test_replace)
{
   Connections connections(6);
   bool replaced;

   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 0),
      Connections::Entry(dt::VertexId(30), dt::EdgeId(300), 0),
      Connections::Entry(dt::VertexId(40), dt::EdgeId(400), 0),
      Connections::Entry(dt::VertexId(50), dt::EdgeId(500), 1),
      Connections::Entry(dt::VertexId(60), dt::EdgeId(600), 1)
   };
   GLint c = connections.create(entries, 6);

   replaced = connections.replace(c, dt::VertexId(11), dt::VertexId(15));
   BOOST_REQUIRE(!replaced);

   int target1[] = {
      10, 100, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      50, 500, 1, 5,
      60, 600, 1, -1
   };

   if (memcmp(connections.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after replacing "
         "11 with 15");
   }

   replaced = connections.replace(c, dt::VertexId(10), dt::VertexId(15));
   BOOST_REQUIRE(replaced);

   int target2[] = {
      15, 100, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      50, 500, 1, 5,
      60, 600, 1, -1
   };

   if (memcmp(connections.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after replacing "
         "10 with 15");
   }

   replaced = connections.replace(c, dt::VertexId(20), dt::VertexId(25));
   BOOST_REQUIRE(replaced);

   int target3[] = {
      15, 100, 0, 1,
      25, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      50, 500, 1, 5,
      60, 600, 1, -1
   };

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after replacing "
         "20 with 25");
   }

   replaced = connections.replace(c, dt::VertexId(50), dt::VertexId(55));
   BOOST_REQUIRE(replaced);

   int target4[] = {
      15, 100, 0, 1,
      25, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      55, 500, 1, 5,
      60, 600, 1, -1
   };

   if (memcmp(connections.data(), target4, sizeof(target4)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after replacing "
         "50 with 55");
   }

   replaced = connections.replace(c, dt::VertexId(60), dt::VertexId(65));
   BOOST_REQUIRE(replaced);

   int target5[] = {
      15, 100, 0, 1,
      25, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      55, 500, 1, 5,
      65, 600, 1, -1
   };

   if (memcmp(connections.data(), target5, sizeof(target5)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after replacing "
         "60 with 65");
   }
}


BOOST_AUTO_TEST_CASE(test_replaceEdge)
{
   Connections connections(6);
   bool replaced;

   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 0),
      Connections::Entry(dt::VertexId(30), dt::EdgeId(300), 0),
      Connections::Entry(dt::VertexId(40), dt::EdgeId(400), 0),
      Connections::Entry(dt::VertexId(50), dt::EdgeId(500), 1),
      Connections::Entry(dt::VertexId(60), dt::EdgeId(600), 1)
   };
   GLint c = connections.create(entries, 6);

   replaced = connections.replaceEdge(c, 700, 750);
   BOOST_REQUIRE(!replaced);

   int target1[] = {
      10, 100, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      50, 500, 1, 5,
      60, 600, 1, -1
   };

   if (memcmp(connections.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after replacing "
         "700 with 750");
   }

   replaced = connections.replaceEdge(c, 100, 150);
   BOOST_REQUIRE(replaced);

   int target2[] = {
      10, 150, 0, 1,
      20, 200, 0, 2,
      30, 300, 0, 3,
      40, 400, 0, 4,
      50, 500, 1, 5,
      60, 600, 1, -1
   };

   if (memcmp(connections.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after replacing "
         "100 with 150");
   }
}


BOOST_AUTO_TEST_CASE(test_convertToInternal)
{
   Connections connections(6);
   boost::optional<GLint> newFirstColor;

   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 0),
      Connections::Entry(dt::VertexId(30), dt::EdgeId(300), 0)
   };
   GLint c = connections.create(entries, 3);

   newFirstColor = connections.convertToInternal(c, dt::VertexId(10));
   BOOST_REQUIRE(newFirstColor && *newFirstColor == 1);
   c = *newFirstColor;

   int target1[] = {
      10, 100, 1, -1,
      20, 200, 0, 2,
      30, 300, 0, 0,
      -1, -1, -1, 4,
      -1, -1, -1, 5,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after converting "
         "100 edge vith 10 vertex to internal");
   }

   newFirstColor = connections.convertToInternal(c, dt::VertexId(30));
   BOOST_REQUIRE(newFirstColor && *newFirstColor == c);

   int target2[] = {
      10, 100, 1, -1,
      20, 200, 0, 2,
      30, 300, 1, 0,
      -1, -1, -1, 4,
      -1, -1, -1, 5,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after converting "
         "300 edge vith 30 vertex to internal");
   }

   newFirstColor = connections.convertToInternal(c, dt::VertexId(40));
   BOOST_REQUIRE(!newFirstColor);

   if (memcmp(connections.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after converting "
         "edge vith 40 vertex to internal");
   }

   newFirstColor = connections.convertToInternal(c, dt::VertexId(20));
   BOOST_REQUIRE(newFirstColor && *newFirstColor == 2);
   c = *newFirstColor;

   int target3[] = {
      10, 100, 1, -1,
      20, 200, 1, 0,
      30, 300, 1, 1,
      -1, -1, -1, 4,
      -1, -1, -1, 5,
      -1, -1, -1, -1
   };

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after converting "
         "200 edge vith 20 vertex to internal");
   }

   newFirstColor = connections.convertToInternal(c, dt::VertexId(10));
   BOOST_REQUIRE(!newFirstColor);

   if (memcmp(connections.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("Connections(6) contains wrong data after converting "
         "100 edge vith 10 vertex to internal");
   }

   // TODO: Test for Connections(3);
}


BOOST_AUTO_TEST_CASE(test_findEdge)
{
   Connections connections(3);

   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 0),
      Connections::Entry(dt::VertexId(30), dt::EdgeId(300), 1)
   };
   GLint c = connections.create(entries, 3);

   BOOST_REQUIRE(connections.findEdge(c, dt::VertexId(10)) == 100);
   BOOST_REQUIRE(connections.findEdge(c, dt::VertexId(20)) == 200);
   BOOST_REQUIRE(connections.findEdge(c, dt::VertexId(30)) == 300);
   BOOST_REQUIRE(connections.findEdge(c, dt::VertexId(40)) == -1);
}


BOOST_AUTO_TEST_CASE(test_findFirst)
{
   Connections cn(3);

   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(1), dt::EdgeId(10), 0),
      Connections::Entry(dt::VertexId(2), dt::EdgeId(20), 0),
      Connections::Entry(dt::VertexId(3), dt::EdgeId(30), 1)
   };
   GLint c = cn.create(entries, 3);

   Connections::iterator it0 = cn.begin(c);
   Connections::iterator it1 = ++cn.begin(c);
   Connections::iterator it2 = ++(++cn.begin(c));

   BOOST_REQUIRE(cn.findFirst(c, 1, Connections::VT_VERTEX) == it0);
   BOOST_REQUIRE(cn.findFirst(c, 10, Connections::VT_EDGE) == it0);
   BOOST_REQUIRE(cn.findFirst(c, 0, Connections::VT_INTERNAL) == it0);
   BOOST_REQUIRE(cn.findFirst(c, 2, Connections::VT_VERTEX) == it1);
   BOOST_REQUIRE(cn.findFirst(c, 20, Connections::VT_EDGE) == it1);
   BOOST_REQUIRE(cn.findFirst(c, 3, Connections::VT_VERTEX) == it2);
   BOOST_REQUIRE(cn.findFirst(c, 30, Connections::VT_EDGE) == it2);
   BOOST_REQUIRE(cn.findFirst(c, 1, Connections::VT_INTERNAL) == it2);
   BOOST_REQUIRE(!cn.findFirst(c, 4, Connections::VT_VERTEX).isValid());
   BOOST_REQUIRE(!cn.findFirst(c, 4, Connections::VT_EDGE).isValid());
   BOOST_REQUIRE(!cn.findFirst(c, 4, Connections::VT_INTERNAL).isValid());
}


BOOST_AUTO_TEST_CASE(test_getPrevExternalVertex)
{
   Connections cn(20);
   Connections::const_iterator it;

   const Connections::Entry entries1[] = {
      Connections::Entry(dt::VertexId(1), dt::EdgeId(10), 1)
   };
   GLint c1 = cn.create(entries1, 1);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c1, dt::VertexId(1)).isValid());
   BOOST_REQUIRE(!cn.getPrevExternalVertex(c1, dt::VertexId(2)).isValid());

   const Connections::Entry entries2[] = {
      Connections::Entry(dt::VertexId(2), dt::EdgeId(20), 0)
   };
   GLint c2 = cn.create(entries2, 1);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c2, dt::VertexId(1)).isValid());
   BOOST_REQUIRE(!cn.getPrevExternalVertex(c2, dt::VertexId(2)).isValid());

   const Connections::Entry entries3[] = {
      Connections::Entry(dt::VertexId(3), dt::EdgeId(30), 0),
      Connections::Entry(dt::VertexId(4), dt::EdgeId(40), 1)
   };
   GLint c3 = cn.create(entries3, 2);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c3, dt::VertexId(2)).isValid());
   BOOST_REQUIRE(!cn.getPrevExternalVertex(c3, dt::VertexId(3)).isValid());
   BOOST_REQUIRE(!cn.getPrevExternalVertex(c3, dt::VertexId(4)).isValid());

   const Connections::Entry entries4[] = {
      Connections::Entry(dt::VertexId(5), dt::EdgeId(50), 0),
      Connections::Entry(dt::VertexId(6), dt::EdgeId(60), 0)
   };
   GLint c4 = cn.create(entries4, 2);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c4, dt::VertexId(4)).isValid());
   it = cn.getPrevExternalVertex(c4, dt::VertexId(5));
   BOOST_REQUIRE(it == ++cn.begin(c4));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 6);
   it = cn.getPrevExternalVertex(c4, dt::VertexId(6));
   BOOST_REQUIRE(it == cn.begin(c4));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 5);

   const Connections::Entry entries5[] = {
      Connections::Entry(dt::VertexId(7), dt::EdgeId(70), 0),
      Connections::Entry(dt::VertexId(8), dt::EdgeId(80), 0),
      Connections::Entry(dt::VertexId(9), dt::EdgeId(90), 1)
   };
   GLint c5 = cn.create(entries5, 3);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c5, dt::VertexId(6)).isValid());
   BOOST_REQUIRE(!cn.getPrevExternalVertex(c5, dt::VertexId(9)).isValid());
   it = cn.getPrevExternalVertex(c5, dt::VertexId(7));
   BOOST_REQUIRE(it == ++cn.begin(c5));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 8);
   it = cn.getPrevExternalVertex(c5, dt::VertexId(8));
   BOOST_REQUIRE(it == cn.begin(c5));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 7);

   const Connections::Entry entries6[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(11), dt::EdgeId(110), 0),
      Connections::Entry(dt::VertexId(12), dt::EdgeId(120), 0)
   };
   GLint c6 = cn.create(entries6, 3);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c6, dt::VertexId(9)).isValid());
   it = cn.getPrevExternalVertex(c6, dt::VertexId(10));
   BOOST_REQUIRE(it == ++(++cn.begin(c6)));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 12);
   it = cn.getPrevExternalVertex(c6, dt::VertexId(11));
   BOOST_REQUIRE(it == cn.begin(c6));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 10);
   it = cn.getPrevExternalVertex(c6, dt::VertexId(12));
   BOOST_REQUIRE(it == ++cn.begin(c6));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 11);

   const Connections::Entry entries7[] = {
      Connections::Entry(dt::VertexId(13), dt::EdgeId(13), 0),
      Connections::Entry(dt::VertexId(14), dt::EdgeId(140), 0),
      Connections::Entry(dt::VertexId(15), dt::EdgeId(150), 0),
      Connections::Entry(dt::VertexId(16), dt::EdgeId(160), 1)
   };
   GLint c7 = cn.create(entries7, 4);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c7, dt::VertexId(12)).isValid());
   BOOST_REQUIRE(!cn.getPrevExternalVertex(c7, dt::VertexId(16)).isValid());
   it = cn.getPrevExternalVertex(c7, dt::VertexId(13));
   BOOST_REQUIRE(it == ++(++cn.begin(c7)));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 15);
   it = cn.getPrevExternalVertex(c7, dt::VertexId(14));
   BOOST_REQUIRE(it == cn.begin(c7));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 13);
   it = cn.getPrevExternalVertex(c7, dt::VertexId(15));
   BOOST_REQUIRE(it == ++cn.begin(c7));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 14);

   const Connections::Entry entries8[] = {
      Connections::Entry(dt::VertexId(17), dt::EdgeId(170), 0),
      Connections::Entry(dt::VertexId(17), dt::EdgeId(170), 0),
      Connections::Entry(dt::VertexId(18), dt::EdgeId(180), 0),
      Connections::Entry(dt::VertexId(18), dt::EdgeId(180), 0)
   };
   GLint c8 = cn.create(entries8, 4);

   BOOST_REQUIRE(!cn.getPrevExternalVertex(c8, dt::VertexId(16)).isValid());
   it = cn.getPrevExternalVertex(c8, dt::VertexId(17));
   BOOST_REQUIRE(it == ++(++(++cn.begin(c8))));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 18);
   it = cn.getPrevExternalVertex(c8, dt::VertexId(18));
   BOOST_REQUIRE(it == ++cn.begin(c8));
   BOOST_REQUIRE(it.get(Connections::VT_VERTEX) == 17);
}


BOOST_AUTO_TEST_CASE(test_areExternalVerticesNextToEachOther)
{
   Connections cn(6);
   const Connections::Entry entries[] = {
      Connections::Entry(dt::VertexId(10), dt::EdgeId(100), 0),
      Connections::Entry(dt::VertexId(20), dt::EdgeId(200), 0),
      Connections::Entry(dt::VertexId(30), dt::EdgeId(300), 0),
      Connections::Entry(dt::VertexId(40), dt::EdgeId(400), 0),
      Connections::Entry(dt::VertexId(50), dt::EdgeId(500), 1),
      Connections::Entry(dt::VertexId(60), dt::EdgeId(600), 1)
   };
   const GLint fc = cn.create(entries, 6);

   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(10), dt::VertexId(20)));
   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(20), dt::VertexId(10)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(10), dt::VertexId(30)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(30), dt::VertexId(10)));
   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(20), dt::VertexId(30)));
   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(30), dt::VertexId(20)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(20), dt::VertexId(60)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(60), dt::VertexId(20)));
   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(30), dt::VertexId(40)));
   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(40), dt::VertexId(30)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(20), dt::VertexId(40)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(40), dt::VertexId(20)));
   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(10), dt::VertexId(40)));
   BOOST_REQUIRE(cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(40), dt::VertexId(10)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(40), dt::VertexId(50)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(50), dt::VertexId(40)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(50), dt::VertexId(60)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(60), dt::VertexId(50)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(10), dt::VertexId(60)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(60), dt::VertexId(10)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(10), dt::VertexId(10)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(30), dt::VertexId(30)));
   BOOST_REQUIRE(!cn.areExternalVerticesNextToEachOther(fc,  dt::VertexId(60), dt::VertexId(60)));
}


BOOST_AUTO_TEST_SUITE_END()
