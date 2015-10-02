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


#ifndef COLORWRAPPEDLISTS_H
#define COLORWRAPPEDLISTS_H


#include <GL/gl.h>


#include <boost/shared_ptr.hpp>


namespace mesh {


class MemoryModification;


/***************************************************************************
 *   ColorWrappedLists class declaration                                   *
 ***************************************************************************/


class ColorWrappedLists
{
   public:
      enum COLOR_COMPONENTS
      {
         CC_RGB = 3,
         CC_RGBA = 4
      };

      class iterator
      {
         friend class ColorWrappedLists;

         public:
            iterator();

            bool isValid() const;
            GLint color() const;

            iterator & operator++();

            GLint get(int component) const;
            void set(int component, GLint value) const;

            inline bool operator==(const iterator & other) const
            {
               return (m_container == other.m_container &&
                  m_indexOfFirstComponent == other.m_indexOfFirstComponent
               );
            }

            inline bool operator!=(const iterator & other) const
            {
               return (!this->operator==(other));
            }

         protected:
            iterator(ColorWrappedLists * container, GLint firstColor);

         private:
            ColorWrappedLists * m_container;
            GLint m_indexOfFirstComponent;
      };

      class const_iterator
      {
         friend class ColorWrappedLists;

         public:
            const_iterator();
            const_iterator(const iterator & other);

            bool isValid() const;
            GLint color() const;

            const_iterator & operator++();

            GLint get(int component) const;

            inline bool operator==(const const_iterator & other) const
            {
               return (m_container == other.m_container &&
                  m_indexOfFirstComponent == other.m_indexOfFirstComponent
               );
            }

            inline bool operator!=(const const_iterator & other) const
            {
               return (!this->operator==(other));
            }

         protected:
            const_iterator(
               const ColorWrappedLists * container,
               GLint firstColor
            );

         private:
            const ColorWrappedLists * m_container;
            GLint m_indexOfFirstComponent;
      };

      ColorWrappedLists(GLint colorCount, COLOR_COMPONENTS colorComponents);
      virtual ~ColorWrappedLists();

      inline GLint colorCount() const
      {
         return m_colorCount;
      }
      inline COLOR_COMPONENTS colorComponents() const
      {
         return m_colorComponents;
      }

      inline const GLint * data() const
      {
         return m_data;
      }

      inline GLint firstFreeColor() const
      {
         return m_firstFreeColor;
      }

      iterator begin(GLint firstColor);
      const_iterator begin(GLint firstColor) const;

      iterator createFirstColor();
      iterator insertAfter(iterator it);

      GLint removeFirst(GLint firstColor);
      iterator removeAfter(iterator it);
      void removeAll(GLint firstColor);

      void clearMemoryModificationLog();
      inline void startMemoryModificationLogging()
      {
         clearMemoryModificationLog();
         m_memModLogging = true;
      }
      inline void stopMemoryModificationLogging()
      {
         m_memModLogging = false;
      }
      inline boost::shared_ptr<MemoryModification> memoryModification() const
      {
         return m_memMod;
      }

   private:
      void logModifiedColor(GLint color);

      inline GLint indexOfLastComponent(GLint color)
      {
         return ((color + 1) * m_colorComponents - 1);
      }

      GLint takeFreeColor();
      void freeColor(GLint color);

      GLint m_colorCount;
      COLOR_COMPONENTS m_colorComponents;
      GLint * m_data;
      GLint m_firstFreeColor;
      bool m_memModLogging;
      boost::shared_ptr<MemoryModification> m_memMod;
};


}


#endif
