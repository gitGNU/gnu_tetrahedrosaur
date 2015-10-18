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


static const char * _context = "Application";
static const char * _tetrahedrosaur = QT_TRANSLATE_NOOP("Application", "Tetrahedrosaur");


#include <QtWidgets/QMessageBox>


#include "Application.hpp"
#include "MainWindow.hpp"
#include "Project.hpp"
#include "SharedGLWidget.hpp"
#include "translation.hpp"


/***************************************************************************
 *   Application class implementation                                      *
 ***************************************************************************/


Application::Application(int & argc, char ** argv)
   : QApplication(argc, argv), m_initializationTimer(0)
{
   QIcon icon;
   icon.addFile(":/Tetrahedrosaur_24x24.png", QSize(24, 24));
   icon.addFile(":/Tetrahedrosaur_32x32.png", QSize(32, 32));
   icon.addFile(":/Tetrahedrosaur_64x64.png", QSize(64, 64));
   icon.addFile(":/Tetrahedrosaur_128x128.png", QSize(128, 128));
   setWindowIcon(icon);
   m_initializationTimer = startTimer(0);
}


Application::~Application()
{
   SharedGLWidget::deleteInstance();
}


Application * Application::instance()
{
   return reinterpret_cast<Application *>(qApp);
}


boost::shared_ptr<Project> Application::project()
{
   return reinterpret_cast<Application *>(qApp)->m_project;
}


void Application::setProject(boost::shared_ptr<Project> project)
{
   reinterpret_cast<Application *>(qApp)->m_project = project;
}


MainWindow * Application::mainWindow() const
{
   return m_mainWindow;
}


void Application::addOrganismTab(boost::shared_ptr<const GuiOrganismDesc> desc)
{
   if (m_mainWindow)
   {
      m_mainWindow->addOrganismTab(desc);
   }
}


void Application::addGenomeTab(boost::shared_ptr<const GuiOrganismDesc> desc)
{
   if (m_mainWindow)
   {
      m_mainWindow->addGenomeTab(desc);
   }
}


void Application::timerEvent(QTimerEvent * event)
{
   if (m_initializationTimer && event->timerId() == m_initializationTimer)
   {
      killTimer(m_initializationTimer);
      m_initializationTimer = 0;
      initialize();
   }
}


void Application::initialize()
{
   SharedGLWidget * gl = SharedGLWidget::instance();
   if (gl->hasError())
   {
      QMessageBox::critical(0, TSLC(_tetrahedrosaur), gl->errorText());
      quit();
      return;
   }

   Q_ASSERT(!m_mainWindow);
   m_mainWindow = new MainWindow();
   m_mainWindow->setAttribute(Qt::WA_DeleteOnClose);
   m_mainWindow->show();

   connect(this, SIGNAL(lastWindowClosed()), SLOT(quit()));
}
