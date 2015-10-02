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


#ifndef ORGANISMVIEWPORT_HPP
#define ORGANISMVIEWPORT_HPP


#include <GL/gl.h>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>


#include "Viewport.hpp"


#include "datatypes/mesh.hpp"
namespace bio {
class Organism;
struct OrganismDesc;
}


class EdgeTool3D;
class Organism3D;


/***************************************************************************
 *   OrganismViewport class declaration                                    *
 ***************************************************************************/


class OrganismViewport : public Viewport
{
   Q_OBJECT

   public:
      explicit OrganismViewport(
         boost::shared_ptr<const bio::OrganismDesc> desc,
         QWidget *parent = 0
      );
      virtual ~OrganismViewport();

      inline bool isPaused() const {return m_pause;}
      void pause(bool paused);
      void showInterior(bool shown);
      void showNormals(bool shown);

      boost::shared_ptr<bio::Organism> organism() const;
      inline bool paused() const {return m_pause;}
      inline bool interiorShown() const {return m_showInterior;}
      inline bool normalsShown() const {return m_showNormals;}

   public slots:
      void setSelectionMode(dt::SelectionMode selectionMode);

   protected:
      virtual void timerEvent(QTimerEvent * event);
      virtual void wheelEvent(QWheelEvent * event);
      virtual void initializeGL();
      virtual void paintGL();

   signals:
      void selectionChanged();
      void edgeResized(float delta);

   private:
      virtual void handleLeftMouseButtonPress(int x, int y);
      boost::optional<dt::VertexId> pickVertex(int x, int y) const;

      bool m_pause;
      dt::SelectionMode m_selectionMode;
      bool m_showInterior;
      bool m_showNormals;
      int m_idleTimerId;
      boost::scoped_ptr<EdgeTool3D> m_edgeTool;
      boost::shared_ptr<const bio::OrganismDesc> m_desc;
      boost::shared_ptr<Organism3D> m_organism;
      GLuint m_framebufferForPicking;
      GLuint m_renderbufferForPicking;
};


#endif
