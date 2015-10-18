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


static const char * _context = "MutationParamsDialog";
static const char * _cellLimit = QT_TRANSLATE_NOOP("MutationParamsDialog", "Cell limit");
static const char * _maxDelta = QT_TRANSLATE_NOOP("MutationParamsDialog", "Max delta");
static const char * _mutationProbability = QT_TRANSLATE_NOOP("MutationParamsDialog", "Mutation probability");
static const char * _initialCoords = QT_TRANSLATE_NOOP("MutationParamsDialog", "Initial coordinates");
static const char * _chromosome = QT_TRANSLATE_NOOP("MutationParamsDialog", "Chromosome");
static const char * _maxInitialSize = QT_TRANSLATE_NOOP("MutationParamsDialog", "Max initial size");
static const char * _maxDistanceBetwCrPoints = QT_TRANSLATE_NOOP("MutationParamsDialog", "Max distance between crossing points");
static const char * _deletionProbability = QT_TRANSLATE_NOOP("MutationParamsDialog", "Deletion probability");
static const char * _instruction = QT_TRANSLATE_NOOP("MutationParamsDialog", "Genetic instruction");
static const char * _insertionProbability = QT_TRANSLATE_NOOP("MutationParamsDialog", "Insertion probability");
static const char * _maxBitFlips = QT_TRANSLATE_NOOP("MutationParamsDialog", "Max bit flips");


#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>


#include "MutationParamsDialog.hpp"
#include "ProbabilitySpinBox.hpp"
#include "translation.hpp"


/***************************************************************************
 *   MutationParamsDialog class implementation                             *
 ***************************************************************************/


MutationParamsDialog::MutationParamsDialog(
   const bio::MutationParams & params,
   QWidget * parent
) : QDialog(parent, Qt::Dialog),
   m_maxCellLimitDelta(0),
   m_cellLimitMutation(0),
   m_initialCoordMutation(0),
   m_maxCodeSize(0),
   m_maxDistanceBetweenCrossingPoints(0),
   m_chromosomeDeletion(0),
   m_instructionInsertion(0),
   m_instructionDeletion(0),
   m_instructionMutation(0),
   m_maxInstructionBitFlips(0),
   m_buttonBox(0)
{
   QVBoxLayout * mainLayout = new QVBoxLayout(this);
   const Qt::Alignment align = Qt::AlignVCenter | Qt::AlignRight;
   const int fieldWidth = 64;

   {
      QGroupBox * box = new QGroupBox(TSLC(_cellLimit), this);
      m_maxCellLimitDelta = new QSpinBox(box);
      m_maxCellLimitDelta->setRange(0, 100);
      m_maxCellLimitDelta->setFixedWidth(fieldWidth);
      m_cellLimitMutation = new ProbabilitySpinBox(box);
      m_cellLimitMutation->setFixedWidth(fieldWidth);
      QGridLayout * layout = new QGridLayout(box);
      layout->addWidget(new QLabel(TSLC(_maxDelta), box), 0, 0);
      layout->addWidget(m_maxCellLimitDelta, 0, 1, align);
      layout->addWidget(new QLabel(TSLC(_mutationProbability), box), 1, 0);
      layout->addWidget(m_cellLimitMutation, 1, 1, align);
      mainLayout->addWidget(box);
   }

   {
      QGroupBox * box = new QGroupBox(TSLC(_initialCoords), this);
      m_initialCoordMutation = new ProbabilitySpinBox(box);
      m_initialCoordMutation->setFixedWidth(fieldWidth);
      QGridLayout * layout = new QGridLayout(box);
      layout->addWidget(new QLabel(TSLC(_mutationProbability), box), 0, 0);
      layout->addWidget(m_initialCoordMutation, 0, 1, align);
      mainLayout->addWidget(box);
   }

   {
      QGroupBox * box = new QGroupBox(TSLC(_chromosome), this);
      m_maxCodeSize = new QSpinBox(box);
      m_maxCodeSize->setRange(1, 1024);
      m_maxCodeSize->setFixedWidth(fieldWidth);
      m_maxDistanceBetweenCrossingPoints = new QSpinBox(box);
      m_maxDistanceBetweenCrossingPoints->setRange(1, 1024);
      m_maxDistanceBetweenCrossingPoints->setFixedWidth(fieldWidth);
      m_chromosomeDeletion = new ProbabilitySpinBox(box);
      m_chromosomeDeletion->setFixedWidth(fieldWidth);
      QGridLayout * layout = new QGridLayout(box);
      layout->addWidget(new QLabel(TSLC(_maxInitialSize), box), 0, 0);
      layout->addWidget(m_maxCodeSize, 0, 1, align);
      layout->addWidget(new QLabel(TSLC(_maxDistanceBetwCrPoints), box), 1, 0);
      layout->addWidget(m_maxDistanceBetweenCrossingPoints, 1, 1, align);
      layout->addWidget(new QLabel(TSLC(_deletionProbability), box), 2, 0);
      layout->addWidget(m_chromosomeDeletion, 2, 1, align);
      mainLayout->addWidget(box);
   }

   {
      QGroupBox * box = new QGroupBox(TSLC(_instruction), this);
      m_instructionInsertion = new ProbabilitySpinBox(box);
      m_instructionInsertion->setFixedWidth(fieldWidth);
      m_instructionDeletion = new ProbabilitySpinBox(box);
      m_instructionDeletion->setFixedWidth(fieldWidth);
      m_instructionMutation = new ProbabilitySpinBox(box);
      m_instructionMutation->setFixedWidth(fieldWidth);
      m_maxInstructionBitFlips = new QSpinBox(box);
      m_maxInstructionBitFlips->setRange(0, 32);
      m_maxInstructionBitFlips->setFixedWidth(fieldWidth);
      QGridLayout * layout = new QGridLayout(box);
      layout->addWidget(new QLabel(TSLC(_insertionProbability), box), 0, 0);
      layout->addWidget(m_instructionInsertion, 0, 1, align);
      layout->addWidget(new QLabel(TSLC(_deletionProbability), box), 1, 0);
      layout->addWidget(m_instructionDeletion, 1, 1, align);
      layout->addWidget(new QLabel(TSLC(_mutationProbability), box), 2, 0);
      layout->addWidget(m_instructionMutation, 2, 1, align);
      layout->addWidget(new QLabel(TSLC(_maxBitFlips), box), 3, 0);
      layout->addWidget(m_maxInstructionBitFlips, 3, 1, align);
      mainLayout->addWidget(box);
   }

   m_buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
      Qt::Horizontal,
      this
   );
   mainLayout->addWidget(m_buttonBox);

   m_maxCellLimitDelta->setValue(params.maxCellLimitDelta);
   m_cellLimitMutation->setProbability(params.cellLimitMutation);
   m_initialCoordMutation->setProbability(params.initialCoordMutation);
   m_maxCodeSize->setValue(params.maxCodeSize);
   m_maxDistanceBetweenCrossingPoints->setValue(
      params.maxDistanceBetweenCrossingPoints
   );
   m_chromosomeDeletion->setProbability(params.chromosomeDeletion);
   m_instructionInsertion->setProbability(params.instructionInsertion);
   m_instructionDeletion->setProbability(params.instructionDeletion);
   m_instructionMutation->setProbability(params.instructionMutation);
   m_maxInstructionBitFlips->setValue(params.maxInstructionBitFlips);

   connect(m_buttonBox, SIGNAL(rejected()), SLOT(reject()));
   connect(m_buttonBox, SIGNAL(accepted()), SLOT(accept()));
}


MutationParamsDialog::~MutationParamsDialog()
{
}


bio::MutationParams MutationParamsDialog::mutationParams() const
{
   bio::MutationParams mp;
   mp.maxCellLimitDelta = m_maxCellLimitDelta->value();
   mp.cellLimitMutation = m_cellLimitMutation->probability();
   mp.initialCoordMutation = m_initialCoordMutation->probability();
   mp.maxCodeSize = m_maxCodeSize->value();
   mp.maxDistanceBetweenCrossingPoints =
      m_maxDistanceBetweenCrossingPoints->value();
   mp.chromosomeDeletion = m_chromosomeDeletion->probability();
   mp.instructionInsertion = m_instructionInsertion->probability();
   mp.instructionDeletion = m_instructionDeletion->probability();
   mp.instructionMutation = m_instructionMutation->probability();
   mp.maxInstructionBitFlips = m_maxInstructionBitFlips->value();
   return mp;
}
