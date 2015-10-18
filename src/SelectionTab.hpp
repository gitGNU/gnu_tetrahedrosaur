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


#ifndef SELECTIONTAB_HPP
#define SELECTIONTAB_HPP


#include <boost/shared_ptr.hpp>


#include <QtCore/QtGlobal>
QT_FORWARD_DECLARE_CLASS(QModelIndex)
QT_FORWARD_DECLARE_CLASS(QToolButton)


#include "MainWindowTab.hpp"
class FigureViewport;
class MatingWidget;
class OffspringModel;
class PopulationView;
class Project;


#include "bio/MutationParams.hpp"


class SelectionTab : public MainWindowTab
{
   Q_OBJECT

   public:
      explicit SelectionTab(QWidget * parent = 0);
      virtual ~SelectionTab();

   private slots:
      void mateLow();
      void mateMedium();
      void mateHigh();
      void mateCustom();
      void setCustomMutationParams();
      void clearOffsprings();
      void updateState();
      void setMate(const QModelIndex & index);
      void moveToPopulation(const QModelIndex & index);
      void previewFigure();

   private:
      QToolButton * createToolButton(const QIcon & icon, const char * tip);
      void mate(const bio::MutationParams & mutationParams);

      boost::shared_ptr<Project> m_project;
      PopulationView * m_projectView;
      OffspringModel * m_offspringModel;
      PopulationView * m_offspringView;
      MatingWidget * m_matingWidget;
      QToolButton * m_mateLowButton;
      QToolButton * m_mateMediumButton;
      QToolButton * m_mateHighButton;
      QToolButton * m_mateCustomButton;
      QToolButton * m_clearOffspringsButton;
      FigureViewport * m_viewport;
      bool m_previewFigureCausedByClearSelection;
      bio::MutationParams m_customMutationParams;
};


#endif
