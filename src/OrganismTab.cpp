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


static const char * _context = "OrganismTab";
static const char * _start = QT_TRANSLATE_NOOP("OrganismTab", "Start");
static const char * _pause = QT_TRANSLATE_NOOP("OrganismTab", "Pause");
static const char * _makeBud = QT_TRANSLATE_NOOP("OrganismTab", "Make bud");


#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>


#include "CellModel.hpp"
#include "EdgeModel.hpp"
#include "GuiOrganismDesc.hpp"
#include "OrganismMenu.hpp"
#include "OrganismTab.hpp"
#include "OrganismViewport.hpp"
#include "PropertyView.hpp"
#include "translation.hpp"
#include "TriangleModel.hpp"
#include "VertexModel.hpp"


#include "bio/Organism.hpp"
#include "mesh/Mesh.hpp"


OrganismTab::OrganismTab(
   boost::shared_ptr<const GuiOrganismDesc> desc,
   QWidget * parent
) : MainWindowTab(parent),
   m_cellModel(0),
   m_edgeModel(0),
   m_triangleModel(0),
   m_vertexModel(0),
   m_propertyView(0),
   m_viewport(0),
   m_playPauseButton(0),
   m_budButton(0)
{
   m_cellModel = new CellModel(this);
   m_edgeModel = new EdgeModel(this);
   m_triangleModel = new TriangleModel(this);
   m_vertexModel = new VertexModel(this);

   m_propertyView = new PropertyView(this);

   m_viewport = new OrganismViewport(desc, this);

   QHBoxLayout * bottomToolBarLayout = new QHBoxLayout();

   { // Create start button;
      m_playPauseButton = new QToolButton(this);
      bottomToolBarLayout->addWidget(m_playPauseButton);
   }

   { // Create bud button;
      m_budButton = new QToolButton(this);
      m_budButton->setIcon(
         QApplication::style()->standardIcon(QStyle::SP_MediaSeekForward)
      );
      m_budButton->setText(TSLC(_makeBud));
      bottomToolBarLayout->addWidget(m_budButton);
   }

   bottomToolBarLayout->addStretch(1);

   QGridLayout * mainLayout = new QGridLayout(this);
   mainLayout->addWidget(m_propertyView, 0, 0);
   mainLayout->addWidget(m_viewport, 0, 1);
   mainLayout->addLayout(bottomToolBarLayout, 1, 0, 1, 2);
   mainLayout->setColumnStretch(1, 1);

   connect(
      m_viewport,
      SIGNAL(selectionChanged()),
      SLOT(updateProperties()),
      Qt::DirectConnection
   );
   connect(
      m_cellModel,
      SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
      m_viewport,
      SLOT(updateGL())
   );
   connect(
      m_edgeModel,
      SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
      m_viewport,
      SLOT(updateGL())
   );
   connect(
      m_viewport,
      SIGNAL(edgeResized(float)),
      m_edgeModel,
      SLOT(resizeEdge(float))
   );
   connect(
      m_vertexModel,
      SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
      m_viewport,
      SLOT(updateGL())
   );
   connect(m_playPauseButton, SIGNAL(clicked()), this, SLOT(playOrPause()));
   connect(m_budButton, SIGNAL(clicked()), SLOT(makeBud()));

   updatePlayPauseButton();
}


OrganismTab::~OrganismTab()
{
}


/*QMenuBar * OrganismTab::createMenuBar(QWidget * parent) const
{
   OrganismMenu * menu = new OrganismMenu(parent);
   connect(menu, SIGNAL(cellDivided()), SLOT(makeBud()));
   connect(
      menu, SIGNAL(selectionModeChanged(dt::SelectionMode)),
      SLOT(setSelectionMode(dt::SelectionMode))
   );
   connect(menu, SIGNAL(interiorShown(bool)), SLOT(showInterior(bool)));
   connect(menu, SIGNAL(normalsShown(bool)), SLOT(showNormals(bool)));
   return menu;
}*/


void OrganismTab::setSelectionMode(dt::SelectionMode mode)
{
   m_viewport->setSelectionMode(mode);
}


void OrganismTab::showInterior(bool shown)
{
   m_viewport->showInterior(shown);
}


void OrganismTab::showNormals(bool shown)
{
   m_viewport->showNormals(shown);
}


void OrganismTab::playOrPause()
{
   m_viewport->pause(!m_viewport->isPaused());
   updatePlayPauseButton();
}


void OrganismTab::makeBud()
{
   if (boost::shared_ptr<bio::Organism> organism = m_viewport->organism())
   {
      if (const auto selTrId = organism->mesh().selectedTriangle())
      {
         const auto & selTr = organism->mesh().triangles()[selTrId->get()];
         const bio::TetrahedronsMap & map = organism->tetrahedronsMap();
         bio::TetrahedronsMap::const_iterator it = map.begin();
         bio::TetrahedronsMap::const_iterator ite = map.end();
         for (; it != ite; ++it)
         {
            if (const auto face = it->first.faceAny(selTr.a, selTr.b, selTr.c))
            {
               organism->makeCellBud(it->first, *face);
               m_viewport->updateGL();
               break;
            }
         }
      }
   }
}


void OrganismTab::updateProperties()
{
   boost::shared_ptr<bio::Organism> organism(m_viewport->organism());
   if (organism->mesh().selectedVertex())
   {
      m_vertexModel->setOrganism(organism);
      m_propertyView->setModel(m_vertexModel);
   }
   else if (organism->mesh().selectedEdge())
   {
      m_edgeModel->setOrganism(organism);
      m_propertyView->setModel(m_edgeModel);
   }
   else if (organism->mesh().selectedTriangle())
   {
      m_triangleModel->setOrganism(organism);
      m_propertyView->setModel(m_triangleModel);
   }
   else if (organism->selectedCell())
   {
      m_cellModel->setOrganism(organism);
      m_propertyView->setModel(m_cellModel);
   }
   else
   {
      m_cellModel->setOrganism(boost::shared_ptr<bio::Organism>());
      m_edgeModel->setOrganism(boost::shared_ptr<bio::Organism>());
      m_triangleModel->setOrganism(boost::shared_ptr<bio::Organism>());
      m_vertexModel->setOrganism(boost::shared_ptr<bio::Organism>());
   }
}


void OrganismTab::updatePlayPauseButton()
{
   if (m_viewport->isPaused())
   {
      m_playPauseButton->setIcon(
         QApplication::style()->standardIcon(QStyle::SP_MediaPlay)
      );
      m_playPauseButton->setText(TSLC(_start));
   }
   else
   {
      m_playPauseButton->setIcon(
         QApplication::style()->standardIcon(QStyle::SP_MediaPause)
      );
      m_playPauseButton->setText(TSLC(_pause));
   }
}
