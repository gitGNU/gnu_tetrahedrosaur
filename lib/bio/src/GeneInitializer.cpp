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


#include "Config.hpp"
#include "GeneInitializer.hpp"
#include "InstructionSet.hpp"
#include "Organism.hpp"


namespace {

bio::I8x3GeneParam _i8x3GeneParam(const bio::Instruction & instr)
{
   const int16_t ang = instr.i16().get();
   return bio::I8x3GeneParam(
      instr.i8(),
      dt::Int8(0xFF & ang),
      dt::Int8(0xFF & (ang >> 8))
   );
}

} // anonymous namespace;


namespace bio {


/***************************************************************************
 *   GeneInitializer structure implementation                              *
 ***************************************************************************/


GeneInitializer::GeneInitializer()
   : commit(0), rollback(0)
{
}


void GeneInitializer::append(const Instruction & instr, const Config & config)
{
   switch (config.opcode(instr.cmd()))
   {
      case OP_NOP:
      case OP_PROM:
         break;
      case OP_CGEGEN:
         generationGreaterOrEqual.add(instr.u16());
         break;
      case OP_NGEGEN:
         generationGreaterOrEqual.add_nop();
         break;
      case OP_CLEGEN:
         generationLessOrEqual.add(instr.u16());
         break;
      case OP_NLEGEN:
         generationLessOrEqual.add_nop();
         break;
      case OP_CGEX:
         xGreaterOrEqual.add(instr.i16());
         break;
      case OP_NGEX:
         xGreaterOrEqual.add_nop();
         break;
      case OP_CLEX:
         xLessOrEqual.add(instr.i16());
         break;
      case OP_NLEX:
         xLessOrEqual.add_nop();
         break;
      case OP_CEQX:
         xEqual.add(instr.i16());
         break;
      case OP_NEQX:
         xEqual.add_nop();
         break;
      case OP_CGEY:
         yGreaterOrEqual.add(instr.i16());
         break;
      case OP_NGEY:
         yGreaterOrEqual.add_nop();
         break;
      case OP_CLEY:
         yLessOrEqual.add(instr.i16());
         break;
      case OP_NLEY:
         yLessOrEqual.add_nop();
         break;
      case OP_CEQY:
         yEqual.add(instr.i16());
         break;
      case OP_NEQY:
         yEqual.add_nop();
         break;
      case OP_RCOMM:
         ++commit;
         break;
      case OP_RROLL:
         ++rollback;
         break;
      case OP_RBABC:
         abcBud.add(_i8x3GeneParam(instr));
         break;
      case OP_RNBABC:
         abcBud.add_nop();
         break;
      case OP_RBACD:
         acdBud.add(_i8x3GeneParam(instr));
         break;
      case OP_RNBACD:
         acdBud.add_nop();
         break;
      case OP_RBADB:
         adbBud.add(_i8x3GeneParam(instr));
         break;
      case OP_RNBADB:
         adbBud.add_nop();
         break;
      case OP_RBBCD:
         bcdBud.add(_i8x3GeneParam(instr));
         break;
      case OP_RNBBCD:
         bcdBud.add_nop();
         break;
   }
}


}
