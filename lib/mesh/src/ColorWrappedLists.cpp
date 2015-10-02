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


#include <new>


#include "ColorWrappedLists.hpp"
#include "MemoryModification.hpp"


namespace mesh {


/***************************************************************************
 *   ColorWrappedLists::iterator class implementation                      *
 ***************************************************************************/


ColorWrappedLists::iterator::iterator()
   : m_container(0), m_indexOfFirstComponent(-1)
{
}


bool ColorWrappedLists::iterator::isValid() const
{
   return (m_container && m_indexOfFirstComponent >= 0);
}


GLint ColorWrappedLists::iterator::color() const
{
   if (isValid())
   {
      return (m_indexOfFirstComponent / m_container->m_colorComponents);
   }

   return -1;
}


ColorWrappedLists::iterator & ColorWrappedLists::iterator::operator++()
{
   if (isValid())
   {
      m_indexOfFirstComponent = m_container->m_data[
         m_indexOfFirstComponent + m_container->m_colorComponents - 1
      ] * m_container->m_colorComponents;
   }
   return *this;
}


GLint ColorWrappedLists::iterator::get(int component) const
{
   if (isValid() && component >= 0 &&
      component < m_container->m_colorComponents - 1)
   {
      return m_container->m_data[m_indexOfFirstComponent + component];
   }

   return -1;
}


void ColorWrappedLists::iterator::set(int component, GLint value) const
{
   if (isValid() && component >= 0 &&
      component < m_container->m_colorComponents - 1)
   {
      m_container->logModifiedColor(color());
      m_container->m_data[m_indexOfFirstComponent + component] = value;
   }
}


ColorWrappedLists::iterator::iterator(
   ColorWrappedLists * container,
   GLint firstColor
) : m_container(container), m_indexOfFirstComponent(-1)
{
   if (container)
   {
      m_indexOfFirstComponent = firstColor * container->m_colorComponents;
   }
}


/***************************************************************************
 *   ColorWrappedLists::const_iterator class implementation                *
 ***************************************************************************/


ColorWrappedLists::const_iterator::const_iterator()
   : m_container(0), m_indexOfFirstComponent(-1)
{
}


ColorWrappedLists::const_iterator::const_iterator(const iterator & other)
   : m_container(other.m_container),
   m_indexOfFirstComponent(other.m_indexOfFirstComponent)
{
}


bool ColorWrappedLists::const_iterator::isValid() const
{
   return (m_container && m_indexOfFirstComponent >= 0);
}


GLint ColorWrappedLists::const_iterator::color() const
{
   if (isValid())
   {
      return (m_indexOfFirstComponent / m_container->m_colorComponents);
   }

   return -1;
}


ColorWrappedLists::const_iterator &
ColorWrappedLists::const_iterator::operator++()
{
   if (isValid())
   {
      m_indexOfFirstComponent = m_container->m_data[
         m_indexOfFirstComponent + m_container->m_colorComponents - 1
      ] * m_container->m_colorComponents;
   }
   return *this;
}


GLint ColorWrappedLists::const_iterator::get(int component) const
{
   if (isValid() && component >= 0 &&
      component < m_container->m_colorComponents - 1)
   {
      return m_container->m_data[m_indexOfFirstComponent + component];
   }

   return -1;
}


ColorWrappedLists::const_iterator::const_iterator(
   const ColorWrappedLists * container,
   GLint firstColor
) : m_container(container), m_indexOfFirstComponent(-1)
{
   if (container)
   {
      m_indexOfFirstComponent = firstColor * container->m_colorComponents;
   }
}


/***************************************************************************
 *   ColorWrappedLists class implementation                                *
 ***************************************************************************/


ColorWrappedLists::ColorWrappedLists(
   GLint colorCount,
   COLOR_COMPONENTS colorComponents
   ) : m_colorCount(0),
   m_colorComponents(colorComponents),
   m_data(0),
   m_firstFreeColor(0),
   m_memModLogging(false)
{
   m_memMod.reset(new MemoryModification);

   GLint size = colorCount * colorComponents;
   m_data = new (std::nothrow) GLint[size];
   if (m_data)
   {
      m_colorCount = colorCount;

      // Mark all the colors as free;
      GLint index = size - 1;
      for (GLint color = 0, nextColor = -1; color < colorCount; color ++)
      {
         m_data[index --] = nextColor;
         nextColor = colorCount - color - 1;

         for (int item = 0; item < colorComponents - 1; item ++)
         {
            m_data[index --] = -1;
         }
      }
   }
}


ColorWrappedLists::~ColorWrappedLists()
{
   delete[] m_data;
}


ColorWrappedLists::iterator ColorWrappedLists::begin(GLint firstColor)
{
   iterator it;
   if (firstColor >= 0 && firstColor < m_colorCount)
   {
      return iterator(this, firstColor);
   }
   return iterator();
}


ColorWrappedLists::const_iterator ColorWrappedLists::begin(
   GLint firstColor
) const
{
   const_iterator it;
   if (firstColor >= 0 && firstColor < m_colorCount)
   {
      return const_iterator(this, firstColor);
   }
   return const_iterator();
}


ColorWrappedLists::iterator ColorWrappedLists::createFirstColor()
{
   GLint color = takeFreeColor();
   if (color < 0)
   {
      return iterator();
   }

   return iterator(this, color);
}


ColorWrappedLists::iterator ColorWrappedLists::insertAfter(iterator it)
{
   if (!it.isValid())
   {
      return iterator();
   }

   GLint newColor = takeFreeColor();
   if (newColor < 0)
   {
      return iterator();
   }

   // Insert new color and set it's first item;
   GLint indexOfLastComponentOfIt = indexOfLastComponent(it.color());
   m_data[indexOfLastComponent(newColor)] = m_data[indexOfLastComponentOfIt];
   m_data[indexOfLastComponentOfIt] = newColor;

   logModifiedColor(it.color());

   return iterator(this, newColor);
}


GLint ColorWrappedLists::removeFirst(GLint firstColor)
{
   if (firstColor >= 0)
   {
      GLint colorToBeRemoved = firstColor;
      firstColor = m_data[indexOfLastComponent(colorToBeRemoved)];
      freeColor(colorToBeRemoved);
      return firstColor;
   }

   return -1;
}


ColorWrappedLists::iterator ColorWrappedLists::removeAfter(
   ColorWrappedLists::iterator it
)
{
   if (!it.isValid())
   {
      return iterator();
   }

   GLint indexOfLastComponentOfIt = indexOfLastComponent(it.color());
   GLint nextColor = m_data[indexOfLastComponentOfIt];

   if (nextColor >= 0)
   {
      GLint nextNextColor = m_data[indexOfLastComponent(nextColor)];
      logModifiedColor(it.color());
      m_data[indexOfLastComponentOfIt] = nextNextColor;
      freeColor(nextColor);
      return iterator(this, nextNextColor);
   }

   return iterator();
}


void ColorWrappedLists::removeAll(GLint firstColor)
{
   while ((firstColor = removeFirst(firstColor)) >= 0);
}


void ColorWrappedLists::clearMemoryModificationLog()
{
   m_memMod->clear();
}


void ColorWrappedLists::logModifiedColor(GLint color)
{
   if (m_memModLogging)
   {
      m_memMod->insertArrayElement(color, sizeof(GLint) * m_colorComponents);
   }
}


GLint ColorWrappedLists::takeFreeColor()
{
   GLint freeColor = m_firstFreeColor;
   if (freeColor >= 0)
   {
      logModifiedColor(freeColor);
      GLint indexOfLastComponentOfFreeColor = indexOfLastComponent(freeColor);
      m_firstFreeColor = m_data[indexOfLastComponentOfFreeColor];
      m_data[indexOfLastComponentOfFreeColor] = -1;
   }

   return freeColor;
}


void ColorWrappedLists::freeColor(GLint color)
{
   if (color >= 0)
   {
      GLint index = color * m_colorComponents;
      for (int component = 0; component < m_colorComponents - 1; component ++)
      {
         m_data[index ++] = -1; // Clear color component;
      }

      logModifiedColor(color);
      m_data[index] = m_firstFreeColor;
      m_firstFreeColor = color;
   }
}


}
