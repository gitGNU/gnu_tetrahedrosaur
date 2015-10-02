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
#include "Gene.hpp"
#include "GeneInitializer.hpp"
#include "InstructionSet.hpp"


namespace {

bio::RangeCondition<dt::Int16> _getRangeCondition(
   const bio::AverageGeneI16Param & greaterOrEqual,
   const bio::AverageGeneI16Param & lessOrEqual,
   const bio::AverageGeneI16Param & equal
)
{
   const boost::optional<dt::Int16> ge = greaterOrEqual.value();
   const boost::optional<dt::Int16> le = lessOrEqual.value();
   const boost::optional<dt::Int16> eq = equal.value();
   if (ge || le || eq)
   {
      if (equal.isStronger(greaterOrEqual) && equal.isStronger(lessOrEqual))
      {
         return bio::RangeCondition<dt::Int16>(eq, eq);
      }
      return bio::RangeCondition<dt::Int16>(ge, le);
   }
   return bio::RangeCondition<dt::Int16>();
}

} // anonymous namespace;


namespace bio {


/***************************************************************************
 *   Gene structure implementation                                         *
 ***************************************************************************/


Gene::Gene(const Config & config, const GeneInitializer & init)
   : generationCondition(
      init.generationGreaterOrEqual.value(),
      init.generationLessOrEqual.value()
   ),
   xCondition(std::move(_getRangeCondition(
      init.xGreaterOrEqual,
      init.xLessOrEqual,
      init.xEqual
   ))),
   yCondition(std::move(_getRangeCondition(
      init.yGreaterOrEqual,
      init.yLessOrEqual,
      init.yEqual
   )))
{
   if (init.rollback > init.commit)
   {
      return;
   }

   // Responses;

   { // ABC face;
      if (const boost::optional<I8x3GeneParam> bud = init.abcBud.value())
      {
         responses.push_back(Instruction(config.cmd(OP_RBABC),
            bud->a.get(), bud->b.get(), bud->c.get()
         ));
      }
      else if (init.abcBud.nopCount())
      {
         responses.push_back(Instruction(config.cmd(OP_RNBABC), 0, 0));
      }
   }

   { // ACD face;
      if (const boost::optional<I8x3GeneParam> bud = init.acdBud.value())
      {
         responses.push_back(Instruction(config.cmd(OP_RBACD),
            bud->a.get(), bud->b.get(), bud->c.get()
         ));
      }
      else if (init.abcBud.nopCount())
      {
         responses.push_back(Instruction(config.cmd(OP_RNBACD), 0, 0));
      }
   }

   { // ADB face;
      if (const boost::optional<I8x3GeneParam> bud = init.adbBud.value())
      {
         responses.push_back(Instruction(config.cmd(OP_RBADB),
            bud->a.get(), bud->b.get(), bud->c.get()
         ));
      }
      else if (init.abcBud.nopCount())
      {
         responses.push_back(Instruction(config.cmd(OP_RNBADB), 0, 0));
      }
   }

   { // BCD face;
      if (const boost::optional<I8x3GeneParam> bud = init.bcdBud.value())
      {
         responses.push_back(Instruction(config.cmd(OP_RBBCD),
            bud->a.get(), bud->b.get(), bud->c.get()
         ));
      }
      else if (init.abcBud.nopCount())
      {
         responses.push_back(Instruction(config.cmd(OP_RNBBCD), 0, 0));
      }
   }
}


Gene::~Gene()
{
}


bool Gene::hasResponse() const
{
   return !responses.empty();
}


std::ostream & operator<<(std::ostream & os, const Gene & gene)
{
   /*os << "{" << cell.v0() << ", " << cell.v1() << ", " <<
      cell.v2() << ", " << cell.v3() << "}";*/
   return os;
}


}
