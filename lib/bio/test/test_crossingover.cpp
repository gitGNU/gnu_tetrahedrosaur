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


#include "crossingover.hpp"


using namespace bio;


/***************************************************************************
 *   crossingover test                                                     *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libbio_crossingover)


BOOST_AUTO_TEST_CASE(test_crossingover)
{
   {
      const std::vector<int> left =  {0, 50, 1, 51, 52, 53, 2, 54, 55};
      const std::vector<int> right = {0, 70, 1, 71, 72, 73, 2, 74, 75};
      const algo::Alignment alignment(left, right);
      const std::vector<uint16_t> distances = {2, 0, 4, 3};
      const algo::UniformUInt16RandomGenerator frg(distances);

      std::pair<std::vector<int>, std::vector<int> > result =
         std::move(crossOver<int>(left, right, alignment, frg));

      BOOST_REQUIRE(result.first.size() == 9);
      BOOST_REQUIRE(result.first[0] == 0);
      BOOST_REQUIRE(result.first[1] == 50);
      BOOST_REQUIRE(result.first[2] == 1);
      BOOST_REQUIRE(result.first[3] == 51);
      BOOST_REQUIRE(result.first[4] == 52);
      BOOST_REQUIRE(result.first[5] == 53);
      BOOST_REQUIRE(result.first[6] == 2);
      BOOST_REQUIRE(result.first[7] == 74);
      BOOST_REQUIRE(result.first[8] == 75);

      BOOST_REQUIRE(result.second.size() == 9);
      BOOST_REQUIRE(result.second[0] == 0);
      BOOST_REQUIRE(result.second[1] == 70);
      BOOST_REQUIRE(result.second[2] == 1);
      BOOST_REQUIRE(result.second[3] == 71);
      BOOST_REQUIRE(result.second[4] == 72);
      BOOST_REQUIRE(result.second[5] == 73);
      BOOST_REQUIRE(result.second[6] == 2);
      BOOST_REQUIRE(result.second[7] == 54);
      BOOST_REQUIRE(result.second[8] == 55);
   }

   {
      const std::vector<int> left =  {50, 51, 0,             1, 52, 53, 54, 3, 55};
      const std::vector<int> right =         {0, 70, 71, 72, 1, 73, 74,     3, 75};
      const algo::Alignment alignment(left, right);
      const std::vector<uint16_t> distances = {4, 1, 2, 4};
      const algo::UniformUInt16RandomGenerator frg(distances);

      std::pair<std::vector<int>, std::vector<int> > result =
         std::move(crossOver<int>(left, right, alignment, frg));

      BOOST_REQUIRE(result.first.size() == 8);
      BOOST_REQUIRE(result.first[0] == 50);
      BOOST_REQUIRE(result.first[1] == 51);
      BOOST_REQUIRE(result.first[2] == 0);
      BOOST_REQUIRE(result.first[3] == 1);
      BOOST_REQUIRE(result.first[4] == 73);
      BOOST_REQUIRE(result.first[5] == 54);
      BOOST_REQUIRE(result.first[6] == 3);
      BOOST_REQUIRE(result.first[7] == 75);

      BOOST_REQUIRE(result.second.size() == 10);
      BOOST_REQUIRE(result.second[0] == 0);
      BOOST_REQUIRE(result.second[1] == 70);
      BOOST_REQUIRE(result.second[2] == 71);
      BOOST_REQUIRE(result.second[3] == 72);
      BOOST_REQUIRE(result.second[4] == 1);
      BOOST_REQUIRE(result.second[5] == 52);
      BOOST_REQUIRE(result.second[6] == 53);
      BOOST_REQUIRE(result.second[7] == 74);
      BOOST_REQUIRE(result.second[8] == 3);
      BOOST_REQUIRE(result.second[9] == 55);
   }
}


BOOST_AUTO_TEST_SUITE_END()
