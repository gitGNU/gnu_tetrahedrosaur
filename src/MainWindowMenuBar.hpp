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


#ifndef MAINWINDOWMENUBAR_HPP
#define MAINWINDOWMENUBAR_HPP


#include <QtWidgets/QMenuBar>
QT_FORWARD_DECLARE_CLASS(QAction)


class MainWindowMenuBar : public QMenuBar
{
   Q_OBJECT

   public:
      explicit MainWindowMenuBar(QWidget * parent = 0);

   signals:
      void newProjectRequested();
      void openProjectRequested();
      void closeProjectRequested();
      void saveProjectRequested();
      void saveProjectAsRequested();
      void exitRequested();
      void aboutRequested();

   private:
      void createFileMenu();
      void createHelpMenu();

      QAction * m_saveProjectAction;
      QAction * m_saveProjectAsAction;
};


#endif
