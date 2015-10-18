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


static const char * _context = "EdgeModel";
static const char * _equilibriumLength = QT_TRANSLATE_NOOP("EdgeModel", "equilibrium length");


#include "EdgeModel.hpp"
#include "translation.hpp"


#include "bio/Organism.hpp"
#include "mesh/Edge.hpp"
#include "mesh/Mesh.hpp"


/*
text                   internalPionter        parent                 row
========================================================================
id                     0                      -                       0
equilibrium length     0                      -                       0
*/


#define _SWITCH_COLUMN(property, value) \
   switch (index.column()) \
   { \
      case CID_PROPERTY: \
         return property; \
      case CID_VALUE: \
         return value; \
   }


enum EDGE_PROPERTY
{
   EP_ID = 0,
   EP_EQUILIBRIUM_LENGTH,
   EdgePropertyCount
};


/***************************************************************************
 *   EdgeModel class implementation                                        *
 ***************************************************************************/


EdgeModel::EdgeModel(QObject * parent)
   : PropertyModel(parent)
{
}


void EdgeModel::setOrganism(boost::shared_ptr<bio::Organism> organism)
{
   beginResetModel();
   m_organism = organism;
   m_edge = (m_organism ?
      organism->mesh().selectedEdge() :
      boost::optional<size_t>()
   );
   endResetModel();
}


QVariant EdgeModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid() && m_organism && m_edge)
   {
      if (role == Qt::DisplayRole || role == Qt::EditRole)
      {
         const mesh::Edge & edge = m_organism->mesh().edges()[*m_edge];
         switch (index.row())
         {
            case EP_ID:
               _SWITCH_COLUMN(QString("id"), static_cast<qulonglong>(*m_edge));
               break;
            case EP_EQUILIBRIUM_LENGTH:
               _SWITCH_COLUMN(TSLC(_equilibriumLength), edge.equilibriumLength);
               break;
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags EdgeModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;
   if (index.isValid() && m_organism && m_edge)
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
      if (index.column() == CID_VALUE)
      {
         switch (index.row())
         {
            case EP_EQUILIBRIUM_LENGTH:
               flags |= Qt::ItemIsEditable;
               break;
         }
      }
   }
   return flags;
}


QModelIndex EdgeModel::index(
   int row,
   int column,
   const QModelIndex & parent
) const
{
   if (m_organism && m_edge && row >= 0 && column >= 0 &&
      column < ColumnCount)
   {
      if (!parent.isValid() && row < EdgePropertyCount)
      {
         return createIndex(row, column);
      }
   }
   return QModelIndex();
}


QModelIndex EdgeModel::parent(const QModelIndex &) const
{
   return QModelIndex();
}


int EdgeModel::rowCount(const QModelIndex & parent) const
{
   if (m_organism && m_edge && !parent.isValid())
   {
      return EdgePropertyCount;
   }
   return 0;
}


bool EdgeModel::setData(
   const QModelIndex & index,
   const QVariant & value,
   int role
)
{
   if (role == Qt::EditRole && index.isValid() && index.column() == CID_VALUE &&
      m_organism && m_edge)
   {
      if (index.row() == EP_EQUILIBRIUM_LENGTH)
      {
         bool ok = false;
         float equilibriumLength = value.toFloat(&ok);
         if (ok && equilibriumLength >= 0.0f)
         {
            m_organism->resizeEdge(*m_edge, equilibriumLength);
            emit dataChanged(index, index);
            return true;
         }
      }
   }
   return false;
}


void EdgeModel::resizeEdge(float delta)
{
   if (m_organism && m_edge)
   {
      setData(
         index(EP_EQUILIBRIUM_LENGTH, CID_VALUE),
         m_organism->mesh().edges()[*m_edge].equilibriumLength + delta
      );
   }
}
