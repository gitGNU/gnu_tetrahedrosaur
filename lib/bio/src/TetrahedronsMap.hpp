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


#ifndef BIO_TETRAHEDRONSMAP_H
#define BIO_TETRAHEDRONSMAP_H


#include <cstdlib>
#include <map>
#include <ostream>


#include <boost/optional.hpp>


#include "mesh/Tetrahedron.hpp"


namespace bio {


class Cell;


/***************************************************************************
 *   TetrahedronsMap class declaration                                     *
 ***************************************************************************/


class TetrahedronsMap
{
   public:
      typedef std::map<mesh::Tetrahedron, Cell *>::const_iterator const_iterator;
      typedef std::map<mesh::Tetrahedron, Cell *>::iterator iterator;

      virtual ~TetrahedronsMap();

      inline const_iterator begin() const
      {
         return m_map.begin();
      }

      inline iterator begin()
      {
         return m_map.begin();
      }

      inline const_iterator end() const
      {
         return m_map.end();
      }

      inline iterator end()
      {
         return m_map.end();
      }

      inline bool empty() const
      {
         return m_map.empty();
      }

      inline size_t size() const
      {
         return m_map.size();
      }

      inline const_iterator find(const mesh::Tetrahedron & t) const
      {
         return m_map.find(t);
      }

      Cell * findAny(const mesh::Tetrahedron & t) const;

      inline void insert(const mesh::Tetrahedron & t, Cell * cell)
      {
         m_map.insert(std::pair<mesh::Tetrahedron, Cell *>(t, cell));
      }

      void remove(const mesh::Tetrahedron & t);

      void replaceVertexIndex(size_t oldIndex, size_t newIndex);

    private:
        std::map<mesh::Tetrahedron, Cell *> m_map;
};


std::ostream & operator<<(std::ostream & os, const TetrahedronsMap & tm);


}


#endif
