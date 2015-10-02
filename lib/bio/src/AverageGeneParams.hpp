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


#ifndef BIO_AVERAGEGENEPARAMS_H
#define BIO_AVERAGEGENEPARAMS_H


#include <cstdint>


#include <boost/optional.hpp>


#include "datatypes/numeric.hpp"


namespace bio {


/***************************************************************************
 *   BaseAverageGeneParams class declaration                               *
 ***************************************************************************/


class BaseAverageGeneParams
{
   public:
      explicit BaseAverageGeneParams();

      inline void add_nop() {++m_nopCount;}
      inline uint32_t nopCount() const {return m_nopCount;}

      bool isStronger(const BaseAverageGeneParams & other) const;

   protected:
      bool go() const;

      uint32_t m_count;

   private:
      uint32_t m_nopCount;
};


/***************************************************************************
 *   AverageGeneParams class declaration and implementation                *
 ***************************************************************************/


template <typename StrongT, typename SumT>
class AverageGeneParams : public BaseAverageGeneParams
{
   public:
      explicit AverageGeneParams();

      void add(const StrongT & value);
      boost::optional<StrongT> value() const;

   private:
      SumT m_sum;
};


template <typename StrongT, typename SumT>
AverageGeneParams<StrongT, SumT>::AverageGeneParams()
   : BaseAverageGeneParams(), m_sum(0)
{
}


template <typename StrongT, typename SumT>
void AverageGeneParams<StrongT, SumT>::add(const StrongT & value)
{
   m_sum += value.get();
   ++m_count;
}


template <typename StrongT, typename SumT>
boost::optional<StrongT> AverageGeneParams<StrongT, SumT>::value() const
{
   if (go())
   {
      return StrongT(m_sum / static_cast<SumT>(m_count));
   }
   return boost::none;
}


/***************************************************************************
 *   AverageGeneI8x3Param class declaration and implementation             *
 ***************************************************************************/


struct I8x3GeneParam
{
   explicit I8x3GeneParam(
      const dt::Int8 & a,
      const dt::Int8 & b,
      const dt::Int8 & c
   );

   dt::Int8 a;
   dt::Int8 b;
   dt::Int8 c;
};


struct I8x3GeneParamSum
{
   explicit I8x3GeneParamSum();

   int32_t a;
   int32_t b;
   int32_t c;
};


typedef AverageGeneParams<I8x3GeneParam, I8x3GeneParamSum> AverageGeneI8x3Param;


template <>
AverageGeneParams<I8x3GeneParam, I8x3GeneParamSum>::AverageGeneParams();


template <>
void AverageGeneParams<I8x3GeneParam, I8x3GeneParamSum>::add(
   const I8x3GeneParam & value
);


template <>
boost::optional<I8x3GeneParam> AverageGeneParams<I8x3GeneParam,
   I8x3GeneParamSum>::value() const;


/***************************************************************************
 *   AverageGeneI16Param class declaration                                 *
 ***************************************************************************/


typedef AverageGeneParams<dt::Int16, int32_t> AverageGeneI16Param;


/***************************************************************************
 *   AverageGeneU16Param class declaration                                 *
 ***************************************************************************/


typedef AverageGeneParams<dt::UInt16, uint32_t> AverageGeneU16Param;


}


#endif
