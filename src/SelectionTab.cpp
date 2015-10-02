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


static const char * _context = "SelectionTab";
static const char * _mate = QT_TRANSLATE_NOOP("SelectionTab", "Mate");
static const char * _clearOffspringsButton = QT_TRANSLATE_NOOP("SelectionTab", "Clear offsprings");


#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>

#include "Application.hpp"
#include "FigureViewport.hpp"
#include "GuiOrganismDesc.hpp"
#include "MatingWidget.hpp"
#include "OffspringModel.hpp"
#include "PopulationView.hpp"
#include "Project.hpp"
#include "SelectionTab.hpp"


SelectionTab::SelectionTab(QWidget * parent)
   : MainWindowTab(parent),
   m_projectView(0),
   m_offspringModel(0),
   m_offspringView(0),
   m_matingWidget(0),
   m_mateButton(0),
   m_clearOffspringsButton(0),
   m_viewport(0),
   m_previewFigureCausedByClearSelection(false)
{
   m_project = Application::project();
   m_offspringModel = new OffspringModel(this);
   m_offspringModel->setOrientation(Qt::Horizontal);

   QHBoxLayout * toolBarLayout = new QHBoxLayout();

   { // Create start button;
      m_mateButton = new QToolButton(this);
      m_mateButton->setIcon(
         QApplication::style()->standardIcon(QStyle::SP_MediaPlay)
      );
      m_mateButton->setText(QCoreApplication::translate(_context, _mate));
      toolBarLayout->addWidget(m_mateButton);
   }

   { // Create clear offsprings button;
      m_clearOffspringsButton = new QToolButton(this);
      m_clearOffspringsButton->setIcon(
         QApplication::style()->standardIcon(QStyle::SP_TrashIcon)
      );
      m_clearOffspringsButton->setText(
         QCoreApplication::translate(_context, _clearOffspringsButton)
      );
      toolBarLayout->addWidget(m_clearOffspringsButton);
   }

   toolBarLayout->addStretch(1);

   m_projectView = new PopulationView(this);
   m_projectView->setModel(m_project.get());

   m_matingWidget = new MatingWidget(this);

   m_offspringView = new PopulationView(this);
   m_offspringView->setModel(m_offspringModel);

   m_viewport = new FigureViewport(this);

   QGridLayout * mainLayout = new QGridLayout(this);
   mainLayout->addLayout(toolBarLayout, 0, 0, 1, 2);
   mainLayout->addWidget(m_projectView, 1, 0);
   mainLayout->addWidget(m_viewport, 1, 1);
   mainLayout->addWidget(m_matingWidget, 2, 0, Qt::AlignCenter);
   mainLayout->addWidget(m_offspringView, 2, 1);
   mainLayout->setRowStretch(1, 1);
   mainLayout->setColumnStretch(1, 1);

   connect(
      m_offspringModel, SIGNAL(indexStarted(const QModelIndex &)),
      m_offspringView, SLOT(scrollToIndex(const QModelIndex &))
   );
   connect(m_offspringModel, SIGNAL(finished()), SLOT(updateState()));
   connect(m_mateButton, SIGNAL(clicked()), SLOT(mate()));
   connect(m_clearOffspringsButton, SIGNAL(clicked()), SLOT(clearOffsprings()));
   connect(
      m_projectView,
      SIGNAL(doubleClicked(const QModelIndex &)),
      SLOT(setMate(const QModelIndex &))
   );
   connect(m_projectView, SIGNAL(selectionChanged()), SLOT(previewFigure()));
   connect(m_offspringView, SIGNAL(selectionChanged()), SLOT(previewFigure()));
   connect(
      m_offspringView,
      SIGNAL(doubleClicked(const QModelIndex &)),
      SLOT(moveToPopulation(const QModelIndex &))
   );

   updateState();
}


SelectionTab::~SelectionTab()
{
}


void SelectionTab::mate()
{
   if (m_offspringModel->isReady())
   {
      const auto offsprings = m_matingWidget->mate(1);
      if (!offsprings.empty())
      {
         m_offspringModel->append(offsprings);
         updateState();
      }
   }
}


void SelectionTab::clearOffsprings()
{
   if (m_offspringModel->isReady())
   {
      m_offspringModel->clear();
   }
}


void SelectionTab::updateState()
{
   const bool isReady = m_offspringModel->isReady();
   m_mateButton->setEnabled(isReady);
}


void SelectionTab::setMate(const QModelIndex & index)
{
   if (auto desc = m_project->organismDesc(index))
   {
      m_matingWidget->swap();
      m_matingWidget->setLeftParent(desc);
   }
}


void SelectionTab::moveToPopulation(const QModelIndex & index)
{
   if (auto desc = m_offspringModel->takeOrganismDesc(index))
   {
      m_project->append({desc});
      m_projectView->scrollToBottom();
   }
}


void SelectionTab::previewFigure()
{
   if (m_previewFigureCausedByClearSelection)
   {
      m_previewFigureCausedByClearSelection = false;
   }
   else
   {
      const QObject * view = sender();
      if (view == m_projectView)
      {
         m_viewport->setFigure(m_projectView->selectedFigure());
         if (m_offspringView->isSomethingSelected())
         {
            m_previewFigureCausedByClearSelection = true;
            m_offspringView->clearSelection();
         }
      }
      else if (view == m_offspringView)
      {
         m_viewport->setFigure(m_offspringView->selectedFigure());
         if (m_projectView->isSomethingSelected())
         {
            m_previewFigureCausedByClearSelection = true;
            m_projectView->clearSelection();
         }
      }
   }
}
