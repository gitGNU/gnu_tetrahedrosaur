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


#ifndef ORGANISM3D_H
#define ORGANISM3D_H


#include <cstdlib>
#include <GL/gl.h>


#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include "datatypes/geometry.hpp"
#include "datatypes/mesh.hpp"

namespace bio {
class Organism;
struct OrganismDesc;
}


class EdgeTool3D;
class Scene;


/***************************************************************************
 *   Organism3D class declaration                                          *
 ***************************************************************************/


class Organism3D
{
   public:
      enum PAINTING_MODE
      {
         PM_VIEW = 0,
         PM_EDIT,
         PM_PICK_VERTEX
      };

      enum SELECTED_PART
      {
         SP_NONE = 0,
         SP_VERTEX,
         SP_EDGE
      };

      explicit Organism3D(boost::shared_ptr<const bio::OrganismDesc> desc);
      virtual ~Organism3D();

      void stepOver();

      void paintGL(
         const Scene * scene,
         PAINTING_MODE mode,
         bool showInterior,
         bool showNormals
      ) const;

      void paintEdgeTool(
         const Scene * scene,
         const EdgeTool3D * tool
      ) const;

      void selectVertex(
         const boost::optional<dt::VertexId> & v,
         dt::SelectionMode selectionMode = dt::SM_Vertex
      );

      inline boost::shared_ptr<bio::Organism> organism() const;

      SELECTED_PART selectedPart() const;

   private:
      boost::shared_ptr<bio::Organism> m_organism;
};


inline boost::shared_ptr<bio::Organism> Organism3D::organism() const
{
   return m_organism;
}


#endif
