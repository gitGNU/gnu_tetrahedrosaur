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


#ifndef BIO_INSTRUCTIONSET_H
#define BIO_INSTRUCTIONSET_H


#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>


#include <boost/shared_ptr.hpp>


#include "datatypes/numeric.hpp"


namespace bio {


struct Config;
struct MutationParams;


enum InstructionType
{
   IT_NOP = 0,
   IT_PROMOTER = 1,
   IT_CONDITION = 2,
   IT_RESPONSE = 3
};


enum Opcode {
   OP_NOP = 0,// No operation;
   OP_PROM,   // Promoter;

   OP_CGEGEN, // Generation of cell is greater than or equal to u16;
   OP_NGEGEN, // Do not check whether generation of cell is greater than or equal to something;
   OP_CLEGEN, // Generation of cell is less then or equal to u16;
   OP_NLEGEN, // Do not check whether generation of cell is less than or equal to something;
   OP_CGEX,   // X coordinate of cell is greater then or equal to i16;
   OP_NGEX,   // Do not check whether X coordinate of cell is greater than or equal to something;
   OP_CLEX,   // X coordinate of cell is less then or equal to i16;
   OP_NLEX,   // Do not check whether X coordinate of cell is less than or equal to something;
   OP_CEQX,   // X coordinate of cell is equal to i16;
   OP_NEQX,   // Do not check whether X coordinate of cell is equal to something;
   OP_CGEY,   // Y coordinate of cell is greater then or equal to i16;
   OP_NGEY,   // Do not check whether Y coordinate of cell is greater than or equal to something;
   OP_CLEY,   // Y coordinate of cell is less then or equal to i16;
   OP_NLEY,   // Do not check whether Y coordinate of cell is less than or equal to something;
   OP_CEQY,   // Y coordinate of cell is equal to i16;
   OP_NEQY,   // Do not check whether Y coordinate of cell is equal to something;
   // TODO: ABC face has/hasn't bud;
   // TODO: ACD face has/hasn't bud;
   // TODO: ADB face has/hasn't bud;
   // TODO: BCD face has/hasn't bud;

   OP_RCOMM,  // Commit response;
   OP_RROLL,  // Rollback response;
   OP_RBABC,  // Make bud from ABC face;
   OP_RNBABC, // Do not make bud from ABC face;
   OP_RBACD,  // Make bud from ACD face;
   OP_RNBACD, // Do not make bud from ACD face;
   OP_RBADB,  // Make bud from ADB face;
   OP_RNBADB, // Do not make bud from ADB face;
   OP_RBBCD,  // Make bud from BCD face;
   OP_RNBBCD  // Do not make bud from BCD face;
};


#pragma pack(push)
#pragma pack(1)


class Instruction
{
   public:
      enum Params
      {
         P_NONE = 0x0,
         P_INT8 = 0x1,
         P_UINT8 = P_INT8 | 0x4,
         P_INT16 = 0x2,
         P_UINT16 = P_INT16 | 0x8,
         P_INT8_INT16 = P_INT8 | P_INT16,
         P_UINT8_INT16 = P_UINT8 | P_INT16,
         P_INT8_UINT16 = P_INT8 | P_UINT16,
         P_UINT8_UINT16 = P_UINT8 | P_UINT16
      };

      struct Desc
      {
         Opcode opcode;
         InstructionType type;
         const char * mnemonic;
         Instruction::Params params;
      };

      explicit Instruction();
      explicit Instruction(uint8_t cmd, uint8_t u8, uint16_t u16);
      explicit Instruction(uint8_t cmd, int8_t a, int8_t b, int8_t c);

      static const Desc & descriptor(Opcode opcode);
      static size_t instructionCount();

      Instruction mutated(const MutationParams & params) const;

      inline uint8_t cmd() const {return m_cmd;}
      inline dt::Int8 i8() const {return dt::Int8(m_u8);}
      inline dt::UInt8 u8() const {return dt::UInt8(m_u8);}
      inline dt::Int16 i16() const {return dt::Int16(m_u16);}
      inline dt::UInt16 u16() const {return dt::UInt16(m_u16);}

   private:
      uint8_t m_cmd;
      uint8_t m_u8;
      uint16_t m_u16;
};


class InstructionEquals
{
   public:
      explicit InstructionEquals(boost::shared_ptr<const Config> config);

      bool operator()(const Instruction & lhs, const Instruction & rhs) const;

      std::string toString(const Instruction & instr) const;

   private:
      boost::shared_ptr<const Config> m_config;
};


#pragma pack(pop)


std::ostream & operator<<(std::ostream & os, const Instruction & instruction);


}


#endif
