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


#ifndef ALGO_RANDOM_GENERATORS_HPP
#define ALGO_RANDOM_GENERATORS_HPP


#include <cstdint>
#include <vector>


namespace algo {


/***************************************************************************
 *   UniformUInt16RandomGenerator class declaration                        *
 ***************************************************************************/


class UniformUInt16RandomGenerator
{
   public:
      explicit UniformUInt16RandomGenerator(uint16_t upperLimit);
      explicit UniformUInt16RandomGenerator(
         const std::vector<uint16_t> & predefinedValues
      );

      uint16_t generate() const;

   private:
      uint16_t m_upperLimit;
      std::vector<uint16_t> m_predefinedValues;
      mutable size_t m_predefinedValueIndex;
};


/***************************************************************************
 *   UniformNormalizedFloatRandomGenerator class declaration               *
 ***************************************************************************/


class UniformNormalizedFloatRandomGenerator
{
   public:
      explicit UniformNormalizedFloatRandomGenerator();
      explicit UniformNormalizedFloatRandomGenerator(
         const std::vector<float> & predefinedValues
      );

      float generate() const;

   private:
      std::vector<float> m_predefinedValues;
      mutable size_t m_predefinedValueIndex;
};


}


#endif
