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


#ifndef MUTATIONPARAMSDIALOG_HPP
#define MUTATIONPARAMSDIALOG_HPP


#include <QtWidgets/QDialog>
QT_FORWARD_DECLARE_CLASS(QDialogButtonBox)
QT_FORWARD_DECLARE_CLASS(QSpinBox)


class ProbabilitySpinBox;


#include "bio/MutationParams.hpp"


/***************************************************************************
 *   MutationParamsDialog class declaration                                *
 ***************************************************************************/


class MutationParamsDialog : public QDialog
{
   public:
      explicit MutationParamsDialog(
         const bio::MutationParams & params,
         QWidget * parent = 0
      );
      virtual ~MutationParamsDialog();

      bio::MutationParams mutationParams() const;

   private:
      QSpinBox * m_maxCellLimitDelta;
      ProbabilitySpinBox * m_cellLimitMutation;
      ProbabilitySpinBox * m_initialCoordMutation;
      QSpinBox * m_maxCodeSize;
      QSpinBox * m_maxDistanceBetweenCrossingPoints;
      ProbabilitySpinBox * m_chromosomeDeletion;
      ProbabilitySpinBox * m_instructionInsertion;
      ProbabilitySpinBox * m_instructionDeletion;
      ProbabilitySpinBox * m_instructionMutation;
      QSpinBox * m_maxInstructionBitFlips;
      QDialogButtonBox * m_buttonBox;
};


#endif
