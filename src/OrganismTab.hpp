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


#ifndef ORGANISMTAB_HPP
#define ORGANISMTAB_HPP


#include <boost/shared_ptr.hpp>


#include <QtCore/QtGlobal>
QT_FORWARD_DECLARE_CLASS(QToolButton)


#include "MainWindowTab.hpp"
class CellModel;
class EdgeModel;
struct GuiOrganismDesc;
class PropertyView;
class TriangleModel;
class VertexModel;
class OrganismViewport;


#include "datatypes/mesh.hpp"


class OrganismTab : public MainWindowTab
{
   Q_OBJECT

   public:
      explicit OrganismTab(
         boost::shared_ptr<const GuiOrganismDesc> desc,
         QWidget * parent = 0
      );
      virtual ~OrganismTab();

   public slots:
      void setSelectionMode(dt::SelectionMode mode);
      void showInterior(bool shown);
      void showNormals(bool shown);

   private slots:
      void playOrPause();
      void makeBud();
      void updateProperties();

   private:
      void updatePlayPauseButton();

      CellModel * m_cellModel;
      EdgeModel * m_edgeModel;
      TriangleModel * m_triangleModel;
      VertexModel * m_vertexModel;
      PropertyView * m_propertyView;
      OrganismViewport * m_viewport;
      QToolButton * m_playPauseButton;
      QToolButton * m_budButton;
};


#endif
