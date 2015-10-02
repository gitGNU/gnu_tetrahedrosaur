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
#include <cstdlib>


#include "Chromosome.hpp"


namespace bio {


/***************************************************************************
 *   Chromosome class implementation                                       *
 ***************************************************************************/


Chromosome::Chromosome()
{
}


Chromosome::Chromosome(const std::vector<Instruction> & code)
   : m_code(code)
{
}


Chromosome::Chromosome(std::initializer_list<Instruction> list)
   : m_code(list)
{
}


void Chromosome::applyMutations()
{
   std::vector<Instruction> newCode;
   newCode.reserve(m_code.size());
   for (const Instruction & instr : m_code)
   {
      if ((rand() % 100) < 5)
      {
         newCode.push_back(Instruction(rand(), rand(), rand()));
      }
      newCode.push_back(((rand() % 100) < 5) ? instr.mutated() : instr);
   }

   if ((rand() % 100) < 5)
   {
      newCode.push_back(Instruction(rand(), rand(), rand()));
   }
   m_code = newCode; // FIXME: Optimization needed;
}


std::ostream & operator<<(std::ostream & os, const Chromosome & chr)
{
   const std::vector<Instruction> & code = chr.code();
   for (size_t i = 0, count = code.size(); i < count; ++i)
   {
      os << "Instruction(" << code[i] << ")";
      if (i + 1 < count)
      {
         os << "," << std::endl;
      }
   }
   return os;
}


}
