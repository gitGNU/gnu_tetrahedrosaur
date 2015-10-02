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


static const char * _context = "NewProjectDialog";
static const char * _newProject = QT_TRANSLATE_NOOP("NewProjectDialog", "New project");
static const char * _projectName = QT_TRANSLATE_NOOP("NewProjectDialog", "Project name");
static const char * _untitled = QT_TRANSLATE_NOOP("NewProjectDialog", "untitled");


#include <QtCore/QCoreApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>


#include "NewProjectDialog.hpp"


/***************************************************************************
 *   NewProjectDialog class implementation                                 *
 ***************************************************************************/


NewProjectDialog::NewProjectDialog(QWidget * parent)
   : QDialog(parent, Qt::Dialog), m_projectName(0), m_buttonBox(0)
{
   setWindowTitle(QCoreApplication::translate(_context, _newProject));

   m_projectName = new QLineEdit(this);
   m_projectName->setText(QCoreApplication::translate(_context, _untitled));

   m_buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
      Qt::Horizontal,
      this
   );

   QFormLayout * formLayout = new QFormLayout();
   formLayout->addRow(
      QCoreApplication::translate(_context, _projectName),
      m_projectName
   );

   QVBoxLayout * layout = new QVBoxLayout(this);
   layout->addLayout(formLayout);
   layout->addWidget(m_buttonBox);

   connect(
      m_projectName,
      SIGNAL(textChanged(const QString &)),
      SLOT(updateOkButton())
   );
   connect(m_buttonBox, SIGNAL(rejected()), SLOT(reject()));
   connect(m_buttonBox, SIGNAL(accepted()), SLOT(verifyAndAccept()));

   updateOkButton();
}


NewProjectDialog::~NewProjectDialog()
{
}


QString NewProjectDialog::projectName() const
{
   return m_projectName->text();
}


void NewProjectDialog::updateOkButton()
{
   QPushButton * okButton = m_buttonBox->button(QDialogButtonBox::Ok);
   Q_ASSERT(okButton);
   okButton->setEnabled(!m_projectName->text().isEmpty());
}


void NewProjectDialog::verifyAndAccept()
{
   if (!m_projectName->text().isEmpty())
   {
      accept();
   }
}
