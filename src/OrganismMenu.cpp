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


#include <QtCore/QtGlobal>


static const char * _context = "OrganismMenu";
static const char * _divideCell = QT_TRANSLATE_NOOP("OrganismMenu", "Divide cell");
static const char * _menu = QT_TRANSLATE_NOOP("OrganismMenu", "Menu");
static const char * _showInterior = QT_TRANSLATE_NOOP("OrganismMenu", "Show interior");
static const char * _hideInterior = QT_TRANSLATE_NOOP("OrganismMenu", "Hide interior");
static const char * _showNormals = QT_TRANSLATE_NOOP("OrganismMenu", "Show normals");
static const char * _hideNormals = QT_TRANSLATE_NOOP("OrganismMenu", "Hide normals");
static const char * _view = QT_TRANSLATE_NOOP("OrganismMenu", "View");
static const char * _selection = QT_TRANSLATE_NOOP("OrganismMenu", "Selection");
static const char * _vertex = QT_TRANSLATE_NOOP("OrganismMenu", "Vertex");
static const char * _edge = QT_TRANSLATE_NOOP("OrganismMenu", "Edge");
static const char * _triangle = QT_TRANSLATE_NOOP("OrganismMenu", "Triangle");
static const char * _tetrahedronCell = QT_TRANSLATE_NOOP("OrganismMenu", "Tetrahedron / Cell");


#include <QtGui/QKeySequence>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#include <QtWidgets/QMenu>


#include "OrganismMenu.hpp"
#include "translation.hpp"


OrganismMenu::OrganismMenu(QWidget * parent)
   : MainWindowMenuBar(parent),
   m_interiorShown(false),
   m_normalsShown(false),
   m_vertexSelectionAction(0),
   m_edgeSelectionAction(0),
   m_triangleSelectionAction(0),
   m_tetrahedronSelectionAction(0),
   m_showInteriorAction(0),
   m_showNormalsAction(0)
{
   createMenuMenu();
   createSelectionMenu();
   createViewMenu();
}


OrganismMenu::~OrganismMenu()
{
}


void OrganismMenu::setSelectionMode(QAction * action)
{
   if (action == m_vertexSelectionAction)
   {
      emit selectionModeChanged(dt::SM_Vertex);
   }
   else if (action == m_edgeSelectionAction)
   {
      emit selectionModeChanged(dt::SM_Edge);
   }
   else if (action == m_triangleSelectionAction)
   {
      emit selectionModeChanged(dt::SM_Triangle);
   }
   else if (action == m_tetrahedronSelectionAction)
   {
      emit selectionModeChanged(dt::SM_Tetrahedron);
   }
}


void OrganismMenu::showInterior()
{
   m_interiorShown = !m_interiorShown;
   m_showInteriorAction->setText(TSLC(
      m_interiorShown ? _hideInterior : _showInterior
   ));
   emit interiorShown(m_interiorShown);
}


void OrganismMenu::showNormals()
{
   m_normalsShown = !m_normalsShown;
   m_showNormalsAction->setText(TSLC(
      m_normalsShown ? _hideNormals : _showNormals
   ));
   emit normalsShown(m_normalsShown);
}


void OrganismMenu::createMenuMenu()
{
   QAction * divideCellAction = new QAction(TSLC(_divideCell), this);
   divideCellAction->setShortcut(QKeySequence("s"));
   connect(divideCellAction, SIGNAL(triggered()), SIGNAL(cellDivided()));

   QMenu * menu = addMenu(TSLC(_menu));
   menu->addAction(divideCellAction);
}


void OrganismMenu::createSelectionMenu()
{
   QActionGroup * group = new QActionGroup(this);

   m_vertexSelectionAction = new QAction(TSLC(_vertex), group);
   m_vertexSelectionAction->setCheckable(true);

   m_edgeSelectionAction = new QAction(TSLC(_edge), group);
   m_edgeSelectionAction->setCheckable(true);

   m_triangleSelectionAction = new QAction(TSLC(_triangle), group);
   m_triangleSelectionAction->setCheckable(true);

   m_tetrahedronSelectionAction = new QAction(TSLC(_tetrahedronCell), group);
   m_tetrahedronSelectionAction->setCheckable(true);

   group->addAction(m_vertexSelectionAction);
   group->addAction(m_edgeSelectionAction);
   group->addAction(m_triangleSelectionAction);
   group->addAction(m_tetrahedronSelectionAction);

   m_tetrahedronSelectionAction->setChecked(true);

   QMenu * menu = addMenu(TSLC(_selection));
   menu->addAction(m_vertexSelectionAction);
   menu->addAction(m_edgeSelectionAction);
   menu->addAction(m_triangleSelectionAction);
   menu->addAction(m_tetrahedronSelectionAction);

   connect(
      group, SIGNAL(triggered(QAction *)),
      SLOT(setSelectionMode(QAction *))
   );
}


void OrganismMenu::createViewMenu()
{
   m_showInteriorAction = new QAction(TSLC(_showInterior), this);
   connect(m_showInteriorAction, SIGNAL(triggered()), SLOT(showInterior()));

   m_showNormalsAction = new QAction(TSLC(_showNormals), this);
   connect(m_showNormalsAction, SIGNAL(triggered()), SLOT(showNormals()));

   QMenu * menu = addMenu(TSLC(_view));
   menu->addAction(m_showInteriorAction);
   menu->addAction(m_showNormalsAction);
}
