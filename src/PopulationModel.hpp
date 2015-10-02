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


#ifndef POPULATIONMODEL_HPP
#define POPULATIONMODEL_HPP


#include <boost/shared_ptr.hpp>


#include <QtCore/QAbstractTableModel>


struct GuiOrganismDesc;


/***************************************************************************
 *   PopulationModel class declaration                                     *
 ***************************************************************************/


class PopulationModel : public QAbstractTableModel
{
   Q_OBJECT

   public:
      explicit PopulationModel(
         Qt::Orientation orientation,
         QObject * parent = 0
      );
      virtual ~PopulationModel();

      inline Qt::Orientation orientation() const {return m_orientation;}
      void setOrientation(Qt::Orientation orientation);

      virtual boost::shared_ptr<const GuiOrganismDesc> organismDesc(
         const QModelIndex & index
      ) const = 0;

   signals:
      void orientationChanged();

   private:
      Qt::Orientation m_orientation;
};


#endif
