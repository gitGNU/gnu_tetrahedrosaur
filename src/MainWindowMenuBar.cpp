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


static const char * _context = "MainWindowMenuBar";
static const char * _file = QT_TRANSLATE_NOOP("MainWindowMenuBar", "File");
static const char * _newProject = QT_TRANSLATE_NOOP("MainWindowMenuBar", "New project");
static const char * _openProject = QT_TRANSLATE_NOOP("MainWindowMenuBar", "Open project...");
static const char * _closeProject = QT_TRANSLATE_NOOP("MainWindowMenuBar", "Close project");
static const char * _saveProject = QT_TRANSLATE_NOOP("MainWindowMenuBar", "Save project");
static const char * _saveProjectAs = QT_TRANSLATE_NOOP("MainWindowMenuBar", "Save project as...");
static const char * _exit = QT_TRANSLATE_NOOP("MainWindowMenuBar", "Exit");
static const char * _help = QT_TRANSLATE_NOOP("MainWindowMenuBar", "Help");
static const char * _about = QT_TRANSLATE_NOOP("MainWindowMenuBar", "About");
static const char * _aboutQt = QT_TRANSLATE_NOOP("MainWindowMenuBar", "About Qt");


#include <QtCore/QCoreApplication>


#include "MainWindowMenuBar.hpp"
#include "translation.hpp"


MainWindowMenuBar::MainWindowMenuBar(QWidget * parent)
   : QMenuBar(parent),
   m_saveProjectAction(0),
   m_saveProjectAsAction(0)
{
   createFileMenu();
   addSeparator();
   createHelpMenu();
}


void MainWindowMenuBar::createFileMenu()
{
   QAction * newProjectAction = new QAction(TSLC(_newProject), this);
   connect(
      newProjectAction,
      SIGNAL(triggered()),
      SIGNAL(newProjectRequested())
   );

   QAction * openProjectAction = new QAction(TSLC(_openProject), this);
   connect(
      openProjectAction,
      SIGNAL(triggered()),
      SIGNAL(openProjectRequested())
   );

   QAction * closeProjectAction = new QAction(TSLC(_closeProject), this);
   connect(
      closeProjectAction,
      SIGNAL(triggered()),
      SIGNAL(closeProjectRequested())
   );

   m_saveProjectAction = new QAction(TSLC(_saveProject), this);
   connect(
      m_saveProjectAction,
      SIGNAL(triggered()),
      SIGNAL(saveProjectRequested())
   );

   m_saveProjectAsAction = new QAction(TSLC(_saveProjectAs), this);
   connect(
      m_saveProjectAsAction,
      SIGNAL(triggered()),
      SIGNAL(saveProjectAsRequested())
   );

   QAction * exitAction = new QAction(TSLC(_exit), this);
   connect(exitAction, SIGNAL(triggered()), SIGNAL(exitRequested()));

   QMenu * menu = addMenu(TSLC(_file));
   menu->addAction(newProjectAction);
   menu->addAction(openProjectAction);
   menu->addAction(closeProjectAction);
   menu->addSeparator();
   menu->addAction(m_saveProjectAction);
   menu->addAction(m_saveProjectAsAction);
   menu->addSeparator();
   menu->addAction(exitAction);
}


void MainWindowMenuBar::createHelpMenu()
{
   QAction * aboutAction = new QAction(TSLC(_about), this);
   connect(aboutAction, SIGNAL(triggered()), SIGNAL(aboutRequested()));

   QAction * aboutQtAction = new QAction(TSLC(_aboutQt), this);
   connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

   QMenu * menu = addMenu(TSLC(_help));
   menu->addAction(aboutAction);
   menu->addAction(aboutQtAction);
}
