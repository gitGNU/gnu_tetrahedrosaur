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


#ifndef BIO_CONFIG_HPP
#define BIO_CONFIG_HPP


#include <array>
#include <map>


#include "datatypes/geometry.hpp"
#include "datatypes/numeric.hpp"


#include "InstructionSet.hpp"


namespace bio {


/***************************************************************************
 *   Configurable struct declaration                                       *
 ***************************************************************************/


struct Configurable
{
public:
   explicit Configurable(const dt::Float & min, const dt::Float & max);

   dt::Float convert(const dt::Int8 & param) const;
   dt::Float convert(const dt::Int16 & param) const;
   dt::Float convert(const dt::UInt8 & param) const;
   dt::Float convert(const dt::UInt16 & param) const;

private:
   dt::Float m_length;
   dt::Float m_min;
   dt::Float m_middle;
};


/***************************************************************************
 *   Config struct declaration                                             *
 ***************************************************************************/


struct Config
{
   explicit Config();
   explicit Config(
      const Configurable & budTopRadius,
      const Configurable & budTopPolarAngle,
      const Configurable & budTopAzimuthalAngle,
      const std::map<Opcode, uint8_t> & opcodeDistribution
   );

   Opcode opcode(uint8_t cmd) const;
   uint8_t cmd(Opcode opcode) const;

   Configurable budTopRadius;
   Configurable budTopPolarAngle;
   Configurable budTopAzimuthalAngle;
   std::map<Opcode, uint8_t> opcodeDistribution;
   std::array<Opcode, 256> opcodeArray;
};


}


#endif
