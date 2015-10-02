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


#include <algorithm>
#include <cassert>
#include <cmath>


#include <boost/test/unit_test.hpp>


#include "Alignment.hpp"
#include "distance.hpp"
#include "pairing.hpp"


using namespace algo;


namespace {


unsigned int _uintSymmetricCompare(unsigned int lhs, unsigned int rhs)
{
   return ((lhs > rhs) ? (lhs - rhs) : (rhs - lhs));
}


float _floatSymmetricCompare(unsigned int lhs, unsigned int rhs)
{
   if (lhs != rhs) {
      unsigned int min = lhs;
      unsigned int max = rhs;
      if (min > max)
      {
         std::swap(min, max);
      }
      assert(max);
      return (static_cast<float>(min) / static_cast<float>(max));
   }
   return 1.0f;
}


float _stringSymmetricCompare(const char * lhs, const char * rhs)
{
   const std::vector<char> left(lhs, lhs + strlen(lhs));
   const std::vector<char> right(rhs, rhs + strlen(rhs));
   return getLevenshteinDistanceSimilarity(left, right);
}


}


/***************************************************************************
 *   PairingHelper test                                                    *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libalgo_pairing)


BOOST_AUTO_TEST_CASE(test_MetricMatrix_constructor)
{
   {
      std::vector<unsigned int> items;
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      BOOST_REQUIRE(m.size() == 0);
      BOOST_REQUIRE(!m.tailSize());
   }

   {
      std::vector<unsigned int> items = {1};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      BOOST_REQUIRE(m.size() == 1);
      BOOST_REQUIRE(m.at(0, 0) == 0);
      BOOST_REQUIRE(!m.tailSize());
   }

   {
      std::vector<unsigned int> items = {1, 10};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      BOOST_REQUIRE(m.size() == 2);
      BOOST_REQUIRE(m.at(0, 0) == 0 && m.at(0, 1) == 9);
      BOOST_REQUIRE(m.at(1, 0) == 9 && m.at(1, 1) == 0);
      BOOST_REQUIRE(!m.tailSize());
   }

   {
      std::vector<unsigned int> items = {1, 10, 2};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      BOOST_REQUIRE(m.size() == 3);
      BOOST_REQUIRE(m.at(0, 0) == 0 && m.at(0, 1) == 9 && m.at(0, 2) == 1);
      BOOST_REQUIRE(m.at(1, 0) == 9 && m.at(1, 1) == 0 && m.at(1, 2) == 8);
      BOOST_REQUIRE(m.at(2, 0) == 1 && m.at(2, 1) == 8 && m.at(2, 2) == 0);
      BOOST_REQUIRE(!m.tailSize());
   }

   {
      std::vector<unsigned int> items = {1, 10, 2, 12};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      BOOST_REQUIRE(m.size() == 4);
      BOOST_REQUIRE(m.at(0, 0) == 0 && m.at(0, 1) == 9 && m.at(0, 2) == 1 && m.at(0, 3) == 11);
      BOOST_REQUIRE(m.at(1, 0) == 9 && m.at(1, 1) == 0 && m.at(1, 2) == 8 && m.at(1, 3) == 2);
      BOOST_REQUIRE(m.at(2, 0) == 1 && m.at(2, 1) == 8 && m.at(2, 2) == 0 && m.at(2, 3) == 10);
      BOOST_REQUIRE(m.at(3, 0) == 11 && m.at(3, 1) == 2 && m.at(3, 2) == 10 && m.at(3, 3) == 0);
      BOOST_REQUIRE(!m.tailSize());
   }
}


BOOST_AUTO_TEST_CASE(test_MetricMatrix_remove)
{
   std::vector<unsigned int> items = {1, 10, 2, 12};
   pairing::MetricMatrix<unsigned int> matrix(
      items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
   );
   {
      pairing::MetricMatrix<unsigned int> m = matrix;
      m.remove(0);
      BOOST_REQUIRE(m.size() == 3);
      BOOST_REQUIRE(m.at(0, 0) == 0 && m.at(0, 1) == 8 && m.at(0, 2) == 2);
      BOOST_REQUIRE(m.at(1, 0) == 8 && m.at(1, 1) == 0 && m.at(1, 2) == 10);
      BOOST_REQUIRE(m.at(2, 0) == 2 && m.at(2, 1) == 10 && m.at(2, 2) == 0);
      BOOST_REQUIRE(m.originalIndex(0) == 1);
      BOOST_REQUIRE(m.originalIndex(1) == 2);
      BOOST_REQUIRE(m.originalIndex(2) == 3);
      BOOST_REQUIRE(m.tailSize() == 1);
      BOOST_REQUIRE(m.tail()[0] == 0);

      m.remove(1);
      BOOST_REQUIRE(m.size() == 2);
      BOOST_REQUIRE(m.at(0, 0) == 0 && m.at(0, 1) == 2);
      BOOST_REQUIRE(m.at(1, 0) == 2 && m.at(1, 1) == 0);
      BOOST_REQUIRE(m.originalIndex(0) == 1);
      BOOST_REQUIRE(m.originalIndex(1) == 3);
      BOOST_REQUIRE(m.tailSize() == 2);
      BOOST_REQUIRE(m.tail()[0] == 2);
      BOOST_REQUIRE(m.tail()[1] == 0);

      m.remove(1);
      BOOST_REQUIRE(m.size() == 1);
      BOOST_REQUIRE(m.at(0, 0) == 0);
      BOOST_REQUIRE(m.originalIndex(0) == 1);
      BOOST_REQUIRE(m.tailSize() == 3);
      BOOST_REQUIRE(m.tail()[0] == 3);
      BOOST_REQUIRE(m.tail()[1] == 2);
      BOOST_REQUIRE(m.tail()[2] == 0);

      m.remove(0);
      BOOST_REQUIRE(m.size() == 0);
      BOOST_REQUIRE(m.tailSize() == 4);
      BOOST_REQUIRE(m.tail()[0] == 1);
      BOOST_REQUIRE(m.tail()[1] == 3);
      BOOST_REQUIRE(m.tail()[2] == 2);
      BOOST_REQUIRE(m.tail()[3] == 0);
   }

   {
      pairing::MetricMatrix<unsigned int> m = matrix;
      m.remove(2);
      BOOST_REQUIRE(m.size() == 3);
      BOOST_REQUIRE(m.at(0, 0) == 0 && m.at(0, 1) == 9 && m.at(0, 2) == 11);
      BOOST_REQUIRE(m.at(1, 0) == 9 && m.at(1, 1) == 0 && m.at(1, 2) == 2);
      BOOST_REQUIRE(m.at(2, 0) == 11 && m.at(2, 1) == 2 && m.at(2, 2) == 0);
      BOOST_REQUIRE(m.originalIndex(0) == 0);
      BOOST_REQUIRE(m.originalIndex(1) == 1);
      BOOST_REQUIRE(m.originalIndex(2) == 3);
      BOOST_REQUIRE(m.tailSize() == 1);
      BOOST_REQUIRE(m.tail()[0] == 2);

      m.remove(1);
      BOOST_REQUIRE(m.size() == 2);
      BOOST_REQUIRE(m.at(0, 0) == 0 && m.at(0, 1) == 11);
      BOOST_REQUIRE(m.at(1, 0) == 11 && m.at(1, 1) == 0);
      BOOST_REQUIRE(m.originalIndex(0) == 0);
      BOOST_REQUIRE(m.originalIndex(1) == 3);
      BOOST_REQUIRE(m.tailSize() == 2);
      BOOST_REQUIRE(m.tail()[0] == 1);
      BOOST_REQUIRE(m.tail()[1] == 2);

      m.remove(0);
      BOOST_REQUIRE(m.size() == 1);
      BOOST_REQUIRE(m.at(0, 0) == 0);
      BOOST_REQUIRE(m.originalIndex(0) == 3);
      BOOST_REQUIRE(m.tailSize() == 3);
      BOOST_REQUIRE(m.tail()[0] == 0);
      BOOST_REQUIRE(m.tail()[1] == 1);
      BOOST_REQUIRE(m.tail()[2] == 2);

      m.remove(0);
      BOOST_REQUIRE(m.size() == 0);
      BOOST_REQUIRE(m.tailSize() == 4);
      BOOST_REQUIRE(m.tail()[0] == 3);
      BOOST_REQUIRE(m.tail()[1] == 0);
      BOOST_REQUIRE(m.tail()[2] == 1);
      BOOST_REQUIRE(m.tail()[3] == 2);
   }
}


BOOST_AUTO_TEST_CASE(test_MetricMatrix_reduce)
{
   #define _EQUALS(a, b) (fabs(a - b) <= 0.001f)
   {
      std::vector<unsigned int> items;
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      m.reduce(boost::none);
      BOOST_REQUIRE(!m.size());
      BOOST_REQUIRE(!m.tailSize());
   }

   {
      std::vector<unsigned int> items = {10};
      pairing::MetricMatrix<unsigned int> m1(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<unsigned int> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(!m2.size());
      BOOST_REQUIRE(m2.tailSize() == 1);
      BOOST_REQUIRE(m2.tail()[0] == 0);

      m1.reduce(4);
      BOOST_REQUIRE(!m1.size());
      BOOST_REQUIRE(m1.tailSize() == 1);
      BOOST_REQUIRE(m1.tail()[0] == 0);
   }

   {
      std::vector<unsigned int> items = {4, 6};
      pairing::MetricMatrix<unsigned int> m1(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<unsigned int> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 2);
      BOOST_REQUIRE(m2.at(0, 0) == 0 && m2.at(0, 1) == 2);
      BOOST_REQUIRE(m2.at(1, 0) == 2 && m2.at(1, 1) == 0);

      pairing::MetricMatrix<unsigned int> m3 = m1;
      m3.reduce(2);
      BOOST_REQUIRE(m3.size() == 2);
      BOOST_REQUIRE(m3.at(0, 0) == 0 && m3.at(0, 1) == 2);
      BOOST_REQUIRE(m3.at(1, 0) == 2 && m3.at(1, 1) == 0);

      m1.reduce(1);
      BOOST_REQUIRE(!m1.size());
      BOOST_REQUIRE(m1.tailSize() == 2);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
   }

   {
      std::vector<unsigned int> items = {4, 6};
      pairing::MetricMatrix<float> m1(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<float> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 2);
      BOOST_REQUIRE(_EQUALS(m2.at(0, 0), 1.0f) && _EQUALS(m2.at(0, 1), (2.0f / 3.0f)));
      BOOST_REQUIRE(_EQUALS(m2.at(1, 0), (2.0f / 3.0f)) && _EQUALS(m2.at(1, 1), 1.0f));

      pairing::MetricMatrix<float> m3 = m1;
      m3.reduce(0.5f);
      BOOST_REQUIRE(m3.size() == 2);
      BOOST_REQUIRE(_EQUALS(m3.at(0, 0), 1.0f) && _EQUALS(m3.at(0, 1), (2.0f / 3.0f)));
      BOOST_REQUIRE(_EQUALS(m3.at(1, 0), (2.0f / 3.0f)) && _EQUALS(m3.at(1, 1), 1.0f));

      m1.reduce(0.75f);
      BOOST_REQUIRE(!m1.size());
      BOOST_REQUIRE(m1.tailSize() == 2);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9};
      pairing::MetricMatrix<unsigned int> m1(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<unsigned int> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 2);
      BOOST_REQUIRE(m2.at(0, 0) == 0 && m2.at(0, 1) == 2);
      BOOST_REQUIRE(m2.at(1, 0) == 2 && m2.at(1, 1) == 0);
      BOOST_REQUIRE(m2.tailSize() == 1);
      BOOST_REQUIRE(m2.tail()[0] == 0);

      pairing::MetricMatrix<unsigned int> m3 = m1;
      m3.reduce(3);
      BOOST_REQUIRE(m3.size() == 2);
      BOOST_REQUIRE(m3.at(0, 0) == 0 && m3.at(0, 1) == 2);
      BOOST_REQUIRE(m3.at(1, 0) == 2 && m3.at(1, 1) == 0);
      BOOST_REQUIRE(m3.tailSize() == 1);
      BOOST_REQUIRE(m3.tail()[0] == 0);

      m1.reduce(1);
      BOOST_REQUIRE(!m1.size());
      BOOST_REQUIRE(m1.tailSize() == 3);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
      BOOST_REQUIRE(m1.tail()[2] == 2);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9};
      pairing::MetricMatrix<float> m1(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<float> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 2);
      BOOST_REQUIRE(_EQUALS(m2.at(0, 0), 1.0f) && _EQUALS(m2.at(0, 1), (7.0f / 9.0f)));
      BOOST_REQUIRE(_EQUALS(m2.at(1, 0), (7.0f / 9.0f)) && _EQUALS(m2.at(1, 1), 1.0f));
      BOOST_REQUIRE(m2.tailSize() == 1);
      BOOST_REQUIRE(m2.tail()[0] == 0);

      pairing::MetricMatrix<float> m3 = m1;
      m3.reduce(0.5f);
      BOOST_REQUIRE(m3.size() == 2);
      BOOST_REQUIRE(_EQUALS(m3.at(0, 0), 1.0f) && _EQUALS(m3.at(0, 1), (7.0f / 9.0f)));
      BOOST_REQUIRE(_EQUALS(m3.at(1, 0), (7.0f / 9.0f)) && _EQUALS(m3.at(1, 1), 1.0f));
      BOOST_REQUIRE(m3.tailSize() == 1);
      BOOST_REQUIRE(m3.tail()[0] == 0);

      m1.reduce(0.85f);
      BOOST_REQUIRE(!m1.size());
      BOOST_REQUIRE(m1.tailSize() == 3);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
      BOOST_REQUIRE(m1.tail()[2] == 2);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9, 9};
      pairing::MetricMatrix<unsigned int> m1(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<unsigned int> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 4);
      BOOST_REQUIRE(m2.at(0, 0) == 0 && m2.at(0, 1) == 4 && m2.at(0, 2) == 6 && m2.at(0, 3) == 6);
      BOOST_REQUIRE(m2.at(1, 0) == 4 && m2.at(1, 1) == 0 && m2.at(1, 2) == 2 && m2.at(1, 3) == 2);
      BOOST_REQUIRE(m2.at(2, 0) == 6 && m2.at(2, 1) == 2 && m2.at(2, 2) == 0 && m2.at(2, 3) == 0);
      BOOST_REQUIRE(m2.at(3, 0) == 6 && m2.at(3, 1) == 2 && m2.at(3, 2) == 0 && m2.at(3, 3) == 0);

      pairing::MetricMatrix<unsigned int> m3 = m1;
      m3.reduce(10);
      BOOST_REQUIRE(m3.size() == 4);

      pairing::MetricMatrix<unsigned int> m4 = m1;
      m4.reduce(3);
      BOOST_REQUIRE(m4.size() == 4);

      m1.reduce(1);
      BOOST_REQUIRE(m1.size() == 2);
      BOOST_REQUIRE(m1.at(0, 0) == 0 && m1.at(1, 0) == 0);
      BOOST_REQUIRE(m1.at(0, 1) == 0 && m1.at(1, 1) == 0);
      BOOST_REQUIRE(m1.tailSize() == 2);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9, 9};
      pairing::MetricMatrix<float> m1(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<float> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 4);
      BOOST_REQUIRE(_EQUALS(m2.at(0, 0), 1.0f) && _EQUALS(m2.at(0, 1), (3.0f / 7.0f)) && _EQUALS(m2.at(0, 2), (1.0f / 3.0f)) && _EQUALS(m2.at(0, 3), (1.0f / 3.0f)));
      BOOST_REQUIRE(_EQUALS(m2.at(1, 0), (3.0f / 7.0f)) && _EQUALS(m2.at(1, 1), 1.0f) && _EQUALS(m2.at(1, 2), (7.0f / 9.0f)) && _EQUALS(m2.at(1, 3), (7.0f / 9.0f)));
      BOOST_REQUIRE(_EQUALS(m2.at(2, 0), (1.0f / 3.0f)) && _EQUALS(m2.at(2, 1), (7.0f / 9.0f)) && _EQUALS(m2.at(2, 2), 1.0f) && _EQUALS(m2.at(2, 3), 1.0f));
      BOOST_REQUIRE(_EQUALS(m2.at(3, 0), (1.0f / 3.0f)) && _EQUALS(m2.at(3, 1), (7.0f / 9.0f)) && _EQUALS(m2.at(3, 2), 1.0f) && _EQUALS(m2.at(3, 3), 1.0f));

      pairing::MetricMatrix<float> m3 = m1;
      m3.reduce(0.1f);
      BOOST_REQUIRE(m3.size() == 4);

      pairing::MetricMatrix<float> m4 = m1;
      m4.reduce(0.5f);
      BOOST_REQUIRE(m4.size() == 4);

      m1.reduce(0.85f);
      BOOST_REQUIRE(m1.size() == 2);
      BOOST_REQUIRE(_EQUALS(m1.at(0, 0), 1.0f) && _EQUALS(m1.at(1, 0), 1.0f));
      BOOST_REQUIRE(_EQUALS(m1.at(0, 1), 1.0f) && _EQUALS(m1.at(1, 1), 1.0f));
      BOOST_REQUIRE(m1.tailSize() == 2);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
   }

   {
      std::vector<unsigned int> items = {9, 7, 25, 3, 15};
      pairing::MetricMatrix<unsigned int> m1(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<unsigned int> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 4);
      BOOST_REQUIRE(m2.at(0, 0) == 0 && m2.at(0, 1) == 2 && m2.at(0, 2) == 6 && m2.at(0, 3) == 6);
      BOOST_REQUIRE(m2.at(1, 0) == 2 && m2.at(1, 1) == 0 && m2.at(1, 2) == 4 && m2.at(1, 3) == 8);
      BOOST_REQUIRE(m2.at(2, 0) == 6 && m2.at(2, 1) == 4 && m2.at(2, 2) == 0 && m2.at(2, 3) == 12);
      BOOST_REQUIRE(m2.at(3, 0) == 6 && m2.at(3, 1) == 8 && m2.at(3, 2) == 12 && m2.at(3, 3) == 0);
      BOOST_REQUIRE(m2.tailSize() == 1);
      BOOST_REQUIRE(m2.tail()[0] == 2);

      pairing::MetricMatrix<unsigned int> m3 = m1;
      m3.reduce(9);
      BOOST_REQUIRE(m3.size() == 4);

      pairing::MetricMatrix<unsigned int> m4 = m1;
      m4.reduce(3);
      BOOST_REQUIRE(m4.size() == 2);
      BOOST_REQUIRE(m4.at(0, 0) == 0 && m4.at(1, 0) == 2);
      BOOST_REQUIRE(m4.at(0, 1) == 2 && m4.at(1, 1) == 0);
      BOOST_REQUIRE(m4.tailSize() == 3);
      BOOST_REQUIRE(m4.tail()[0] == 2);
      BOOST_REQUIRE(m4.tail()[1] == 3);
      BOOST_REQUIRE(m4.tail()[2] == 4);

      pairing::MetricMatrix<unsigned int> m5 = m2;
      m5.reduce(3);
      BOOST_REQUIRE(m5.size() == 2);
      BOOST_REQUIRE(m5.at(0, 0) == 0 && m5.at(1, 0) == 2);
      BOOST_REQUIRE(m5.at(0, 1) == 2 && m5.at(1, 1) == 0);
      BOOST_REQUIRE(m5.tailSize() == 3);
      BOOST_REQUIRE(m5.tail()[0] == 3);
      BOOST_REQUIRE(m5.tail()[1] == 4);
      BOOST_REQUIRE(m5.tail()[2] == 2);

      m1.reduce(1);
      BOOST_REQUIRE(m1.size() == 0);
      BOOST_REQUIRE(m1.tailSize() == 5);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
      BOOST_REQUIRE(m1.tail()[2] == 2);
      BOOST_REQUIRE(m1.tail()[3] == 3);
      BOOST_REQUIRE(m1.tail()[4] == 4);
   }

   {
      std::vector<unsigned int> items = {9, 7, 25, 3, 15};
      pairing::MetricMatrix<float> m1(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      pairing::MetricMatrix<float> m2 = m1;
      m2.reduce(boost::none);
      BOOST_REQUIRE(m2.size() == 4);
      BOOST_REQUIRE(_EQUALS(m2.at(0, 0), 1.0f) && _EQUALS(m2.at(0, 1), (7.0f / 9.0f)) && _EQUALS(m2.at(0, 2), (9.0f / 25.0f)) && _EQUALS(m2.at(0, 3), (3.0f / 5.0f)));
      BOOST_REQUIRE(_EQUALS(m2.at(1, 0), (7.0f / 9.0f)) && _EQUALS(m2.at(1, 1), 1.0f) && _EQUALS(m2.at(1, 2), (7.0f / 25.0f)) && _EQUALS(m2.at(1, 3), (7.0f / 15.0f)));
      BOOST_REQUIRE(_EQUALS(m2.at(2, 0), (9.0f / 25.0f)) && _EQUALS(m2.at(2, 1), (7.0f / 25.0f)) && _EQUALS(m2.at(2, 2), 1.0f) && _EQUALS(m2.at(2, 3), (3.0f / 5.0f)));
      BOOST_REQUIRE(_EQUALS(m2.at(3, 0), (3.0f / 5.0f)) && _EQUALS(m2.at(3, 1), (7.0f / 15.0f)) && _EQUALS(m2.at(3, 2), (3.0f / 5.0f)) && _EQUALS(m2.at(3, 3), 1.0f));
      BOOST_REQUIRE(m2.tailSize() == 1);
      BOOST_REQUIRE(m2.tail()[0] == 3);

      pairing::MetricMatrix<float> m3 = m1;
      m3.reduce(0.45f);
      BOOST_REQUIRE(m3.size() == 4);

      pairing::MetricMatrix<float> m4 = m1;
      m4.reduce(0.65f);
      BOOST_REQUIRE(m4.size() == 2);
      BOOST_REQUIRE(_EQUALS(m4.at(0, 0), 1.0f) && _EQUALS(m4.at(1, 0), (7.0f / 9.0f)));
      BOOST_REQUIRE(_EQUALS(m4.at(0, 1), (7.0f / 9.0f)) && _EQUALS(m4.at(1, 1), 1.0f));
      BOOST_REQUIRE(m4.tailSize() == 3);
      BOOST_REQUIRE(m4.tail()[0] == 2);
      BOOST_REQUIRE(m4.tail()[1] == 3);
      BOOST_REQUIRE(m4.tail()[2] == 4);

      pairing::MetricMatrix<float> m5 = m2;
      m5.reduce(0.65f);
      BOOST_REQUIRE(m5.size() == 2);
      BOOST_REQUIRE(_EQUALS(m5.at(0, 0), 1.0f) && _EQUALS(m5.at(1, 0), (7.0f / 9.0f)));
      BOOST_REQUIRE(_EQUALS(m5.at(0, 1), (7.0f / 9.0f)) && _EQUALS(m5.at(1, 1), 1.0f));
      BOOST_REQUIRE(m5.tailSize() == 3);
      BOOST_REQUIRE(m5.tail()[0] == 2);
      BOOST_REQUIRE(m5.tail()[1] == 4);
      BOOST_REQUIRE(m5.tail()[2] == 3);

      m1.reduce(0.80f);
      BOOST_REQUIRE(m1.size() == 0);
      BOOST_REQUIRE(m1.tailSize() == 5);
      BOOST_REQUIRE(m1.tail()[0] == 0);
      BOOST_REQUIRE(m1.tail()[1] == 1);
      BOOST_REQUIRE(m1.tail()[2] == 2);
      BOOST_REQUIRE(m1.tail()[3] == 3);
      BOOST_REQUIRE(m1.tail()[4] == 4);
   }
   #undef _EQUALS
}


BOOST_AUTO_TEST_CASE(test_MetricMatrix_roommatesMatrix)
{
   {
      std::vector<unsigned int> items;
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }

   {
      std::vector<unsigned int> items = {10};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 1);
      BOOST_REQUIRE(r[0][0] == 0);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }

   {
      std::vector<unsigned int> items = {4, 6};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);

      m.reduce(1);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }

   {
      std::vector<unsigned int> items = {4, 6};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);

      m.reduce(0.75f);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }

   {
      std::vector<unsigned int> items = {3, 7, 9};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 3);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 2 && r[0][2] == 0);
      BOOST_REQUIRE(r[1][0] == 2 && r[1][1] == 0 && r[1][2] == 1);
      BOOST_REQUIRE(r[2][0] == 1 && r[2][1] == 0 && r[2][2] == 2);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);

      m.reduce(1);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }

   {
      std::vector<unsigned int> items = {3, 7, 9};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 3);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 2 && r[0][2] == 0);
      BOOST_REQUIRE(r[1][0] == 2 && r[1][1] == 0 && r[1][2] == 1);
      BOOST_REQUIRE(r[2][0] == 1 && r[2][1] == 0 && r[2][2] == 2);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);

      m.reduce(0.85f);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }

   {
      std::vector<unsigned int> items = {3, 7, 9, 9};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);
      BOOST_REQUIRE(r[0][0] == 1 && (r[0][1] == 2 || r[0][1] == 3) && (r[0][2] == 3 || r[0][2] == 2) && r[0][3] == 0);
      BOOST_REQUIRE((r[1][0] == 2 || r[1][0] == 3) && (r[1][1] == 3 || r[1][1] == 2) && r[1][2] == 0 && r[1][3] == 1);
      BOOST_REQUIRE(r[2][0] == 3 && r[2][1] == 1 && r[2][2] == 0 && r[2][3] == 2);
      BOOST_REQUIRE(r[3][0] == 2 && r[3][1] == 1 && r[3][2] == 0 && r[3][3] == 3);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);

      m.reduce(3);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);

      m.reduce(1);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9, 9};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);
      BOOST_REQUIRE(r[0][0] == 1 && (r[0][1] == 2 || r[0][1] == 3) && (r[0][2] == 3 || r[0][2] == 2) && r[0][3] == 0);
      BOOST_REQUIRE((r[1][0] == 2 || r[1][0] == 3) && (r[1][1] == 3 || r[1][1] == 2) && r[1][2] == 0 && r[1][3] == 1);
      BOOST_REQUIRE(r[2][0] == 3 && r[2][1] == 1 && r[2][2] == 0 && r[2][3] == 2);
      BOOST_REQUIRE(r[3][0] == 2 && r[3][1] == 1 && r[3][2] == 0 && r[3][3] == 3);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);

      m.reduce(0.5f);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);

      m.reduce(0.85f);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);
   }

   {
      std::vector<unsigned int> items = {9, 7, 25, 3, 15};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 5);
      BOOST_REQUIRE(r[0][0] == 1 && (r[0][1] == 3 || r[0][1] == 4) && (r[0][2] == 4 || r[0][2] == 3) && r[0][3] == 2 && r[0][4] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 3 && r[1][2] == 4 && r[1][3] == 2 && r[1][4] == 1);
      BOOST_REQUIRE(r[2][0] == 4 && r[2][1] == 0 && r[2][2] == 1 && r[2][3] == 3 && r[2][4] == 2);
      BOOST_REQUIRE(r[3][0] == 1 && r[3][1] == 0 && r[3][2] == 4 && r[3][3] == 2 && r[3][4] == 3);
      BOOST_REQUIRE(r[4][0] == 0 && r[4][1] == 1 && r[4][2] == 2 && r[4][3] == 3 && r[4][4] == 4);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);
      BOOST_REQUIRE(r[0][0] == 1 && (r[0][1] == 2 || r[0][1] == 3) && (r[0][2] == 3 || r[0][2] == 2) && r[0][3] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 2 && r[1][2] == 3 && r[1][3] == 1);
      BOOST_REQUIRE(r[2][0] == 1 && r[2][1] == 0 && r[2][2] == 3 && r[2][3] == 2);
      BOOST_REQUIRE(r[3][0] == 0 && r[3][1] == 1 && r[3][2] == 2 && r[3][3] == 3);

      m.reduce(9);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);

      m.reduce(3);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);

      m.reduce(1);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }

   {
      std::vector<unsigned int> items = {9, 7, 25, 3, 15};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );

      roommates::Matrix<size_t> r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 5);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 4 && r[0][2] == 2 && r[0][3] == 3 && r[0][4] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 4 && r[1][2] == 3 && r[1][3] == 2 && r[1][4] == 1);
      BOOST_REQUIRE(r[2][0] == 4 && r[2][1] == 0 && r[2][2] == 1 && r[2][3] == 3 && r[2][4] == 2);
      BOOST_REQUIRE(r[3][0] == 1 && r[3][1] == 0 && r[3][2] == 4 && r[3][3] == 2 && r[3][4] == 3);
      BOOST_REQUIRE((r[4][0] == 0 || r[4][0] == 2) && (r[4][1] == 2 || r[4][1] == 0) && r[4][2] == 1 && r[4][3] == 3 && r[4][4] == 4);

      m.reduce(boost::none);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 3 && r[0][2] == 2 && r[0][3] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 3 && r[1][2] == 2 && r[1][3] == 1);
      BOOST_REQUIRE(r[2][0] == 3 && r[2][1] == 0 && r[2][2] == 1 && r[2][3] == 2);
      BOOST_REQUIRE((r[3][0] == 0 || r[3][0] == 2) && (r[3][1] == 2 || r[3][1] == 0) && r[3][2] == 1 && r[3][3] == 3);


      m.reduce(0.45f);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 4);

      m.reduce(0.65f);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(r.size() == 2);
      BOOST_REQUIRE(r[0][0] == 1 && r[0][1] == 0);
      BOOST_REQUIRE(r[1][0] == 0 && r[1][1] == 1);

      m.reduce(0.80f);
      r = std::move(m.roommatesMatrix());
      BOOST_REQUIRE(!r.size());
   }
}


BOOST_AUTO_TEST_CASE(test_MetricMatrix_globalExtremum)
{
   {
      std::vector<unsigned int> items = {4, 6};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }

   {
      std::vector<unsigned int> items = {4, 6};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 2 && ext.second == 1);

      m.reduce(boost::none);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 2 && ext.second == 1);

      m.reduce(boost::none);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9, 9};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 3 && ext.second == 2);

      m.reduce(1);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }

   {
      std::vector<unsigned int> items = {3, 7, 9, 9};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 3 && ext.second == 2);

      m.reduce(0.85f);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }

   {
      std::vector<unsigned int> items = {9, 7, 25, 3, 15};
      pairing::MetricMatrix<unsigned int> m(
         items, _uintSymmetricCompare, pairing::MS_LESS_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);

      m.reduce(boost::none);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);

      m.reduce(3);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }

   {
      std::vector<unsigned int> items = {9, 7, 25, 3, 15};
      pairing::MetricMatrix<float> m(
         items, _floatSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );
      std::pair<size_t, size_t> ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);

      m.reduce(boost::none);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);

      m.reduce(0.65f);
      ext = m.globalExtremum();
      BOOST_REQUIRE(ext.first == 1 && ext.second == 0);
   }
}


BOOST_AUTO_TEST_CASE(test_resolve)
{
   #define _EQUALS(a, b) (fabs(a - b) <= 0.001f)

   std::vector<const char *> items = {
      "test completed",
      "one two three",
      "one more example",
      "one 2 three",
      "test failed",
      "unique string",
      "example"
   };

   {
      std::vector<pairing::Pair<float> > p = pairing::resolve<float>(
         items, _stringSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE
      );
      BOOST_REQUIRE(p.size() == 4);
      BOOST_REQUIRE(p[0].left == 0 && p[0].right && *p[0].right == 4 && p[0].metric && _EQUALS(*p[0].metric, 0.571429f));
      BOOST_REQUIRE(p[1].left == 1 && p[1].right && *p[1].right == 3 && p[1].metric && _EQUALS(*p[1].metric, 0.76923));
      BOOST_REQUIRE(p[2].left == 2 && p[2].right && *p[2].right == 6 && p[2].metric && _EQUALS(*p[2].metric, 0.4375f));
      BOOST_REQUIRE(p[3].left == 5 && !p[3].right && !p[3].metric);
   }

   {
      std::vector<pairing::Pair<float> > p = pairing::resolve<float>(
         items, _stringSymmetricCompare, pairing::MS_MORE_METRIC_MORE_ALIKE, 0.5f
      );
      BOOST_REQUIRE(p.size() == 5);
      BOOST_REQUIRE(p[0].left == 0 && p[0].right && *p[0].right == 4 && p[0].metric && _EQUALS(*p[0].metric, 0.571429f));
      BOOST_REQUIRE(p[1].left == 1 && p[1].right && *p[1].right == 3 && p[1].metric && _EQUALS(*p[1].metric, 0.76923));
      BOOST_REQUIRE(p[2].left == 2 && !p[2].right && !p[2].metric);
      BOOST_REQUIRE(p[3].left == 5 && !p[3].right && !p[3].metric);
      BOOST_REQUIRE(p[4].left == 6 && !p[4].right && !p[4].metric);
   }

   // TODO: Try to write a case for the greedy branch of the algorithm;

   #undef _EQUALS
}


BOOST_AUTO_TEST_SUITE_END()
