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


#ifndef BIO_CHROMOSOME_HPP
#define BIO_CHROMOSOME_HPP


#include <initializer_list>
#include <ostream>
#include <vector>


#include "InstructionSet.hpp"


namespace bio {


struct MutationParams;


/***************************************************************************
 *   Chromosome class declaration                                          *
 ***************************************************************************/


class Chromosome
{
   public:
      explicit Chromosome();
      explicit Chromosome(const std::vector<Instruction> & code);
      explicit Chromosome(std::initializer_list<Instruction> list);

      inline const std::vector<Instruction> & code() const;

      void applyMutations(const MutationParams & params);

   private:
      std::vector<Instruction> m_code;
};


inline const std::vector<Instruction> & Chromosome::code() const
{
   return m_code;
}


std::ostream & operator<<(std::ostream & os, const Chromosome & chr);


}


#endif
