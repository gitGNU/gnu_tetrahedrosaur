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


#include <ostream>
#include <sstream>
#include <string>


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
      std::string toString() const;
      inline boost::optional<T> greaterOrEqual() const {return m_greaterOrEqual;}
      inline boost::optional<T> lessOrEqual() const {return m_lessOrEqual;}

   private:
      boost::optional<T> m_greaterOrEqual;
      boost::optional<T> m_lessOrEqual;
};


template <typename T>
std::ostream & operator<<(std::ostream & os, const RangeCondition<T> & rc);


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


template <typename T>
std::string RangeCondition<T>::toString() const
{
   std::ostringstream stream;
   stream << *this;
   return stream.str();
}


template <typename T>
std::ostream & operator<<(std::ostream & os, const RangeCondition<T> & rc)
{
   const boost::optional<T> ge = rc.greaterOrEqual();
   const boost::optional<T> le = rc.lessOrEqual();
   if (ge && le)
   {
      if (*ge > *le)
      {
         os << "(.., " << *le << "] || [" << *ge << ", ..)";
      }
      else
      {
         os << "[" << *ge << ", " << *le << "]";
      }
   }
   else if (ge)
   {
      os << "[" << *ge << ", ..)";
   }
   else if (le)
   {
      os << "(.., " << *le << "]";
   }
   return os;
}


} // namespace bio;


#endif
