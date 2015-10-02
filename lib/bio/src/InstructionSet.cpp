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


#include <cstdlib>
#include <iomanip>
#include <sstream>


#include "Config.hpp"
#include "InstructionSet.hpp"


namespace bio {


static Instruction::Desc _instructionSet[] = {
   {OP_NOP, IT_NOP, "NOP", Instruction::P_NONE},
   {OP_PROM, IT_PROMOTER, "PROM", Instruction::P_NONE},

   {OP_CGEGEN, IT_CONDITION, "CGEGEN", Instruction::P_UINT16},
   {OP_NGEGEN, IT_CONDITION, "NGEGEN", Instruction::P_NONE},
   {OP_CLEGEN, IT_CONDITION, "CLEGEN", Instruction::P_UINT16},
   {OP_NLEGEN, IT_CONDITION, "NLEGEN", Instruction::P_NONE},
   {OP_CGEX, IT_CONDITION, "CGEX", Instruction::P_INT16},
   {OP_NGEX, IT_CONDITION, "NGEX", Instruction::P_INT16},
   {OP_CLEX, IT_CONDITION, "CLEX", Instruction::P_INT16},
   {OP_NLEX, IT_CONDITION, "NLEX", Instruction::P_INT16},
   {OP_CEQX, IT_CONDITION, "CEQX", Instruction::P_INT16},
   {OP_NEQX, IT_CONDITION, "NEQX", Instruction::P_INT16},
   {OP_CGEY, IT_CONDITION, "CGEY", Instruction::P_INT16},
   {OP_NGEY, IT_CONDITION, "NGEY", Instruction::P_INT16},
   {OP_CLEY, IT_CONDITION, "CLEY", Instruction::P_INT16},
   {OP_NLEY, IT_CONDITION, "NLEY", Instruction::P_INT16},
   {OP_CEQY, IT_CONDITION, "CEQY", Instruction::P_INT16},
   {OP_NEQY, IT_CONDITION, "NEQY", Instruction::P_INT16},

   {OP_RCOMM, IT_RESPONSE, "RCOMM", Instruction::P_NONE},
   {OP_RROLL, IT_RESPONSE, "RROLL", Instruction::P_NONE},
   {OP_RBABC, IT_RESPONSE, "RBABC", Instruction::P_UINT8_UINT16},
   {OP_RNBABC, IT_RESPONSE, "RNBABC", Instruction::P_NONE},
   {OP_RBACD, IT_RESPONSE, "RBACD", Instruction::P_UINT8_UINT16},
   {OP_RNBACD, IT_RESPONSE, "RNBACD", Instruction::P_NONE},
   {OP_RBADB, IT_RESPONSE, "RBADB", Instruction::P_UINT8_UINT16},
   {OP_RNBADB, IT_RESPONSE, "RNBADB", Instruction::P_NONE},
   {OP_RBBCD, IT_RESPONSE, "RBBCD", Instruction::P_UINT8_UINT16},
   {OP_RNBBCD, IT_RESPONSE, "RNBBCD", Instruction::P_NONE}
};


Instruction::Instruction()
   : m_cmd(0), m_u8(0), m_u16(0)
{
}


Instruction::Instruction(uint8_t cmd, uint8_t u8, uint16_t u16)
   : m_cmd(cmd), m_u8(u8), m_u16(u16)
{
}


Instruction::Instruction(uint8_t cmd, int8_t a, int8_t b, int8_t c)
   : m_cmd(cmd),
   m_u8(a),
   m_u16((static_cast<uint16_t>(b) << 8) | (static_cast<uint16_t>(c) & 0xff))
{
}


const Instruction::Desc & Instruction::descriptor(Opcode opcode)
{
   return _instructionSet[opcode];
}


size_t Instruction::instructionCount()
{
   return sizeof(_instructionSet) / sizeof(Desc);
}


Instruction Instruction::mutated() const
{
   uint32_t raw = (m_u16 << 16) | (m_u8 << 8) | m_cmd;
   const uint32_t mask = (0x00000001 << (rand() % 32));
   raw = raw ^ mask; // Invert bit;
   return Instruction(raw, raw << 8, raw << 16);
}


InstructionEquals::InstructionEquals(boost::shared_ptr<const Config> config)
   : m_config(config)
{
}

bool InstructionEquals::operator()(
   const Instruction & lhs,
   const Instruction & rhs
) const
{
   const Opcode lhsOpcode = m_config->opcode(lhs.cmd());
   const Opcode rhsOpcode = m_config->opcode(rhs.cmd());
   if (lhsOpcode == rhsOpcode)
   {
      uint8_t f = _instructionSet[lhsOpcode].params;
      return (!(f & Instruction::P_INT8) || lhs.u8() == rhs.u8()) &&
         (!(f & Instruction::P_INT16) || lhs.u16() == rhs.u16());
   }
   return false;
}


std::string InstructionEquals::toString(const Instruction & instr) const
{
   const Opcode opcode = m_config->opcode(instr.cmd());
   const Instruction::Desc & desc = _instructionSet[opcode];
   std::ostringstream oss;
   oss << desc.mnemonic << std::setfill('0');
   if (desc.params & Instruction::P_INT8)
   {
      oss << " ";
      if (desc.params & Instruction::P_UINT8)
      {
         oss << static_cast<unsigned int>(instr.u8().get());
      }
      else
      {
         oss << static_cast<int>(instr.u8().get());
      }
   }
   if (desc.params & Instruction::P_INT16)
   {
      oss << " ";
      if (desc.params & Instruction::P_UINT16)
      {
         oss << static_cast<unsigned int>(instr.u16().get());
      }
      else
      {
         oss << static_cast<int>(instr.u16().get());
      }
   }
   return oss.str();
}


std::ostream & operator<<(std::ostream & os, const Instruction & instruction)
{
   std::ostringstream oss;
   oss << std::setfill('0') << static_cast<unsigned int>(instruction.cmd());
   oss << ", ";
   oss << static_cast<unsigned int>(instruction.u8().get());
   oss << ", ";
   oss << static_cast<unsigned int>(instruction.u16().get());
   os << oss.str();
   return os;
}


}
