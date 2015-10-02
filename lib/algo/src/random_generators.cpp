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


#include <cmath>
#include <cstdlib>


#include "random_generators.hpp"


namespace algo {


/***************************************************************************
 *   UniformUInt16RandomGenerator class implementation                     *
 ***************************************************************************/


UniformUInt16RandomGenerator::UniformUInt16RandomGenerator(uint16_t upperLimit)
   : m_upperLimit(upperLimit), m_predefinedValueIndex(0)
{
}


UniformUInt16RandomGenerator::UniformUInt16RandomGenerator(
   const std::vector<uint16_t> & predefinedValues
) : m_upperLimit(0),
   m_predefinedValues(predefinedValues),
   m_predefinedValueIndex(0)
{
}


uint16_t UniformUInt16RandomGenerator::generate() const
{
   if (m_predefinedValueIndex < m_predefinedValues.size())
   {
      return m_predefinedValues[m_predefinedValueIndex++];
   }
   return static_cast<uint16_t>(m_upperLimit ? (rand() % m_upperLimit) : 0);
}


/***************************************************************************
 *   UniformNormalizedFloatRandomGenerator class implementation            *
 ***************************************************************************/


UniformNormalizedFloatRandomGenerator::UniformNormalizedFloatRandomGenerator()
   : m_predefinedValueIndex(0)
{
}


UniformNormalizedFloatRandomGenerator::UniformNormalizedFloatRandomGenerator(
   const std::vector<float> & predefinedValues
) : m_predefinedValues(predefinedValues),
   m_predefinedValueIndex(0)
{
}


float UniformNormalizedFloatRandomGenerator::generate() const
{
   if (m_predefinedValueIndex < m_predefinedValues.size())
   {
      return m_predefinedValues[m_predefinedValueIndex++];
   }
   return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}


}
