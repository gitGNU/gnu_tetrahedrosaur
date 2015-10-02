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


#include "Alignment.hpp"


using namespace algo;


/***************************************************************************
 *   Alignment class test                                                  *
 ***************************************************************************/


BOOST_AUTO_TEST_SUITE(suite_libalgo_Alignment)


BOOST_AUTO_TEST_CASE(test_LcsMatrix)
{
   const std::vector<int> left = {0, 1, 2, 0, 3};
   const std::vector<int> right = {1, 0, 2};
   LcsMatrix matrix(left, right);

   BOOST_REQUIRE(matrix.lcsLength() == 2);
   //BOOST_REQUIRE(matrix.shape()[0] == first.size() + 1);
   //BOOST_REQUIRE(matrix.shape()[1] == second.size() + 1);

   BOOST_REQUIRE(matrix.at(0, 0) == 0);
   BOOST_REQUIRE(matrix.at(0, 1) == 0);
   BOOST_REQUIRE(matrix.at(0, 2) == 0);
   BOOST_REQUIRE(matrix.at(0, 3) == 0);

   BOOST_REQUIRE(matrix.at(1, 0) == 0);
   BOOST_REQUIRE(matrix.at(1, 1) == 0);
   BOOST_REQUIRE(matrix.at(1, 2) == 1);
   BOOST_REQUIRE(matrix.at(1, 3) == 1);

   BOOST_REQUIRE(matrix.at(2, 0) == 0);
   BOOST_REQUIRE(matrix.at(2, 1) == 1);
   BOOST_REQUIRE(matrix.at(2, 2) == 1);
   BOOST_REQUIRE(matrix.at(2, 3) == 1);

   BOOST_REQUIRE(matrix.at(3, 0) == 0);
   BOOST_REQUIRE(matrix.at(3, 1) == 1);
   BOOST_REQUIRE(matrix.at(3, 2) == 1);
   BOOST_REQUIRE(matrix.at(3, 3) == 2);

   BOOST_REQUIRE(matrix.at(4, 0) == 0);
   BOOST_REQUIRE(matrix.at(4, 1) == 1);
   BOOST_REQUIRE(matrix.at(4, 2) == 2);
   BOOST_REQUIRE(matrix.at(4, 3) == 2);

   BOOST_REQUIRE(matrix.at(5, 0) == 0);
   BOOST_REQUIRE(matrix.at(5, 1) == 1);
   BOOST_REQUIRE(matrix.at(5, 2) == 2);
   BOOST_REQUIRE(matrix.at(5, 3) == 2);
}


BOOST_AUTO_TEST_CASE(test_constructor)
{
   {
      //                            0  1  2  3  4
      const std::vector<int> s1 = {0, 1, 2, 0, 3};
      const std::vector<int> s2 =    {1,    0, 2};
      //                               0     1  2
      Alignment alignment(s1, s2);

      Alignment::const_bimap_iterator bit = alignment.bimapBegin();
      Alignment::const_bimap_iterator bite = alignment.bimapEnd();
      BOOST_REQUIRE(bit != bite && bit->left == 1 && bit->right == 0);
      ++bit;
      BOOST_REQUIRE(bit != bite && bit->left == 3 && bit->right == 1);
      ++bit;
      BOOST_REQUIRE(bit == bite);

      Alignment::const_iterator it = alignment.begin();
      Alignment::const_iterator ite = alignment.end();
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 0 && !it.right());
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 1 && it.right() && *it.right() == 0);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 2 && !it.right());
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 3 && it.right() && *it.right() == 1);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 4 && it.right() && *it.right() == 2);
      ++it;
      BOOST_REQUIRE(it == ite && !it.left() && !it.right());
   }

   {
      //                               0     1  2
      const std::vector<int> s1 =    {1,    0, 2};
      const std::vector<int> s2 = {3, 1, 5, 0, 3};
      //                            0  1  2  3  4
      Alignment alignment(s1, s2);

      Alignment::const_bimap_iterator bit = alignment.bimapBegin();
      Alignment::const_bimap_iterator bite = alignment.bimapEnd();
      BOOST_REQUIRE(bit != bite && bit->left == 0 && bit->right == 1);
      ++bit;
      BOOST_REQUIRE(bit != bite && bit->left == 1 && bit->right == 3);
      ++bit;
      BOOST_REQUIRE(bit == bite);

      Alignment::const_iterator it = alignment.begin();
      Alignment::const_iterator ite = alignment.end();
      BOOST_REQUIRE(it != ite && !it.left() && it.right() && *it.right() == 0);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 0 && it.right() && *it.right() == 1);
      ++it;
      BOOST_REQUIRE(it != ite && !it.left() && it.right() && *it.right() == 2);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 1 && it.right() && *it.right() == 3);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 2 && it.right() && *it.right() == 4);
      ++it;
      BOOST_REQUIRE(it == ite && !it.left() && !it.right());
   }

   {
      //                            0  1  2  3  4, 5
      const std::vector<int> s1 = {7, 2, 8, 5, 0, 3};
      const std::vector<int> s2 = {7, 2, 9,    0};
      //                            0  1  2     3
      Alignment alignment(s1, s2);

      Alignment::const_bimap_iterator bit = alignment.bimapBegin();
      Alignment::const_bimap_iterator bite = alignment.bimapEnd();
      BOOST_REQUIRE(bit != bite && bit->left == 0 && bit->right == 0);
      ++bit;
      BOOST_REQUIRE(bit != bite && bit->left == 1 && bit->right == 1);
      ++bit;
      BOOST_REQUIRE(bit != bite && bit->left == 4 && bit->right == 3);
      ++bit;
      BOOST_REQUIRE(bit == bite);

      Alignment::const_iterator it = alignment.begin();
      Alignment::const_iterator ite = alignment.end();
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 0 && it.right() && *it.right() == 0);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 1 && it.right() && *it.right() == 1);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 2 && it.right() && *it.right() == 2);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 3 && !it.right());
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 4 && it.right() && *it.right() == 3);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 5 && !it.right());
      ++it;
      BOOST_REQUIRE(it == ite && !it.left() && !it.right());
   }

   {
      //                            0  1  2     3
      const std::vector<int> s1 = {7, 2, 9,    0};
      const std::vector<int> s2 = {7, 2, 8, 5, 0, 3};
      //                            0  1  2  3  4, 5
      Alignment alignment(s1, s2);

      Alignment::const_bimap_iterator bit = alignment.bimapBegin();
      Alignment::const_bimap_iterator bite = alignment.bimapEnd();
      BOOST_REQUIRE(bit != bite && bit->left == 0 && bit->right == 0);
      ++bit;
      BOOST_REQUIRE(bit != bite && bit->left == 1 && bit->right == 1);
      ++bit;
      BOOST_REQUIRE(bit != bite && bit->left == 3 && bit->right == 4);
      ++bit;
      BOOST_REQUIRE(bit == bite);

      Alignment::const_iterator it = alignment.begin();
      Alignment::const_iterator ite = alignment.end();
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 0 && it.right() && *it.right() == 0);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 1 && it.right() && *it.right() == 1);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 2 && it.right() && *it.right() == 2);
      ++it;
      BOOST_REQUIRE(it != ite && !it.left() && it.right() && *it.right() == 3);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 3 && it.right() && *it.right() == 4);
      ++it;
      BOOST_REQUIRE(it != ite && !it.left() && it.right() && *it.right() == 5);
      ++it;
      BOOST_REQUIRE(it == ite && !it.left() && !it.right());
   }

   {
      //                            0  1  2  3  4
      const std::vector<int> s1 = {8, 2, 5, 3, 0};
      const std::vector<int> s2 = {7, 1, 5, 2, 0};
      //                            0  1  2  3  4
      Alignment alignment(s1, s2);

      Alignment::const_bimap_iterator bit = alignment.bimapBegin();
      Alignment::const_bimap_iterator bite = alignment.bimapEnd();
      BOOST_REQUIRE(bit != bite && bit->left == 2 && bit->right == 2);
      ++bit;
      BOOST_REQUIRE(bit != bite && bit->left == 4 && bit->right == 4);
      ++bit;
      BOOST_REQUIRE(bit == bite);

      Alignment::const_iterator it = alignment.begin();
      Alignment::const_iterator ite = alignment.end();
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 0 && it.right() && *it.right() == 0);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 1 && it.right() && *it.right() == 1);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 2 && it.right() && *it.right() == 2);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 3 && it.right() && *it.right() == 3);
      ++it;
      BOOST_REQUIRE(it != ite && it.left() && *it.left() == 4 && it.right() && *it.right() == 4);
      ++it;
      BOOST_REQUIRE(it == ite && !it.left() && !it.right());
   }
}


BOOST_AUTO_TEST_CASE(test_index)
{
   struct _TestCase
   {
      inline explicit _TestCase(const Alignment &al, bool keyIsFirst)
         : al(al), keyIsFirst(keyIsFirst)
      {}

      const Alignment & al;
      bool keyIsFirst;
   };

   { // (key == key);
      //                      0   1   2       3
      std::vector<int> s1 = {50, 51, 52,     53};
      std::vector<int> s2 = {50,     52, 99, 53, 99};
      //                      0       1   2   3   4
      Alignment al1(s1, s2);
      Alignment al2(s2, s1);

      BOOST_REQUIRE(al1.index(0, true) == 0);
      BOOST_REQUIRE(al1.index(2, true) == 1);
      BOOST_REQUIRE(al1.index(3, true) == 3);

      BOOST_REQUIRE(al2.index(0, false) == 0);
      BOOST_REQUIRE(al2.index(2, false) == 1);
      BOOST_REQUIRE(al2.index(3, false) == 3);
   }

   { // (prev && next);
      { // !(next->value > (prev->value + 1));
         //                      0   1   2   3   4   5   6   7   8   9  10  11  12  13  14
         std::vector<int> s1 = {50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};
         std::vector<int> s2 = {50,     52,         55,             59,                 64};
         //                      0       1           2               3                   4
         Alignment al1(s1, s2);
         Alignment al2(s2, s1);

         _TestCase testCase[] = {_TestCase(al1, true), _TestCase(al2, false)};
         for (size_t i = 0; i < sizeof(testCase) / sizeof(_TestCase); ++i)
         {
            const _TestCase & tc = testCase[i];
            //  (k <= (kmax / 2));
            BOOST_REQUIRE(tc.al.index(1, tc.keyIsFirst) == 0);
            BOOST_REQUIRE(tc.al.index(3, tc.keyIsFirst) == 1);
            BOOST_REQUIRE(tc.al.index(6, tc.keyIsFirst) == 2);
            BOOST_REQUIRE(tc.al.index(7, tc.keyIsFirst) == 2);
            BOOST_REQUIRE(tc.al.index(10, tc.keyIsFirst) == 3);
            BOOST_REQUIRE(tc.al.index(11, tc.keyIsFirst) == 3);

            // !(k <= (kmax / 2));
            BOOST_REQUIRE(tc.al.index(4, tc.keyIsFirst) == 2);
            BOOST_REQUIRE(tc.al.index(8, tc.keyIsFirst) == 3);
            BOOST_REQUIRE(tc.al.index(12, tc.keyIsFirst) == 4);
            BOOST_REQUIRE(tc.al.index(13, tc.keyIsFirst) == 4);
         }
      }

      { // (next->value > (prev->value + 1));
         //                      0   1   2   3   4   5   6       7   8           9  10              11  12  13  14  15  16      17  18  19  20  21
         std::vector<int> s1 = {50, 51, 52, 53, 54, 55, 56,     57, 58,         59, 60,             61, 62, 63, 64, 65, 66,     67, 68, 69, 70, 71};
         std::vector<int> s2 = {50, 99, 52, 99,     55, 99, 99, 57, 99, 99, 99, 59, 99, 99, 99, 99, 61, 99, 99, 64, 99, 99, 99, 67, 99, 99,     71};
         //                      0   1   2   3       4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25      26
         Alignment al1(s1, s2);
         Alignment al2(s2, s1);

         _TestCase testCase[] = {_TestCase(al1, true), _TestCase(al2, false)};
         for (size_t i = 0; i < sizeof(testCase) / sizeof(_TestCase); ++i)
         {
            const _TestCase & tc = testCase[i];
            // (!valueMax);
            BOOST_REQUIRE(tc.al.index(1, tc.keyIsFirst) == 1);
            BOOST_REQUIRE(tc.al.index(3, tc.keyIsFirst) == 3);
            BOOST_REQUIRE(tc.al.index(4, tc.keyIsFirst) == 3);

            // (!keyMax);
            BOOST_REQUIRE(tc.al.index(6, tc.keyIsFirst) == 5);
            BOOST_REQUIRE(tc.al.index(8, tc.keyIsFirst) == 9);
            BOOST_REQUIRE(tc.al.index(10, tc.keyIsFirst) == 13);

            // (keyMax == valueMax);
            BOOST_REQUIRE(tc.al.index(12, tc.keyIsFirst) == 17);
            BOOST_REQUIRE(tc.al.index(13, tc.keyIsFirst) == 18);

            // Resolve proportion;
            BOOST_REQUIRE(tc.al.index(15, tc.keyIsFirst) == 20);
            BOOST_REQUIRE(tc.al.index(16, tc.keyIsFirst) == 22);
            BOOST_REQUIRE(tc.al.index(18, tc.keyIsFirst) == 24);
            BOOST_REQUIRE(tc.al.index(19, tc.keyIsFirst) == 25);
            BOOST_REQUIRE(tc.al.index(20, tc.keyIsFirst) == 25);
         }
      }
   }

   { // (prev);
      { // !(valueSize > (prev->value + 1));
         //                      0   1
         std::vector<int> s1 = {50, 51};
         std::vector<int> s2 = {50};
         //                      0
         Alignment al1(s1, s2);
         Alignment al2(s2, s1);

         BOOST_REQUIRE(al1.index(1, true) == 0);
         BOOST_REQUIRE(al2.index(1, false) == 0);
      }

      { // (valueSize > (prev->value + 1));
         { // (!valueMax);
            //                      0   1   2   3
            std::vector<int> s1 = {50, 51, 52, 53};
            std::vector<int> s2 = {50, 99};
            //                      0   1
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(1, true) == 1);
            BOOST_REQUIRE(al1.index(2, true) == 1);
            BOOST_REQUIRE(al1.index(3, true) == 1);

            BOOST_REQUIRE(al2.index(1, false) == 1);
            BOOST_REQUIRE(al2.index(2, false) == 1);
            BOOST_REQUIRE(al2.index(3, false) == 1);
         }

         { // (!keyMax);
            //                      0   1
            std::vector<int> s1 = {50, 51};
            std::vector<int> s2 = {50, 99, 99, 99};
            //                      0   1   2   3
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(1, true) == 2);
            BOOST_REQUIRE(al2.index(1, false) == 2);
         }

         { // (keyMax == valueMax);
            //                      0   1   2   3
            std::vector<int> s1 = {50, 51, 52, 53};
            std::vector<int> s2 = {50, 99, 99, 99};
            //                      0   1   2   3
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(1, true) == 1);
            BOOST_REQUIRE(al1.index(2, true) == 2);
            BOOST_REQUIRE(al1.index(3, true) == 3);

            BOOST_REQUIRE(al2.index(1, false) == 1);
            BOOST_REQUIRE(al2.index(2, false) == 2);
            BOOST_REQUIRE(al2.index(3, false) == 3);
         }

         { // Resolve proportion;
            {
               //                      0   1   2
               std::vector<int> s1 = {50, 51, 52};
               std::vector<int> s2 = {50, 99, 99, 99};
               //                      0   1   2   3
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(1, true) == 1);
               BOOST_REQUIRE(al1.index(2, true) == 3);

               BOOST_REQUIRE(al2.index(1, false) == 1);
               BOOST_REQUIRE(al2.index(2, false) == 3);
            }

            {
               //                      0   1   2
               std::vector<int> s1 = {50, 51, 52};
               std::vector<int> s2 = {50, 99, 99, 99, 99};
               //                      0   1   2   3   4
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(1, true) == 1);
               BOOST_REQUIRE(al1.index(2, true) == 4);

               BOOST_REQUIRE(al2.index(1, false) == 1);
               BOOST_REQUIRE(al2.index(2, false) == 4);
            }

            {
               //                      0   1   2   3
               std::vector<int> s1 = {50, 51, 52, 53};
               std::vector<int> s2 = {50, 99, 99};
               //                      0   1   2
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(1, true) == 1);
               BOOST_REQUIRE(al1.index(2, true) == 2);
               BOOST_REQUIRE(al1.index(3, true) == 2);

               BOOST_REQUIRE(al2.index(1, false) == 1);
               BOOST_REQUIRE(al2.index(2, false) == 2);
               BOOST_REQUIRE(al2.index(3, false) == 2);
            }

            {
               //                      0   1   2   3   4
               std::vector<int> s1 = {50, 51, 52, 53, 54};
               std::vector<int> s2 = {50, 99, 99};
               //                      0   1   2
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(1, true) == 1);
               BOOST_REQUIRE(al1.index(2, true) == 1);
               BOOST_REQUIRE(al1.index(3, true) == 2);
               BOOST_REQUIRE(al1.index(4, true) == 2);

               BOOST_REQUIRE(al2.index(1, false) == 1);
               BOOST_REQUIRE(al2.index(2, false) == 1);
               BOOST_REQUIRE(al2.index(3, false) == 2);
               BOOST_REQUIRE(al2.index(4, false) == 2);
            }
         }
      }
   }

   { // (next);
      { // !(next->value > 0);
         //                      0   1
         std::vector<int> s1 = {50, 51};
         std::vector<int> s2 =     {51};
         //                          0
         Alignment al1(s1, s2);
         Alignment al2(s2, s1);

         BOOST_REQUIRE(al1.index(0, true) == 0);
         BOOST_REQUIRE(al2.index(0, false) == 0);
      }

      { // (next->value > 0);
         { // (!valueMax);
            //                      0   1   2   3
            std::vector<int> s1 = {50, 51, 52, 53};
            std::vector<int> s2 =         {99, 53};
            //                              0   1
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(0, true) == 0);
            BOOST_REQUIRE(al1.index(1, true) == 0);
            BOOST_REQUIRE(al1.index(2, true) == 0);

            BOOST_REQUIRE(al2.index(0, false) == 0);
            BOOST_REQUIRE(al2.index(1, false) == 0);
            BOOST_REQUIRE(al2.index(2, false) == 0);
         }

         { // (!keyMax);
            //                              0   1
            std::vector<int> s1 =         {50, 51};
            std::vector<int> s2 = {99, 99, 99, 51};
            //                      0   1   2   3
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(0, true) == 1);
            BOOST_REQUIRE(al2.index(0, false) == 1);
         }

         { // (keyMax == valueMax);
            //                      0   1   2   3
            std::vector<int> s1 = {50, 51, 52, 53};
            std::vector<int> s2 = {99, 99, 99, 53};
            //                      0   1   2   3
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(0, true) == 0);
            BOOST_REQUIRE(al1.index(1, true) == 1);
            BOOST_REQUIRE(al1.index(2, true) == 2);

            BOOST_REQUIRE(al2.index(0, false) == 0);
            BOOST_REQUIRE(al2.index(1, false) == 1);
            BOOST_REQUIRE(al2.index(2, false) == 2);
         }

         { // Resolve proportion;
            {
               //                          0   1   2   3
               std::vector<int> s1 =     {50, 51, 52, 53};
               std::vector<int> s2 = {99, 99, 99, 99, 53};
               //                      0   1   2   3   4
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(0, true) == 0);
               BOOST_REQUIRE(al1.index(1, true) == 2);
               BOOST_REQUIRE(al1.index(2, true) == 3);

               BOOST_REQUIRE(al2.index(0, false) == 0);
               BOOST_REQUIRE(al2.index(1, false) == 2);
               BOOST_REQUIRE(al2.index(2, false) == 3);
            }

            {
               //                              0   1   2   3
               std::vector<int> s1 =         {50, 51, 52, 53};
               std::vector<int> s2 = {99, 99, 99, 99, 99, 53};
               //                      0   1   2   3   4   5
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(0, true) == 0);
               BOOST_REQUIRE(al1.index(1, true) == 2);
               BOOST_REQUIRE(al1.index(2, true) == 4);

               BOOST_REQUIRE(al2.index(0, false) == 0);
               BOOST_REQUIRE(al2.index(1, false) == 2);
               BOOST_REQUIRE(al2.index(2, false) == 4);
            }

            {
               //                      0   1   2   3   4
               std::vector<int> s1 = {50, 51, 52, 53, 54};
               std::vector<int> s2 =     {99, 99, 99, 54};
               //                          0   1   2   3
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(0, true) == 0);
               BOOST_REQUIRE(al1.index(1, true) == 1);
               BOOST_REQUIRE(al1.index(2, true) == 1);
               BOOST_REQUIRE(al1.index(3, true) == 2);

               BOOST_REQUIRE(al2.index(0, false) == 0);
               BOOST_REQUIRE(al2.index(1, false) == 1);
               BOOST_REQUIRE(al2.index(2, false) == 1);
               BOOST_REQUIRE(al2.index(3, false) == 2);
            }

            {
               //                      0   1   2   3   4   5
               std::vector<int> s1 = {50, 51, 52, 53, 54, 55};
               std::vector<int> s2 =         {99, 99, 99, 55};
               //                              0   1   2   3
               Alignment al1(s1, s2);
               Alignment al2(s2, s1);

               BOOST_REQUIRE(al1.index(0, true) == 0);
               BOOST_REQUIRE(al1.index(1, true) == 1);
               BOOST_REQUIRE(al1.index(2, true) == 1);
               BOOST_REQUIRE(al1.index(3, true) == 2);
               BOOST_REQUIRE(al1.index(4, true) == 2);

               BOOST_REQUIRE(al2.index(0, false) == 0);
               BOOST_REQUIRE(al2.index(1, false) == 1);
               BOOST_REQUIRE(al2.index(2, false) == 1);
               BOOST_REQUIRE(al2.index(3, false) == 2);
               BOOST_REQUIRE(al2.index(4, false) == 2);
            }
         }
      }
   }

   { // (!prev && !next);
      { // (!valueMax);
         //                      0   1   2
         std::vector<int> s1 = {50, 51, 52};
         std::vector<int> s2 =     {99};
         //                          0
         Alignment al1(s1, s2);
         Alignment al2(s2, s1);

         BOOST_REQUIRE(al1.index(0, true) == 0);
         BOOST_REQUIRE(al1.index(1, true) == 0);
         BOOST_REQUIRE(al1.index(2, true) == 0);

         BOOST_REQUIRE(al2.index(0, false) == 0);
         BOOST_REQUIRE(al2.index(1, false) == 0);
         BOOST_REQUIRE(al2.index(2, false) == 0);
      }

      { // (!keyMax);
         //                          0
         std::vector<int> s1 =     {50};
         std::vector<int> s2 = {99, 99, 99};
         //                      0   1   2
         Alignment al1(s1, s2);
         Alignment al2(s2, s1);

         BOOST_REQUIRE(al1.index(0, true) == 1);
         BOOST_REQUIRE(al2.index(0, false) == 1);
      }

      { // (keyMax == valueMax);
         //                      0   1   2
         std::vector<int> s1 = {50, 51, 52};
         std::vector<int> s2 = {99, 99, 99};
         //                      0   1   2
         Alignment al1(s1, s2);
         Alignment al2(s2, s1);

         BOOST_REQUIRE(al1.index(0, true) == 0);
         BOOST_REQUIRE(al1.index(1, true) == 1);
         BOOST_REQUIRE(al1.index(2, true) == 2);

         BOOST_REQUIRE(al2.index(0, false) == 0);
         BOOST_REQUIRE(al2.index(1, false) == 1);
         BOOST_REQUIRE(al2.index(2, false) == 2);
      }

      { // Resolve proportion;
         {
            //                          0   1   2
            std::vector<int> s1 =     {50, 51, 52};
            std::vector<int> s2 = {99, 99, 99, 99};
            //                      0   1   2   3
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(0, true) == 0);
            BOOST_REQUIRE(al1.index(1, true) == 2);
            BOOST_REQUIRE(al1.index(2, true) == 3);

            BOOST_REQUIRE(al2.index(0, false) == 0);
            BOOST_REQUIRE(al2.index(1, false) == 2);
            BOOST_REQUIRE(al2.index(2, false) == 3);
         }

         {
            //                              0   1   2
            std::vector<int> s1 =         {50, 51, 52};
            std::vector<int> s2 = {99, 99, 99, 99, 99};
            //                      0   1   2   3   4
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(0, true) == 0);
            BOOST_REQUIRE(al1.index(1, true) == 2);
            BOOST_REQUIRE(al1.index(2, true) == 4);

            BOOST_REQUIRE(al2.index(0, false) == 0);
            BOOST_REQUIRE(al2.index(1, false) == 2);
            BOOST_REQUIRE(al2.index(2, false) == 4);
         }

         {
            //                      0   1   2   3
            std::vector<int> s1 = {50, 51, 52, 53};
            std::vector<int> s2 =     {99, 99, 99};
            //                          0   1   2
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(0, true) == 0);
            BOOST_REQUIRE(al1.index(1, true) == 1);
            BOOST_REQUIRE(al1.index(2, true) == 1);
            BOOST_REQUIRE(al1.index(3, true) == 2);

            BOOST_REQUIRE(al2.index(0, false) == 0);
            BOOST_REQUIRE(al2.index(1, false) == 1);
            BOOST_REQUIRE(al2.index(2, false) == 1);
            BOOST_REQUIRE(al2.index(3, false) == 2);
         }

         {
            //                      0   1   2   3   4
            std::vector<int> s1 = {50, 51, 52, 53, 54};
            std::vector<int> s2 =         {99, 99, 99};
            //                              0   1   2
            Alignment al1(s1, s2);
            Alignment al2(s2, s1);

            BOOST_REQUIRE(al1.index(0, true) == 0);
            BOOST_REQUIRE(al1.index(1, true) == 1);
            BOOST_REQUIRE(al1.index(2, true) == 1);
            BOOST_REQUIRE(al1.index(3, true) == 2);
            BOOST_REQUIRE(al1.index(4, true) == 2);
         }
      }
   }
}


BOOST_AUTO_TEST_SUITE_END()
