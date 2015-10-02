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


#include "MemoryModification.hpp"


using namespace mesh;


/***************************************************************************
 *   MemoryModification class test                                         *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libmesh_MemoryModification)


BOOST_AUTO_TEST_CASE(test_insertMemoryBlock)
{
   MemoryModification memMod;

   memMod.insertMemoryBlock(0, 4);
   // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23
   // '---------'

   memMod.insertMemoryBlock(5, 5);
   // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23
   // '---------'    '------------'

   memMod.insertMemoryBlock(10, 5);
   // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23
   // '---------'    '---------------------------'

   memMod.insertMemoryBlock(12, 6);
   // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23
   // '---------'    '------------------------------------'

   MemoryModification::const_iterator it = memMod.begin();
   MemoryModification::const_iterator ite = memMod.end();

   BOOST_REQUIRE(it != ite && it->lower() == 0 && it->upper() == 4);
   ++it;
   BOOST_REQUIRE(it != ite && it->lower() == 5 && it->upper() == 18);
   ++it;
   BOOST_REQUIRE(it == ite);
}


BOOST_AUTO_TEST_SUITE_END()
