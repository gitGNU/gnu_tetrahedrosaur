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


#ifndef DATATYPES_STRONGTYPE_HPP
#define DATATYPES_STRONGTYPE_HPP


#define STRONGTYPE_DECLARATION(_WeakType, _StrongType) \
   class _StrongType \
   { \
      public: \
         typedef _WeakType WeakType; \
         explicit _StrongType(const _WeakType & value) : m_value(value) {} \
         inline _WeakType get() const {return m_value;} \
         inline bool operator==(const _StrongType & other) const \
         {return (m_value == other.m_value);} \
         inline bool operator!=(const _StrongType & other) const \
         {return (m_value != other.m_value);} \
         inline bool operator<(const _StrongType & other) const \
         {return (m_value < other.m_value);} \
         inline bool operator<=(const _StrongType & other) const \
         {return (m_value <= other.m_value);} \
         inline bool operator>(const _StrongType & other) const \
         {return (m_value > other.m_value);} \
         inline bool operator>=(const _StrongType & other) const \
         {return (m_value >= other.m_value);} \
      private: \
         _WeakType m_value; \
   }; \


#endif
