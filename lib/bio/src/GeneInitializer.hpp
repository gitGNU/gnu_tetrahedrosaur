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


#ifndef BIO_GENEINITIALIZER_H
#define BIO_GENEINITIALIZER_H


#include <array>
#include <string>
#include <cstdint>


#include "AverageGeneParams.hpp"


namespace bio {


class Instruction;
struct Config;


/***************************************************************************
 *   GeneInitializer structure declaration                                 *
 ***************************************************************************/


struct GeneInitializer
{
   explicit GeneInitializer();

   void append(const Instruction & instr, const Config & config);

   // Conditions;
   AverageGeneU16Param generationGreaterOrEqual;
   AverageGeneU16Param generationLessOrEqual;
   AverageGeneI16Param xGreaterOrEqual;
   AverageGeneI16Param xLessOrEqual;
   AverageGeneI16Param xEqual;
   AverageGeneI16Param yGreaterOrEqual;
   AverageGeneI16Param yLessOrEqual;
   AverageGeneI16Param yEqual;

   // Responses;
   uint32_t commit;
   uint32_t rollback;
   AverageGeneI8x3Param abcBud;
   AverageGeneI8x3Param acdBud;
   AverageGeneI8x3Param adbBud;
   AverageGeneI8x3Param bcdBud;
};


}


#endif
