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


#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


#include <boost/shared_ptr.hpp>


#include <QtCore/QPointer>
#include <QtWidgets/QMainWindow>
QT_FORWARD_DECLARE_CLASS(QTabWidget)


struct GuiOrganismDesc;
class Project;
class SelectionTab;


class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
      explicit MainWindow(QWidget * parent = 0);
      virtual ~MainWindow();

      virtual QSize sizeHint() const;

      void addOrganismTab(boost::shared_ptr<const GuiOrganismDesc> desc);
      void addGenomeTab(boost::shared_ptr<const GuiOrganismDesc> desc);

   protected:
      virtual void closeEvent(QCloseEvent * event);

   private slots:
      void onCurrentTabChange(int index);
      void closeTab(int index);
      void newProject();
      void openProject();
      void closeProject();
      void saveProject();
      void saveProjectAs();
      void about();
      void updateWindowTitle();

   private:
      void setApplicationProject(boost::shared_ptr<Project> project);
      bool manageUnsavedChangesAndCloseProject();
      bool helpSaveProject(bool saveAs = false);

   private:
      QTabWidget * m_tabs;
      QPointer<SelectionTab> m_selectionTab;
};


#endif
