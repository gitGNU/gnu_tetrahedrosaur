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


#ifndef BIO_RANGECONDITION_HPP
#define BIO_RANGECONDITION_HPP


#include <boost/optional.hpp>


namespace bio {


/***************************************************************************
 *   RangeCondition class declaration                                      *
 ***************************************************************************/


template <typename T>
class RangeCondition
{
   public:
      explicit RangeCondition();
      explicit RangeCondition(
         const boost::optional<T> &greaterOrEqual,
         const boost::optional<T> &lessOrEqual
      );
      bool isAcceptable(const T & value) const;
      bool isAlwaysAcceptable() const;

   private:
      boost::optional<T> m_greaterOrEqual;
      boost::optional<T> m_lessOrEqual;
};


/***************************************************************************
 *   RangeCondition class implementation                                   *
 ***************************************************************************/


template <typename T>
RangeCondition<T>::RangeCondition()
{
}


template <typename T>
RangeCondition<T>::RangeCondition(
   const boost::optional<T> &greaterOrEqual,
   const boost::optional<T> &lessOrEqual
) : m_greaterOrEqual(greaterOrEqual), m_lessOrEqual(lessOrEqual)
{
}


template <typename T>
bool RangeCondition<T>::isAcceptable(const T & value) const
{
   if (m_greaterOrEqual && m_lessOrEqual)
   {
      if (*m_greaterOrEqual > *m_lessOrEqual)
      {
         return (value >= *m_greaterOrEqual || value <= *m_lessOrEqual);
      }
      return (value >= *m_greaterOrEqual && value <= *m_lessOrEqual);
   }
   else if (m_greaterOrEqual)
   {
      return (value >= *m_greaterOrEqual);
   }
   else if (m_lessOrEqual)
   {
      return (value <= *m_lessOrEqual);
   }
   return true;
}


template <typename T>
bool RangeCondition<T>::isAlwaysAcceptable() const
{
   return (!m_greaterOrEqual && !m_lessOrEqual);
}


} // namespace bio;


#endif
