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


#include "TriangleModel.hpp"


#include "bio/Organism.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/Triangle.hpp"


/*
text                   internalPionter        parent                 row
========================================================================
id                     0                      -                       0
a                      0                      -                       0
b                      0                      -                       0
c                      0                      -                       0
*/


#define _SWITCH_COLUMN(property, value) \
   switch (index.column()) \
   { \
      case CID_PROPERTY: \
         return property; \
      case CID_VALUE: \
         return value; \
   }


enum TRIANGLE_PROPERTY
{
   TP_ID = 0,
   TP_A,
   TP_B,
   TP_C,
   TrianglePropertyCount
};


/***************************************************************************
 *   TriangleModel class implementation                                    *
 ***************************************************************************/


TriangleModel::TriangleModel(QObject * parent)
   : PropertyModel(parent)
{
}


void TriangleModel::setOrganism(boost::shared_ptr<bio::Organism> organism)
{
   beginResetModel();
   m_organism = organism;
   m_triangle = (m_organism ?
      organism->mesh().selectedTriangle() :
      boost::optional<dt::TriangleId>()
   );
   endResetModel();
}


QVariant TriangleModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid() && m_organism && m_triangle)
   {
      if (role == Qt::DisplayRole || role == Qt::EditRole)
      {
         const mesh::Triangle & t =
            m_organism->mesh().triangles()[m_triangle->get()];
         switch (index.row())
         {
            case TP_ID:
               _SWITCH_COLUMN(
                  QString("id"),
                  static_cast<qulonglong>(m_triangle->get()));
               break;
            case TP_A:
               _SWITCH_COLUMN(QString("a"), static_cast<qulonglong>(t.a));
               break;
            case TP_B:
               _SWITCH_COLUMN(QString("b"), static_cast<qulonglong>(t.b));
               break;
            case TP_C:
               _SWITCH_COLUMN(QString("c"), static_cast<qulonglong>(t.c));
               break;
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags TriangleModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;
   if (index.isValid() && m_organism && m_triangle)
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
   }
   return flags;
}


QModelIndex TriangleModel::index(
   int row,
   int column,
   const QModelIndex & parent
) const
{
   if (m_organism && m_triangle && row >= 0 && column >= 0 &&
      column < ColumnCount)
   {
      if (!parent.isValid() && row < TrianglePropertyCount)
      {
         return createIndex(row, column);
      }
   }
   return QModelIndex();
}


QModelIndex TriangleModel::parent(const QModelIndex &) const
{
   return QModelIndex();
}


int TriangleModel::rowCount(const QModelIndex & parent) const
{
   if (m_organism && m_triangle && !parent.isValid())
   {
      return TrianglePropertyCount;
   }
   return 0;
}
