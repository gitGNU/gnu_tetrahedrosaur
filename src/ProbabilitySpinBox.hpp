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


#ifndef PROBABILITYSPINBOX_HPP
#define PROBABILITYSPINBOX_HPP


#include <QtWidgets/QSpinBox>


#include "algo/Probability.hpp"


/***************************************************************************
 *   ProbabilitySpinBox class declaration                                  *
 ***************************************************************************/


class ProbabilitySpinBox : public QSpinBox
{
   public:
      explicit ProbabilitySpinBox(QWidget * parent = 0);
      virtual ~ProbabilitySpinBox();

      algo::Probability probability() const;
      void setProbability(const algo::Probability & p);
};


#endif
