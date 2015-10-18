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


#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <boost/shared_ptr.hpp>


#include <QtCore/QPointer>
#include <QtWidgets/QApplication>


struct GuiOrganismDesc;
class MainWindow;
class Project;


/***************************************************************************
 *   Application class declaration                                         *
 ***************************************************************************/


class Application : public QApplication
{
   public:
      explicit Application(int & argc, char ** argv);
      virtual ~Application();

      static Application * instance();

      static boost::shared_ptr<Project> project();
      static void setProject(boost::shared_ptr<Project> project);

      MainWindow * mainWindow() const;

      void addOrganismTab(boost::shared_ptr<const GuiOrganismDesc> desc);
      void addGenomeTab(boost::shared_ptr<const GuiOrganismDesc> desc);

   protected:
      virtual void timerEvent(QTimerEvent * event);

   private:
      void initialize();

      int m_initializationTimer;
      QPointer<MainWindow> m_mainWindow;
      boost::shared_ptr<Project> m_project;
};


#endif
