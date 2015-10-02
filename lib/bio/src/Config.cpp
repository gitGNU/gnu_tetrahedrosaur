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


#include "Config.hpp"


namespace {

std::array<bio::Opcode, 256> _opcodeArray(
   const std::map<bio::Opcode, uint8_t> & opcodeDistribution
)
{
   std::array<bio::Opcode, 256> ar;
   size_t arIndex = 0;
   const size_t count = bio::Instruction::instructionCount();
   for (size_t i = 0; i < count; ++i)
   {
      const bio::Opcode opcode = static_cast<bio::Opcode>(i);
      const auto it = opcodeDistribution.find(opcode);
      if (it != opcodeDistribution.end())
      {
         for (size_t j = 0; ((j < it->second) && (arIndex < 256)); ++j)
         {
            ar[arIndex++] = opcode;
         }
      }
   }

   // Fill the tail;
   while (arIndex < 256)
   {
      ar[arIndex++] = bio::OP_NOP;
   }
   return ar;
}

} // anonymous namespace;


namespace bio {


/***************************************************************************
 *   Configurable struct implementation                                    *
 ***************************************************************************/


Configurable::Configurable(const dt::Float & min, const dt::Float & max)
{
   if (max >= min)
   {
      m_min = min;
      m_length = max - min;
      m_middle = min + (m_length / static_cast<dt::Float>(2));
   }
   else
   {
      m_min = max;
      m_length = min - max;
      m_middle = max + (m_length / static_cast<dt::Float>(2));
   }
}


dt::Float Configurable::convert(const dt::Int8 & param) const
{
   const dt::Float delta = m_length / static_cast<dt::Float>(0x100);
   return (m_middle + delta * static_cast<dt::Float>(param.get()));
}


dt::Float Configurable::convert(const dt::Int16 & param) const
{
   const dt::Float delta = m_length / static_cast<dt::Float>(0x10000);
   return (m_middle + delta * static_cast<dt::Float>(param.get()));
}


dt::Float Configurable::convert(const dt::UInt8 & param) const
{
   const dt::Float delta = m_length / static_cast<dt::Float>(0x100);
   return (m_min + delta * static_cast<dt::Float>(param.get()));
}


dt::Float Configurable::convert(const dt::UInt16 & param) const
{
   const dt::Float delta = m_length / static_cast<dt::Float>(0x10000);
   return (m_min + delta * static_cast<dt::Float>(param.get()));
}


/***************************************************************************
 *   Config struct implementation                                          *
 ***************************************************************************/


Config::Config()
   : budTopRadius(0.2f, 2.2f),
   budTopPolarAngle(0.0f, 170.0f * (M_PI / 360.0f)),
   budTopAzimuthalAngle(0.0f, 2.0f * M_PI)
{
   opcodeDistribution = {
      {OP_NOP, 11},
      {OP_PROM, 11},
      {OP_CGEGEN, 9},
      {OP_NGEGEN, 9},
      {OP_CLEGEN, 9},
      {OP_NLEGEN, 9},
      {OP_CGEX, 9},
      {OP_NGEX, 9},
      {OP_CLEX, 9},
      {OP_NLEX, 9},
      {OP_CEQX, 9},
      {OP_NEQX, 9},
      {OP_CGEY, 9},
      {OP_NGEY, 9},
      {OP_CLEY, 9},
      {OP_NLEY, 9},
      {OP_CEQY, 9},
      {OP_NEQY, 9},
      {OP_RCOMM, 9},
      {OP_RROLL, 9},
      {OP_RBABC, 9},
      {OP_RNBABC, 9},
      {OP_RBACD, 9},
      {OP_RNBACD, 9},
      {OP_RBADB, 9},
      {OP_RNBADB, 9},
      {OP_RBBCD, 9},
      {OP_RNBBCD, 9}
   };
   opcodeArray = std::move(_opcodeArray(opcodeDistribution));
}


Config::Config(
   const Configurable & budTopRadius,
   const Configurable & budTopPolarAngle,
   const Configurable & budTopAzimuthalAngle,
   const std::map<Opcode, uint8_t> & opcodeDistribution
) : budTopRadius(budTopRadius),
   budTopPolarAngle(budTopPolarAngle),
   budTopAzimuthalAngle(budTopAzimuthalAngle),
   opcodeDistribution(opcodeDistribution),
   opcodeArray(_opcodeArray(opcodeDistribution))
{
}


Opcode Config::opcode(uint8_t cmd) const
{
   return opcodeArray[cmd];
}


uint8_t Config::cmd(Opcode opcode) const
{
   for (size_t i = 0; i < 256; ++i)
   {
      if (opcodeArray[i] == opcode)
      {
         return i;
      }
   }
   return 0;
}


}
