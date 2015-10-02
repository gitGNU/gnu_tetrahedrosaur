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


#ifndef ALGO_ROOMMATES_HPP
#define ALGO_ROOMMATES_HPP


#include <cstring>
#include <vector>


namespace algo {


namespace roommates {


typedef size_t PersonType;


template <typename T>
class Matrix
{
   public:
      explicit Matrix(size_t size);
      Matrix(const Matrix & other);
      Matrix & operator=(const Matrix & other);
      Matrix(Matrix && other);
      Matrix & operator=(Matrix && other);
      virtual ~Matrix();

      inline size_t size() const {return m_size;}

      T * operator[](size_t index);
      const T * operator[](size_t index) const;

   private:
      void free();
      void init(const Matrix & other);

      size_t m_size;
      T ** m_data;
};


template <typename T>
Matrix<T>::Matrix(size_t size)
   : m_size(size), m_data(0)
{
   if (m_size)
   {
      m_data = new T *[m_size];
      for (size_t i = 0; i < m_size; ++i)
      {
         m_data[i] = new T[m_size];
      }
   }
}


template <typename T>
Matrix<T>::Matrix(const Matrix & other)
   : m_size(0), m_data(0)
{
   init(other);
}


template <typename T>
Matrix<T> & Matrix<T>::operator=(const Matrix & other)
{
   if (this != &other)
   {
      free();
      init(other);
   }
   return *this;
}


template <typename T>
Matrix<T>::Matrix(Matrix && other)
   : m_size(other.m_size), m_data(other.m_data)
{
   other.m_size = 0;
   other.m_data = 0;
}


template <typename T>
Matrix<T> & Matrix<T>::operator=(Matrix && other)
{
   if (this != &other)
   {
      free();
      m_size = other.m_size;
      m_data = other.m_data;
      other.m_size = 0;
      other.m_data = 0;
   }
   return *this;
}


template <typename T>
Matrix<T>::~Matrix()
{
   free();
}


template <typename T>
T * Matrix<T>::operator[](size_t index)
{
   return m_data[index];
}


template <typename T>
const T * Matrix<T>::operator[](size_t index) const
{
   return m_data[index];
}


template <typename T>
void Matrix<T>::free()
{
   for (size_t i = 0; i < m_size; ++i)
   {
      delete[] m_data[i];
   }
   delete[] m_data;
   m_size = 0;
   m_data = 0;
}


template <typename T>
void Matrix<T>::init(const Matrix<T> & other)
{
   m_size = other.m_size;
   m_data = 0;
   if (m_size)
   {
      m_data = new T *[m_size];
      for (size_t i = 0; i < m_size; ++i)
      {
         m_data[i] = new T[m_size];
         memcpy(m_data[i], other.m_data[i], sizeof(T) * m_size);
      }
   }
}


bool resolve(
   Matrix<PersonType> & preference,
   std::vector<PersonType> & partners
);


} // namespace roommates;


} // namespace algo;


#endif
