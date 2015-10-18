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


#include "ProbabilitySpinBox.hpp"


/***************************************************************************
 *   ProbabilitySpinBox class implementation                               *
 ***************************************************************************/


ProbabilitySpinBox::ProbabilitySpinBox(QWidget * parent)
   : QSpinBox(parent)
{
   setRange(0, 100);
   setSuffix("%");
}


ProbabilitySpinBox::~ProbabilitySpinBox()
{}


algo::Probability ProbabilitySpinBox::probability() const
{
   return algo::Probability(value() / 100.0f);
}


void ProbabilitySpinBox::setProbability(const algo::Probability & p)
{
   setValue(static_cast<int>(p.get() * 100.0f));
}
