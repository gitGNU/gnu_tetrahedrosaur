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


#ifndef NEWPROJECTDIALOG_HPP
#define NEWPROJECTDIALOG_HPP


#include <QtWidgets/QDialog>
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QLineEdit)


/***************************************************************************
 *   NewProjectDialog class declaration                                    *
 ***************************************************************************/


class NewProjectDialog : public QDialog
{
   Q_OBJECT

   public:
      explicit NewProjectDialog(QWidget * parent = 0);
      virtual ~NewProjectDialog();

      QString projectName() const;

   private slots:
      void updateOkButton();
      void verifyAndAccept();

   private:
      QLineEdit * m_projectName;
      QDialogButtonBox * m_buttonBox;
};


#endif
