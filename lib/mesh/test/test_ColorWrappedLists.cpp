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


#include "ColorWrappedLists.hpp"
#include "MemoryModification.hpp"


using namespace mesh;


static char _modAccountingFailedMsg[] = "Modified colors accounting failed";


/***************************************************************************
 *   ColorWrappedLists class test                                          *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libmesh_ColorWrappedLists)


BOOST_AUTO_TEST_CASE(test_constructor)
{
   ColorWrappedLists lists1x4(1, ColorWrappedLists::CC_RGBA);
   int target1x4[] = {
      -1, -1, -1, -1
   };

   ColorWrappedLists lists5x3(5, ColorWrappedLists::CC_RGB);
   int target5x3[] = {
      -1, -1, 1,
      -1, -1, 2,
      -1, -1, 3,
      -1, -1, 4,
      -1, -1, -1
   };

   struct TestCase
   {
      ColorWrappedLists *lists;
      int *target;
      size_t targetSize;
   };

   TestCase testCases[] = {
      {&lists1x4, &target1x4[0], sizeof(target1x4)},
      {&lists5x3, &target5x3[0], sizeof(target5x3)}
   };

   for (size_t i = 0; i < sizeof(testCases) / sizeof(TestCase); i ++)
   {
      TestCase &tc = testCases[i];

      if (memcmp(tc.lists->data(), tc.target, tc.targetSize))
      {
         BOOST_ERROR("Newly created ColorWrappedLists(" <<
            tc.lists->colorCount() << ", " << tc.lists->colorComponents() <<
            ") contains wrong data"
         );
      }

      int firstFreeColor = tc.lists->firstFreeColor();
      if (firstFreeColor)
      {
         BOOST_ERROR("Newly created ColorWrappedLists(" <<
            tc.lists->colorCount() << ", " << tc.lists->colorComponents() <<
            ") has first free color " << firstFreeColor << "instead of 0"
         );
      }

      BOOST_CHECK(tc.lists->memoryModification()->empty());
   }
}


BOOST_AUTO_TEST_CASE(test_createFirstColor)
{
   ColorWrappedLists lists(5, ColorWrappedLists::CC_RGB);

   lists.startMemoryModificationLogging();
   ColorWrappedLists::iterator it = lists.createFirstColor();
   it.set(0, 10);
   it.set(1, 11);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.color() == 0);
   BOOST_REQUIRE(lists.firstFreeColor() == 1);
   lists.stopMemoryModificationLogging();

   MemoryModification m1;
   m1.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m1)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target1[] = {
      10, 11, -1,
      -1, -1, 2,
      -1, -1, 3,
      -1, -1, 4,
      -1, -1, -1
   };

   if (memcmp(lists.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after creating first color (10, 11)");
   }

   lists.startMemoryModificationLogging();
   it = lists.createFirstColor();
   it.set(0, 20);
   it.set(1, 21);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.color() == 1);
   BOOST_REQUIRE(lists.firstFreeColor() == 2);
   it = lists.insertAfter(it);
   it.set(0, 22);
   it.set(1, 23);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.color() == 2);
   BOOST_REQUIRE(lists.firstFreeColor() == 3);
   it = lists.insertAfter(it);
   it.set(0, 24);
   it.set(1, 25);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.color() == 3);
   BOOST_REQUIRE(lists.firstFreeColor() == 4);
   lists.stopMemoryModificationLogging();

   MemoryModification m2;
   m2.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   m2.insertArrayElement(2, sizeof(GLint) * lists.colorComponents());
   m2.insertArrayElement(3, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m2)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target2[] = {
      10, 11, -1,
      20, 21, 2,
      22, 23, 3,
      24, 25, -1,
      -1, -1, -1
   };

   if (memcmp(lists.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after creating first colors (20, 21), (22, 23), (24, 25)");
   }

   lists.startMemoryModificationLogging();
   it = lists.createFirstColor();
   it.set(0, 30);
   it.set(1, 31);
   BOOST_REQUIRE(it.isValid());
   BOOST_REQUIRE(it.color() == 4);
   BOOST_REQUIRE(lists.firstFreeColor() == -1);
   lists.stopMemoryModificationLogging();

   MemoryModification m3;
   m3.insertArrayElement(4, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m3)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target3[] = {
      10, 11, -1,
      20, 21, 2,
      22, 23, 3,
      24, 25, -1,
      30, 31, -1
   };

   if (memcmp(lists.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after creating first color (30, 31)");
   }

   lists.startMemoryModificationLogging();
   it = lists.createFirstColor();
   BOOST_REQUIRE(!it.isValid());
   lists.stopMemoryModificationLogging();

   if (!lists.memoryModification()->empty())
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   if (memcmp(lists.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after creating first color when container is full");
   }
}


BOOST_AUTO_TEST_CASE(test_removeFirst)
{
   ColorWrappedLists lists(5, ColorWrappedLists::CC_RGB);

   ColorWrappedLists::iterator it = lists.createFirstColor();
   GLint firstColor1 = it.color();
   it.set(0, 0);
   it.set(1, 1);

   it = lists.createFirstColor();
   GLint firstColor2 = it.color();
   it.set(0, 20);
   it.set(1, 21);

   it = lists.insertAfter(it);
   it.set(0, 22);
   it.set(1, 23);

   it = lists.createFirstColor();
   GLint firstColor3 = it.color();
   it.set(0, 30);
   it.set(1, 31);

   it = lists.insertAfter(it);
   it.set(0, 32);
   it.set(1, 33);

   lists.startMemoryModificationLogging();

   firstColor2 = lists.removeFirst(firstColor2);
   BOOST_REQUIRE(firstColor2 == 2);
   BOOST_REQUIRE(lists.firstFreeColor() == 1);

   MemoryModification m1;
   m1.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m1)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target21[] = {
      0, 1, -1,
      -1, -1, -1,
      22, 23, -1,
      30, 31, 4,
      32, 33, -1
   };

   if (memcmp(lists.data(), target21, sizeof(target21)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing first color (20, 21)");
   }

   firstColor1 = lists.removeFirst(firstColor1);
   BOOST_REQUIRE(firstColor1 == -1);
   BOOST_REQUIRE(lists.firstFreeColor() == 0);

   MemoryModification m2;
   m2.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   m2.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m2)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target1[] = {
      -1, -1, 1,
      -1, -1, -1,
      22, 23, -1,
      30, 31, 4,
      32, 33, -1
   };

   if (memcmp(lists.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing first color (0, 1)");
   }

   firstColor3 = lists.removeFirst(firstColor3);
   BOOST_REQUIRE(firstColor3 == 4);
   BOOST_REQUIRE(lists.firstFreeColor() == 3);

   MemoryModification m3;
   m3.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   m3.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   m3.insertArrayElement(3, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m3)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target31[] = {
      -1, -1, 1,
      -1, -1, -1,
      22, 23, -1,
      -1, -1, 0,
      32, 33, -1
   };

   if (memcmp(lists.data(), target31, sizeof(target31)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing first color (30, 31)");
   }

   firstColor3 = lists.removeFirst(firstColor3);
   BOOST_REQUIRE(firstColor3 == -1);
   BOOST_REQUIRE(lists.firstFreeColor() == 4);

   MemoryModification m4;
   m4.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   m4.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   m4.insertArrayElement(3, sizeof(GLint) * lists.colorComponents());
   m4.insertArrayElement(4, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m4)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target32[] = {
      -1, -1, 1,
      -1, -1, -1,
      22, 23, -1,
      -1, -1, 0,
      -1, -1, 3
   };

   if (memcmp(lists.data(), target32, sizeof(target32)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing first color (32, 33)");
   }

   firstColor2 = lists.removeFirst(firstColor2);
   BOOST_REQUIRE(firstColor2 == -1);
   BOOST_REQUIRE(lists.firstFreeColor() == 2);

   MemoryModification m5;
   m5.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   m5.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   m5.insertArrayElement(2, sizeof(GLint) * lists.colorComponents());
   m5.insertArrayElement(3, sizeof(GLint) * lists.colorComponents());
   m5.insertArrayElement(4, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m5)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target22[] = {
      -1, -1, 1,
      -1, -1, -1,
      -1, -1, 4,
      -1, -1, 0,
      -1, -1, 3
   };

   if (memcmp(lists.data(), target22, sizeof(target22)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing first color (22, 23)");
   }
}


BOOST_AUTO_TEST_CASE(test_removeAfter)
{
   ColorWrappedLists lists(5, ColorWrappedLists::CC_RGB);

   ColorWrappedLists::iterator it = lists.createFirstColor();
   GLint firstColor = it.color();
   it.set(0, 0);
   it.set(1, 1);

   it = lists.insertAfter(it);
   it.set(0, 2);
   it.set(1, 3);

   it = lists.insertAfter(it);
   it.set(0, 4);
   it.set(1, 5);

   it = lists.insertAfter(it);
   it.set(0, 6);
   it.set(1, 7);

   it = lists.createFirstColor();
   it.set(0, 10);
   it.set(1, 11);

   lists.startMemoryModificationLogging();

   it = lists.removeAfter(lists.begin(firstColor));
   BOOST_REQUIRE(it.color() == 2 && it.get(0) == 4 && it.get(1) == 5);
   BOOST_REQUIRE(lists.firstFreeColor() == 1);

   MemoryModification m1;
   m1.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   m1.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m1)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target1[] = {
      0, 1, 2,
      -1, -1, -1,
      4, 5, 3,
      6, 7, -1,
      10, 11, -1
   };

   if (memcmp(lists.data(), target1, sizeof(target1)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing color (2, 3)");
   }

   it = lists.removeAfter(lists.begin(firstColor));
   BOOST_REQUIRE(it.color() == 3 && it.get(0) == 6 && it.get(1) == 7);
   BOOST_REQUIRE(lists.firstFreeColor() == 2);

   MemoryModification m2;
   m2.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   m2.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   m2.insertArrayElement(2, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m2)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target2[] = {
      0, 1, 3,
      -1, -1, -1,
      -1, -1, 1,
      6, 7, -1,
      10, 11, -1
   };

   if (memcmp(lists.data(), target2, sizeof(target2)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing color (4, 5)");
   }

   it = lists.removeAfter(lists.begin(firstColor));
   BOOST_REQUIRE(!it.isValid());
   BOOST_REQUIRE(lists.firstFreeColor() == 3);

   MemoryModification m3;
   m3.insertArrayElement(0, sizeof(GLint) * lists.colorComponents());
   m3.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   m3.insertArrayElement(2, sizeof(GLint) * lists.colorComponents());
   m3.insertArrayElement(3, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m3)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target3[] = {
      0, 1, -1,
      -1, -1, -1,
      -1, -1, 1,
      -1, -1, 2,
      10, 11, -1
   };

   if (memcmp(lists.data(), target3, sizeof(target3)))
   {
      BOOST_FAIL("ColorWrappedLists(5, 3) contains wrong data "
         "after removing color (6, 7)");
   }
}


BOOST_AUTO_TEST_CASE(test_removeAll)
{
   ColorWrappedLists lists(4, ColorWrappedLists::CC_RGB);

   ColorWrappedLists::iterator it = lists.createFirstColor();
   it.set(0, 0);
   it.set(1, 1);

   it = lists.createFirstColor();
   GLint firstColor = it.color();
   it.set(0, 10);
   it.set(1, 11);

   it = lists.insertAfter(it);
   it.set(0, 12);
   it.set(1, 13);

   it = lists.createFirstColor();
   it.set(0, 20);
   it.set(1, 21);

   lists.startMemoryModificationLogging();

   lists.removeAll(firstColor);
   BOOST_REQUIRE(lists.firstFreeColor() == 2);

   MemoryModification m1;
   m1.insertArrayElement(1, sizeof(GLint) * lists.colorComponents());
   m1.insertArrayElement(2, sizeof(GLint) * lists.colorComponents());
   if (*lists.memoryModification() != m1)
   {
      BOOST_FAIL(_modAccountingFailedMsg);
   }

   int target[] = {
      0, 1, -1,
      -1, -1, -1,
      -1, -1, 1,
      20, 21, -1
   };

   if (memcmp(lists.data(), target, sizeof(target)))
   {
      BOOST_FAIL("ColorWrappedLists(4, 3) contains wrong data "
         "after removing colors (10, 11) and (12, 13)");
   }
}


BOOST_AUTO_TEST_SUITE_END()
