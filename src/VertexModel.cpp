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


#include "VertexModel.hpp"


#include "bio/Organism.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/Vertex.hpp"


/*
text                   internalPionter        parent                 row
========================================================================
id                     0                      -                       0
x                      0                      -                       1
y                      0                      -                       2
z                      0                      -                       3
selected               0                      -                       4
nx                     0                      -                       5
ny                     0                      -                       6
nz                     0                      -                       7
k                      0                      -                       8
vx                     0                      -                       9
vy                     0                      -                      10
vz                     0                      -                      11
mass                   0                      -                      12
*/


#define _SWITCH_COLUMN(property, value) \
   switch (index.column()) \
   { \
      case CID_PROPERTY: \
         return property; \
      case CID_VALUE: \
         return value; \
   }


#define _SET_VERTEX_FLOAT_PROPERTY(setVertexProperty) \
   { \
      bool ok = false; \
      float f = value.toFloat(&ok); \
      if (ok) \
      { \
         m_organism->setVertexProperty(*m_vertex, f); \
         emit dataChanged(index, index); \
         return true; \
      } \
   }


enum VERTEX_PROPERTY
{
   VP_ID = 0,
   VP_X,
   VP_Y,
   VP_Z,
   VP_SELECTED,
   VP_NX,
   VP_NY,
   VP_NZ,
   VP_K,
   VP_VX,
   VP_VY,
   VP_VZ,
   VP_MASS,
   VertexPropertyCount
};


/***************************************************************************
 *   VertexModel class implementation                                      *
 ***************************************************************************/


VertexModel::VertexModel(QObject * parent)
   : PropertyModel(parent)
{
}


void VertexModel::setOrganism(boost::shared_ptr<bio::Organism> organism)
{
   beginResetModel();
   m_organism = organism;
   m_vertex = (m_organism ?
      organism->mesh().selectedVertex() :
      boost::optional<dt::VertexId>()
   );
   endResetModel();
}


QVariant VertexModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid() && m_organism && m_vertex)
   {
      if (role == Qt::DisplayRole || role == Qt::EditRole)
      {
         const mesh::DynamicVertex & dv =
            m_organism->mesh().dynamicVertices()[m_vertex->get()];
         switch (index.row())
         {
            case VP_ID:
               _SWITCH_COLUMN(
                  QString("id"),
                  static_cast<qulonglong>(m_vertex->get())
               );
               break;
            case VP_X:
               _SWITCH_COLUMN(QString("x"), dv.x);
               break;
            case VP_Y:
               _SWITCH_COLUMN(QString("y"), dv.y);
               break;
            case VP_Z:
               _SWITCH_COLUMN(QString("z"), dv.z);
               break;
            case VP_SELECTED:
               _SWITCH_COLUMN(QString("selected"), dv.selected);
               break;
            case VP_NX:
               _SWITCH_COLUMN(QString("nx"), dv.nx);
               break;
            case VP_NY:
               _SWITCH_COLUMN(QString("ny"), dv.ny);
               break;
            case VP_NZ:
               _SWITCH_COLUMN(QString("nz"), dv.nz);
               break;
            case VP_K:
               _SWITCH_COLUMN(QString("k"), dv.k);
               break;
            case VP_VX:
               _SWITCH_COLUMN(QString("vx"), dv.vx);
               break;
            case VP_VY:
               _SWITCH_COLUMN(QString("vy"), dv.vy);
               break;
            case VP_VZ:
               _SWITCH_COLUMN(QString("vz"), dv.vz);
               break;
            case VP_MASS:
               _SWITCH_COLUMN(QString("mass"), dv.mass);
               break;
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags VertexModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;
   if (index.isValid() && m_organism && m_vertex)
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
      if (index.column() == CID_VALUE)
      {
         switch (index.row())
         {
            case VP_X:
            case VP_Y:
            case VP_Z:
            case VP_MASS:
               flags |= Qt::ItemIsEditable;
               break;
         }
      }
   }
   return flags;
}


QModelIndex VertexModel::index(
   int row,
   int column,
   const QModelIndex & parent
) const
{
   if (m_organism && m_vertex && row >= 0 && column >= 0 &&
      column < ColumnCount)
   {
      if (!parent.isValid() && row < VertexPropertyCount)
      {
         return createIndex(row, column);
      }
   }
   return QModelIndex();
}


QModelIndex VertexModel::parent(const QModelIndex &) const
{
   return QModelIndex();
}


int VertexModel::rowCount(const QModelIndex & parent) const
{
   if (m_organism && m_vertex && !parent.isValid())
   {
      return VertexPropertyCount;
   }
   return 0;
}


bool VertexModel::setData(
   const QModelIndex & index,
   const QVariant & value,
   int role
)
{
   if (role == Qt::EditRole && index.isValid() && index.column() == CID_VALUE &&
      m_organism && m_vertex)
   {
      switch (index.row())
      {
         case VP_X:
            _SET_VERTEX_FLOAT_PROPERTY(setVertexPosX)
            break;
         case VP_Y:
            _SET_VERTEX_FLOAT_PROPERTY(setVertexPosY)
            break;
         case VP_Z:
            _SET_VERTEX_FLOAT_PROPERTY(setVertexPosZ)
            break;
         case VP_MASS:
            _SET_VERTEX_FLOAT_PROPERTY(setVertexMass)
            break;
      }
   }
   return false;
}
