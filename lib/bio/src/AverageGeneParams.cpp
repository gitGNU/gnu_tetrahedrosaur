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


#include "AverageGeneParams.hpp"


namespace bio {


/***************************************************************************
 *   BaseAverageGeneParams class implementation                            *
 ***************************************************************************/


BaseAverageGeneParams::BaseAverageGeneParams()
   : m_count(0), m_nopCount(0)
{
}


bool BaseAverageGeneParams::isStronger(
   const BaseAverageGeneParams & other
) const
{
   bool thisGo = go();
   bool otherGo = other.go();
   if (thisGo && otherGo)
   {
      return (m_count > other.m_count);
   }
   else if (thisGo && !otherGo)
   {
      return true;
   }
   return false;
}


bool BaseAverageGeneParams::go() const
{
   return (m_count && (m_count >= m_nopCount));
}


/***************************************************************************
 *   AverageGeneI8I8I8Param class implementation                           *
 ***************************************************************************/


I8x3GeneParam::I8x3GeneParam(
   const dt::Int8 & a,
   const dt::Int8 & b,
   const dt::Int8 & c
) : a(a), b(b), c(c)
{
}


I8x3GeneParamSum::I8x3GeneParamSum()
   : a(0), b(0), c(0)
{
}


template <>
AverageGeneParams<I8x3GeneParam, I8x3GeneParamSum>::AverageGeneParams()
   : BaseAverageGeneParams(), m_sum()
{
}


template <>
void AverageGeneParams<I8x3GeneParam, I8x3GeneParamSum>::add(
   const I8x3GeneParam & value
)
{
   m_sum.a += value.a.get();
   m_sum.b += value.b.get();
   m_sum.c += value.c.get();
   ++m_count;
}


template <>
boost::optional<I8x3GeneParam> AverageGeneParams<I8x3GeneParam,
   I8x3GeneParamSum>::value() const
{
   if (go())
   {
      return I8x3GeneParam(
         dt::Int8(m_sum.a / static_cast<int32_t>(m_count)),
         dt::Int8(m_sum.b / static_cast<int32_t>(m_count)),
         dt::Int8(m_sum.c / static_cast<int32_t>(m_count))
      );
   }
   return boost::none;
}


}
