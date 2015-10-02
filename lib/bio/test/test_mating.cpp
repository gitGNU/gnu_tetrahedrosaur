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


#include "mating.hpp"


#include "algo/random_generators.hpp"


using namespace bio;


/***************************************************************************
 *   mating test                                                     *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libbio_mating)


BOOST_AUTO_TEST_CASE(test_mating)
{
   // |    3    |    2    |    1    |    0    |
   // 0       0.125      0.25      0.5        1
   const std::vector<float> values = {
      0.1f, 0.2f, 0.3f, 0.6f,
      0.7f, 0.8f, 0.1f
   };
   std::vector<std::pair<size_t, size_t> > res;

   {
      const algo::UniformNormalizedFloatRandomGenerator gen(values);

      res = std::move(mate(0, 0, gen));
      BOOST_REQUIRE(res.empty());

      res = std::move(mate(1, 0, gen));
      BOOST_REQUIRE(res.empty());

      res = std::move(mate(1, 1, gen));
      BOOST_REQUIRE(res.empty());

      res = std::move(mate(1, 2, gen));
      BOOST_REQUIRE(res.empty());

      res = std::move(mate(2, 0, gen));
      BOOST_REQUIRE(res.empty());
   }

   {
      const algo::UniformNormalizedFloatRandomGenerator gen(values);
      res = std::move(mate(2, 1, gen));
      BOOST_REQUIRE(res.size() == 1);
      BOOST_REQUIRE(res[0].first == 1 && res[0].second == 0);
   }

   {
      const algo::UniformNormalizedFloatRandomGenerator gen(values);
      res = std::move(mate(2, 2, gen));
      BOOST_REQUIRE(res.size() == 2);
      BOOST_REQUIRE(res[0].first == 1 && res[0].second == 0);
      BOOST_REQUIRE(res[1].first == 0 && res[1].second == 1);
   }


   {
      const algo::UniformNormalizedFloatRandomGenerator gen(values);
      res = std::move(mate(4, 1, gen));
      BOOST_REQUIRE(res.size() == 1);
      BOOST_REQUIRE(res[0].first == 3 && res[0].second == 2);
   }

   {
      const algo::UniformNormalizedFloatRandomGenerator gen(values);
      res = std::move(mate(4, 2, gen));
      BOOST_REQUIRE(res.size() == 2);
      BOOST_REQUIRE(res[0].first == 3 && res[0].second == 2);
      BOOST_REQUIRE(res[1].first == 1 && res[1].second == 0);
   }

   {
      const algo::UniformNormalizedFloatRandomGenerator gen(values);
      res = std::move(mate(4, 3, gen));
      BOOST_REQUIRE(res.size() == 3);
      BOOST_REQUIRE(res[0].first == 3 && res[0].second == 2);
      BOOST_REQUIRE(res[1].first == 1 && res[1].second == 0);
      BOOST_REQUIRE(res[2].first == 0 && res[2].second == 3);
   }
}


BOOST_AUTO_TEST_SUITE_END()
