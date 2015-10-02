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


#ifndef ORGANISMMENU_HPP
#define ORGANISMMENU_HPP


#include <QtCore/QtGlobal>


QT_FORWARD_DECLARE_CLASS(QAction)


#include "MainWindowMenuBar.hpp"


#include "datatypes/mesh.hpp"


class OrganismMenu : public MainWindowMenuBar
{
   Q_OBJECT

   public:
      explicit OrganismMenu(QWidget * parent = 0);
      virtual ~OrganismMenu();

   signals:
      void cellDivided();
      void selectionModeChanged(dt::SelectionMode selectionMode);
      void interiorShown(bool shown);
      void normalsShown(bool shown);

   private slots:
      void setSelectionMode(QAction * action);
      void showInterior();
      void showNormals();

   private:
      void createMenuMenu();
      void createSelectionMenu();
      void createViewMenu();

      bool m_interiorShown;
      bool m_normalsShown;
      QAction * m_vertexSelectionAction;
      QAction * m_edgeSelectionAction;
      QAction * m_triangleSelectionAction;
      QAction * m_tetrahedronSelectionAction;
      QAction * m_showInteriorAction;
      QAction * m_showNormalsAction;
};


#endif
