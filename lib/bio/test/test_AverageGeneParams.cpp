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


#include "AverageGeneParams.hpp"


using namespace bio;


/***************************************************************************
 *   AverageGeneParams class test                                          *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libbio_AverageGeneParams)


BOOST_AUTO_TEST_CASE(test_constructor)
{
   {
      AverageGeneI16Param p;
      BOOST_REQUIRE(!p.value());
   }

   {
      AverageGeneU16Param p;
      BOOST_REQUIRE(!p.value());
   }
}


BOOST_AUTO_TEST_CASE(test_add)
{
   {
      AverageGeneI16Param p;
      p.add(dt::Int16(4000));
      BOOST_REQUIRE(p.value() == dt::Int16(4000));
      p.add(dt::Int16(-8000));
      BOOST_REQUIRE(p.value() == dt::Int16(-2000));
      p.add_nop();
      p.add_nop();
      BOOST_REQUIRE(p.value() == dt::Int16(-2000));
      p.add_nop();
      BOOST_REQUIRE(!p.value());
      p.add(dt::Int16(13000));
      BOOST_REQUIRE(p.value() == dt::Int16(3000));
   }

   {
      AverageGeneU16Param p;
      p.add(dt::UInt16(4000));
      BOOST_REQUIRE(p.value() == dt::UInt16(4000));
      p.add(dt::UInt16(8000));
      BOOST_REQUIRE(p.value() == dt::UInt16(6000));
      p.add_nop();
      p.add_nop();
      BOOST_REQUIRE(p.value() == dt::UInt16(6000));
      p.add_nop();
      BOOST_REQUIRE(!p.value());
      p.add(dt::UInt16(9000));
      BOOST_REQUIRE(p.value() == dt::UInt16(7000));
   }
}


BOOST_AUTO_TEST_SUITE_END()
