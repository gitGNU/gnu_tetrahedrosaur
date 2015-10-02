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


#include "roommates.hpp"


using namespace algo;


/***************************************************************************
 *   roommates test                                                        *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libalgo_roommates)


BOOST_AUTO_TEST_CASE(test_roommates_resolve)
{
   {
      roommates::Matrix<roommates::PersonType> p(6);
      p[0][0] = 2; p[0][1] = 3; p[0][2] = 1; p[0][3] = 5; p[0][4] = 4;
      p[1][0] = 5; p[1][1] = 4; p[1][2] = 3; p[1][3] = 0; p[1][4] = 2;
      p[2][0] = 1; p[2][1] = 3; p[2][2] = 4; p[2][3] = 0; p[2][4] = 5;
      p[3][0] = 4; p[3][1] = 1; p[3][2] = 2; p[3][3] = 5; p[3][4] = 0;
      p[4][0] = 2; p[4][1] = 0; p[4][2] = 1; p[4][3] = 3; p[4][4] = 5;
      p[5][0] = 4; p[5][1] = 0; p[5][2] = 2; p[5][3] = 3; p[5][4] = 1;

      std::vector<roommates::PersonType> res;
      const bool solutionFound = roommates::resolve(p, res);
      BOOST_REQUIRE(solutionFound);
      BOOST_REQUIRE(res.size() == 6);
      BOOST_REQUIRE(res[0] == 5);
      BOOST_REQUIRE(res[1] == 3);
      BOOST_REQUIRE(res[2] == 4);
      BOOST_REQUIRE(res[3] == 1);
      BOOST_REQUIRE(res[4] == 2);
      BOOST_REQUIRE(res[5] == 0);
   }

   {
      roommates::Matrix<roommates::PersonType> p(8);
      p[0][0] = 1; p[0][1] = 4; p[0][2] = 3; p[0][3] = 5; p[0][4] = 6; p[0][5] = 7; p[0][6] = 2;
      p[1][0] = 2; p[1][1] = 5; p[1][2] = 0; p[1][3] = 6; p[1][4] = 7; p[1][5] = 4; p[1][6] = 3;
      p[2][0] = 3; p[2][1] = 6; p[2][2] = 1; p[2][3] = 7; p[2][4] = 4; p[2][5] = 5; p[2][6] = 0;
      p[3][0] = 0; p[3][1] = 7; p[3][2] = 2; p[3][3] = 4; p[3][4] = 5; p[3][5] = 6; p[3][6] = 1;
      p[4][0] = 5; p[4][1] = 0; p[4][2] = 7; p[4][3] = 1; p[4][4] = 2; p[4][5] = 3; p[4][6] = 6;
      p[5][0] = 6; p[5][1] = 1; p[5][2] = 4; p[5][3] = 2; p[5][4] = 3; p[5][5] = 0; p[5][6] = 7;
      p[6][0] = 7; p[6][1] = 2; p[6][2] = 5; p[6][3] = 3; p[6][4] = 0; p[6][5] = 1; p[6][6] = 4;
      p[7][0] = 4; p[7][1] = 3; p[7][2] = 6; p[7][3] = 0; p[7][4] = 1; p[7][5] = 2; p[7][6] = 5;

      std::vector<roommates::PersonType> res;
      const bool solutionFound = roommates::resolve(p, res);
      BOOST_REQUIRE(solutionFound);
      BOOST_REQUIRE(res.size() == 8);
      BOOST_REQUIRE(res[0] == 3);
      BOOST_REQUIRE(res[1] == 2);
      BOOST_REQUIRE(res[2] == 1);
      BOOST_REQUIRE(res[3] == 0);
      BOOST_REQUIRE(res[4] == 5);
      BOOST_REQUIRE(res[5] == 4);
      BOOST_REQUIRE(res[6] == 7);
      BOOST_REQUIRE(res[7] == 6);
   }

   {
      roommates::Matrix<roommates::PersonType> p(6);
      p[0][0] = 3; p[0][1] = 5; p[0][2] = 1; p[0][3] = 4; p[0][4] = 2;
      p[1][0] = 5; p[1][1] = 2; p[1][2] = 4; p[1][3] = 0; p[1][4] = 3;
      p[2][0] = 3; p[2][1] = 4; p[2][2] = 0; p[2][3] = 5; p[2][4] = 1;
      p[3][0] = 1; p[3][1] = 5; p[3][2] = 4; p[3][3] = 0; p[3][4] = 2;
      p[4][0] = 3; p[4][1] = 1; p[4][2] = 2; p[4][3] = 5; p[4][4] = 0;
      p[5][0] = 4; p[5][1] = 0; p[5][2] = 3; p[5][3] = 1; p[5][4] = 2;

      std::vector<roommates::PersonType> res;
      const bool solutionFound = roommates::resolve(p, res);
      BOOST_REQUIRE(solutionFound);
      BOOST_REQUIRE(res.size() == 6);
      BOOST_REQUIRE(res[0] == 5);
      BOOST_REQUIRE(res[1] == 2);
      BOOST_REQUIRE(res[2] == 1);
      BOOST_REQUIRE(res[3] == 4);
      BOOST_REQUIRE(res[4] == 3);
      BOOST_REQUIRE(res[5] == 0);
   }

   {
      roommates::Matrix<roommates::PersonType> p(4);
      p[0][0] = 1; p[0][1] = 2; p[0][2] = 3;
      p[1][0] = 2; p[1][1] = 0; p[1][2] = 3;
      p[2][0] = 0; p[2][1] = 1; p[2][2] = 3;
      p[3][0] = 0; p[3][1] = 1; p[3][2] = 2;

      std::vector<roommates::PersonType> res;
      const bool solutionFound = roommates::resolve(p, res);
      BOOST_REQUIRE(!solutionFound);
      BOOST_REQUIRE(!res.size());
   }

   {
      roommates::Matrix<roommates::PersonType> p(6);
      p[0][0] = 1; p[0][1] = 5; p[0][2] = 3; p[0][3] = 2; p[0][4] = 4;
      p[1][0] = 2; p[1][1] = 4; p[1][2] = 0; p[1][3] = 5; p[1][4] = 3;
      p[2][0] = 0; p[2][1] = 5; p[2][2] = 1; p[2][3] = 4; p[2][4] = 3;
      p[3][0] = 4; p[3][1] = 1; p[3][2] = 2; p[3][3] = 5; p[3][4] = 0;
      p[4][0] = 5; p[4][1] = 0; p[4][2] = 2; p[4][3] = 3; p[4][4] = 1;
      p[5][0] = 3; p[5][1] = 1; p[5][2] = 4; p[5][3] = 0; p[5][4] = 2;

      std::vector<roommates::PersonType> res;
      const bool solutionFound = roommates::resolve(p, res);
      BOOST_REQUIRE(!solutionFound);
      BOOST_REQUIRE(!res.size());
   }

   {
      roommates::Matrix<roommates::PersonType> p(3);
      p[0][0] = 1; p[0][1] = 5;
      p[1][0] = 2; p[1][1] = 4;
      p[2][0] = 0; p[2][1] = 5;

      std::vector<roommates::PersonType> res;
      const bool solutionFound = roommates::resolve(p, res);
      BOOST_REQUIRE(!solutionFound);
      BOOST_REQUIRE(!res.size());
   }
}


BOOST_AUTO_TEST_SUITE_END()
