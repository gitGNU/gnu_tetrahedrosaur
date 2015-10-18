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


static const char * _context = "ProjectLoadingDialog";
static const char * _loadingProject = QT_TRANSLATE_NOOP("ProjectLoadingDialog", "Loading project...");


#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>


#include "DevelopmentEngine.hpp"
#include "Project.hpp"
#include "ProjectLoadingDialog.hpp"
#include "translation.hpp"


/***************************************************************************
 *   ProjectLoadingDialog class implementation                             *
 ***************************************************************************/


ProjectLoadingDialog::ProjectLoadingDialog(
   boost::shared_ptr<Project> project,
   QWidget * parent
) : QDialog(parent, Qt::Dialog),
   m_progressBar(0),
   m_buttonBox(0),
   m_engine(0),
   m_project(project)
{
   setWindowTitle(TSLC(_loadingProject));

   m_progressBar = new QProgressBar(this);
   m_progressBar->setRange(0, 100);

   m_buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Abort,
      Qt::Horizontal,
      this
   );

   QVBoxLayout * layout = new QVBoxLayout(this);
   layout->addWidget(m_progressBar);
   layout->addWidget(m_buttonBox);

   connect(m_buttonBox, SIGNAL(rejected()), SLOT(reject()));
}


ProjectLoadingDialog::~ProjectLoadingDialog()
{
}


void ProjectLoadingDialog::reject()
{
   if (m_engine)
   {
      m_engine->abort();
   }
   QDialog::reject();
}


void ProjectLoadingDialog::showEvent(QShowEvent * event)
{
   QDialog::showEvent(event);
   if (!m_engine)
   {
      m_engine = new DevelopmentEngine(this);
      connect(
         m_engine,
         SIGNAL(descProgressChanged(size_t, int)),
         SLOT(setDescProgress(size_t, int))
      );
      connect(
         m_engine,
         SIGNAL(descFinished(size_t, boost::shared_ptr<mesh::Figure>, QPixmap)),
         SLOT(setDescFigure(size_t, boost::shared_ptr<mesh::Figure>, QPixmap))
      );
      connect(m_engine, SIGNAL(allFinished()), SLOT(accept()));
      m_engine->start(m_project->population());
   }
}


void ProjectLoadingDialog::setDescProgress(size_t descIndex, int progress)
{
   const size_t populationSize = m_project->population().size();
   if (populationSize)
   {
      float percent = descIndex * 100.0f;
      percent += progress;
      percent /= populationSize;
      m_progressBar->setValue(static_cast<int>(percent));
   }
}


void ProjectLoadingDialog::setDescFigure(
   size_t descIndex,
   boost::shared_ptr<mesh::Figure> figure,
   QPixmap portrait
)
{
   m_project->setDescFigure(descIndex, figure, portrait);
}
