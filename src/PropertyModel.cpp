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


static const char * _context = "PropertyModel";
static const char * _property = QT_TRANSLATE_NOOP("PropertyModel", "Property");
static const char * _value = QT_TRANSLATE_NOOP("PropertyModel", "Value");


#include <QtCore/QCoreApplication>


#include "PropertyModel.hpp"


/***************************************************************************
 *   PropertyModel class implementation                                    *
 ***************************************************************************/


PropertyModel::PropertyModel(QObject * parent)
   : QAbstractItemModel(parent)
{
}


bool PropertyModel::rootIsDecorated() const
{
   return false;
}


int PropertyModel::columnCount(const QModelIndex &) const
{
   return ColumnCount;
}


QVariant PropertyModel::headerData(
   int section,
   Qt::Orientation orientation,
   int role
) const
{
   if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
   {
      switch (section)
      {
         case CID_PROPERTY:
            return QCoreApplication::translate(_context, _property);
         case CID_VALUE:
            return QCoreApplication::translate(_context, _value);
      }
   }
   return QVariant();
}
