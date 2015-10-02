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


static const char * _context = "MainWindow";
static const char * _organism = QT_TRANSLATE_NOOP("MainWindow", "Organism");
static const char * _selection = QT_TRANSLATE_NOOP("MainWindow", "Selection");
static const char * _genome = QT_TRANSLATE_NOOP("MainWindow", "Genome");
static const char * _unsavedChanges = QT_TRANSLATE_NOOP("MainWindow", "Unsaved changes");
static const char * _unsavedChangesQuestion = QT_TRANSLATE_NOOP("MainWindow", "Project \"%1\" have unsaved changes");
static const char * _projectFileType = QT_TRANSLATE_NOOP("MainWindow", "Tetrahedrosaur project (*.tzv)");
static const char * _tetrahedrosaur = QT_TRANSLATE_NOOP("MainWindow", "Tetrahedrosaur");
static const char * _aboutTetrahedrosaur = QT_TRANSLATE_NOOP("MainWindow", "About Tetrahedrosaur");
static const char * _aboutText = QT_TRANSLATE_NOOP("MainWindow", "<h3>Tetrahedrosaur 1.0.0-rc2</h3>"
   "<p>Tetrahedrosaur is an experimental software that allows user to create various "
   "3D shapes via selective breeding. It uses genetic instructions to grow virtual "
   "organism from a single tetrahedral cell. The primary goal of the program is to "
   "demonstrate how genetic algorithms work.</p>"
   "<p>This program is free software. It is published under GNU General Public License "
   "v3 or later. See the license for more details.</p>"
   "<p>Copyright (C) 2015 Andrey Timashov</p>");


#include <QtCore/QCoreApplication>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>


#include "Application.hpp"
#include "GenomeTab.hpp"
#include "GuiOrganismDesc.hpp"
#include "MainWindow.hpp"
#include "MainWindowMenuBar.hpp"
#include "NewProjectDialog.hpp"
#include "OrganismTab.hpp"
#include "Project.hpp"
#include "ProjectLoadingDialog.hpp"
#include "SelectionTab.hpp"


namespace {


QMessageBox::StandardButton _askAboutUnsavedChanges(
   QWidget * parent,
   const QString & projectName
)
{
   return QMessageBox::warning(
      parent,
      QCoreApplication::translate(_context, _unsavedChanges),
      QString(QCoreApplication::translate(_context, _unsavedChangesQuestion))
         .arg(projectName),
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
      QMessageBox::Save
   );
}


} // anonymous namespace;


MainWindow::MainWindow(QWidget * parent)
   : QMainWindow(parent),
   m_tabs(0)
{
   setMinimumSize(320, 240);

   MainWindowMenuBar * menuBar = new MainWindowMenuBar(this);
   setMenuBar(menuBar);

   m_tabs = new QTabWidget(this);
   m_tabs->setDocumentMode(true);
   m_tabs->setTabsClosable(true);
   m_tabs->setMovable(true);

   setCentralWidget(m_tabs);
   updateWindowTitle();

   connect(m_tabs, SIGNAL(currentChanged(int)), SLOT(onCurrentTabChange(int)));
   connect(m_tabs, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
   connect(menuBar, SIGNAL(newProjectRequested()), SLOT(newProject()));
   connect(menuBar, SIGNAL(openProjectRequested()), SLOT(openProject()));
   connect(menuBar, SIGNAL(closeProjectRequested()), SLOT(closeProject()));
   connect(menuBar, SIGNAL(saveProjectRequested()), SLOT(saveProject()));
   connect(menuBar, SIGNAL(saveProjectAsRequested()), SLOT(saveProjectAs()));
   connect(menuBar, SIGNAL(exitRequested()), SLOT(close()));
   connect(menuBar, SIGNAL(aboutRequested()), SLOT(about()));
}


MainWindow::~MainWindow()
{
}


QSize MainWindow::sizeHint() const
{
   return QSize(900, 600);
}


void MainWindow::addOrganismTab(boost::shared_ptr<const GuiOrganismDesc> desc)
{
   OrganismTab * organismTab = new OrganismTab(desc, m_tabs);
   m_tabs->setCurrentIndex(m_tabs->addTab(organismTab, tr(_organism)));
}


void MainWindow::addGenomeTab(boost::shared_ptr<const GuiOrganismDesc> desc)
{
   GenomeTab * genomeTab = new GenomeTab(m_tabs);
   genomeTab->setOrganismDesc(desc);
   m_tabs->setCurrentIndex(m_tabs->addTab(genomeTab, tr(_genome)));
}


void MainWindow::closeEvent(QCloseEvent * event)
{
   if (manageUnsavedChangesAndCloseProject())
   {
      QMainWindow::closeEvent(event);
   }
   else
   {
      event->ignore();
   }
}


void MainWindow::onCurrentTabChange(int index)
{
   /*if (index >= 0)
   {
      if (const MainWindowTab * tab = dynamic_cast<const MainWindowTab *>(
         m_tabs->widget(index))
      )
      {
         // TODO: Manage status bar;
      }
   }*/
}


void MainWindow::closeTab(int index)
{
   if (MainWindowTab * tab = dynamic_cast<MainWindowTab *>(
      m_tabs->widget(index))
   )
   {
      if (tab == m_selectionTab)
      {
         manageUnsavedChangesAndCloseProject();
      }
      else
      {
         m_tabs->removeTab(index);
         tab->close();
      }
   }
}


void MainWindow::newProject()
{
   if (manageUnsavedChangesAndCloseProject())
   {
      NewProjectDialog dialog(this);
      if (dialog.exec() == QDialog::Accepted)
      {
         auto project = Project::create(dialog.projectName());
         if (project)
         {
            setApplicationProject(project);
            Q_ASSERT(!m_selectionTab);
            m_selectionTab = new SelectionTab(m_tabs);
            m_tabs->addTab(m_selectionTab, tr(_selection));
         }
      }
   }
}


void MainWindow::openProject()
{
   if (manageUnsavedChangesAndCloseProject())
   {
      QFileDialog dialog(this);
      dialog.setFileMode(QFileDialog::ExistingFile);
      dialog.setNameFilter(
         QCoreApplication::translate(_context, _projectFileType)
      );

      if (dialog.exec() == QDialog::Accepted)
      {
         QStringList fileNames = dialog.selectedFiles();
         if (!fileNames.isEmpty())
         {
            auto project = Project::open(fileNames.front());
            if (project)
            {
               ProjectLoadingDialog loadingDialog(project, this);
               if (loadingDialog.exec() == QDialog::Accepted)
               {
                  setApplicationProject(project);
                  Q_ASSERT(!m_selectionTab);
                  m_selectionTab = new SelectionTab(m_tabs);
                  m_tabs->addTab(m_selectionTab, tr(_selection));
               }
            }
         }
      }
   }
}


void MainWindow::closeProject()
{
   manageUnsavedChangesAndCloseProject();
}


void MainWindow::saveProject()
{
   helpSaveProject();
}


void MainWindow::saveProjectAs()
{
   helpSaveProject(true);
}


void MainWindow::about()
{
   QMessageBox::about(
      this,
      QCoreApplication::translate(_context, _aboutTetrahedrosaur),
      QCoreApplication::translate(_context, _aboutText)
   );
}


void MainWindow::updateWindowTitle()
{
   boost::shared_ptr<Project> project = Application::project();
   QString title;
   if (project)
   {
      if (!project->isSaved())
      {
         title += "*";
      }
      title += project->projectName();

      QString fileName = project->fileName();
      if (!fileName.isEmpty())
      {
         title += QString(" (%1)").arg(fileName);
      }
      title += " - ";
   }
   setWindowTitle(title + QCoreApplication::translate(_context, _tetrahedrosaur));
}


void MainWindow::setApplicationProject(boost::shared_ptr<Project> project)
{
   if (boost::shared_ptr<Project> oldProject = Application::project())
   {
      disconnect(oldProject.get(),
         SIGNAL(projectChanged()),
         this,
         SLOT(updateWindowTitle())
      );
   }
   Application::setProject(project);
   if (project)
   {
      connect(project.get(),
         SIGNAL(projectChanged()),
         SLOT(updateWindowTitle()),
         Qt::QueuedConnection
      );
   }
   updateWindowTitle();
}


bool MainWindow::manageUnsavedChangesAndCloseProject()
{
   boost::shared_ptr<Project> project = Application::project();
   if (project && !project->isSaved())
   {
      QMessageBox::StandardButton button =
         _askAboutUnsavedChanges(this, project->projectName());
      switch (button)
      {
         case QMessageBox::Save:
         {
            if (!helpSaveProject())
            {
               return false;
            }
            break;
         }
         case QMessageBox::Discard:
            break;
         default:
            return false;
      }
   }

   // Close all tabs and set null project to application;
   const int tabsCount = m_tabs->count();
   if (tabsCount)
   {
      Q_ASSERT(m_selectionTab);
      Q_ASSERT(project);

      disconnect(
         m_tabs,
         SIGNAL(currentChanged(int)),
         this,
         SLOT(onCurrentTabChange(int))
      );
      disconnect(
         m_tabs,
         SIGNAL(tabCloseRequested(int)),
         this,
         SLOT(closeTab(int))
      );

      for (int i = tabsCount - 1; i >= 0; --i)
      {
         if (MainWindowTab * tab = dynamic_cast<MainWindowTab *>(
            m_tabs->widget(i))
         )
         {
            m_tabs->removeTab(i);
            tab->close();
         }
      }
      m_selectionTab = 0;

      connect(
         m_tabs,
         SIGNAL(currentChanged(int)),
         SLOT(onCurrentTabChange(int))
      );
      connect(m_tabs, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
   }

   setApplicationProject(boost::shared_ptr<Project>());
   return true;
}


bool MainWindow::helpSaveProject(bool saveAs)
{
   boost::shared_ptr<Project> project = Application::project();
   if (!project)
   {
      return false;
   }

   QString fileName;
   if (!saveAs)
   {
      fileName = project->fileName();
   }

   if (fileName.isEmpty())
   {
      QFileDialog dialog(this);
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      dialog.setFileMode(QFileDialog::AnyFile);
      dialog.setNameFilter(
         QCoreApplication::translate(_context, _projectFileType)
      );

      if (dialog.exec() == QDialog::Accepted)
      {
         QStringList fileNames = dialog.selectedFiles();
         if (!fileNames.isEmpty())
         {
            fileName = fileNames.front();
         }
      }

      if (fileName.isEmpty())
      {
         return false;
      }
   }

   const bool ok = project->saveAs(fileName);
   if (!ok)
   {
      // TODO: Show message box;
      return false;
   }
   return true;
}
