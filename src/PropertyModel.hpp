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


#ifndef PROPERTYMODEL_HPP
#define PROPERTYMODEL_HPP


#include <QtCore/QAbstractItemModel>


/***************************************************************************
 *   PropertyModel class declaration                                       *
 ***************************************************************************/


class PropertyModel : public QAbstractItemModel
{
   public:
      enum COLUMN_ID
      {
         CID_PROPERTY = 0,
         CID_VALUE,
         ColumnCount
      };

      explicit PropertyModel(QObject * parent = 0);

      virtual bool rootIsDecorated() const;

      virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
      virtual QVariant headerData(
         int section,
         Qt::Orientation orientation,
         int role = Qt::DisplayRole
      ) const;
};


#endif
