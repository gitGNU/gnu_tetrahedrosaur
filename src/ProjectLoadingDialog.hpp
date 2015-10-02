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


#ifndef PROJECTLOADINGDIALOG_HPP
#define PROJECTLOADINGDIALOG_HPP


#include <boost/shared_ptr.hpp>


#include <QtWidgets/QDialog>
#include <QtGui/QPixmap>
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QProgressBar)


class DevelopmentEngine;
class Project;


namespace mesh {
class Figure;
}


/***************************************************************************
 *   ProjectLoadingDialog class declaration                                *
 ***************************************************************************/


class ProjectLoadingDialog : public QDialog
{
   Q_OBJECT

   public:
      explicit ProjectLoadingDialog(
         boost::shared_ptr<Project> project,
         QWidget * parent = 0
      );
      virtual ~ProjectLoadingDialog();

      virtual void reject();

   protected:
      virtual void showEvent(QShowEvent * event);

   private slots:
      void setDescProgress(size_t descIndex, int progress);
      void setDescFigure(
         size_t descIndex,
         boost::shared_ptr<mesh::Figure> figure,
         QPixmap portrait
      );

   private:
      QProgressBar * m_progressBar;
      QDialogButtonBox * m_buttonBox;
      DevelopmentEngine * m_engine;
      boost::shared_ptr<Project> m_project;
};


#endif
