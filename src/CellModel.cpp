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


static const char * _context = "CellModel";
static const char * _tetrahedron = QT_TRANSLATE_NOOP("CellModel", "tetrahedron");
static const char * _volume = QT_TRANSLATE_NOOP("CellModel", "volume");
static const char * _generation = QT_TRANSLATE_NOOP("CellModel", "generation");


#include <QtCore/QCoreApplication>


#include "CellModel.hpp"


#include "bio/Cell.hpp"
#include "bio/Organism.hpp"
#include "mesh/Mesh.hpp"


/*
text                   internalPionter        parent                 row
========================================================================
tetrahedron            0                      -                        0
|-- a                  &m_mesh                0, 0, 0                  0
|-- b                  &m_mesh                0, 0, 0                  1
|-- c                  &m_mesh                0, 0, 0                  2
|-- d                  &m_mesh                0, 0, 0                  3
volume                 0                      -                        1
generation             0                      -                        2
x                      0                      -                        3
y                      0                      -                        4
*/


#define _SWITCH_COLUMN(property, value) \
   switch (index.column()) \
   { \
      case CID_PROPERTY: \
         return property; \
      case CID_VALUE: \
         return value; \
   }


enum CELL_PROPERTY
{
   CP_TETRAHEDRON = 0,
   CP_VOLUME,
   CP_GENERATION,
   CP_X,
   CP_Y,
   CellPropertyCount
};


enum TETRAHEDRON_PROPERTY
{
   TP_A = 0,
   TP_B,
   TP_C,
   TP_D,
   TetrahedronPropertyCount
};


/***************************************************************************
 *   CellModel class implementation                                        *
 ***************************************************************************/


CellModel::CellModel(QObject * parent)
   : PropertyModel(parent), m_mesh(0)
{
}


void CellModel::setOrganism(boost::shared_ptr<bio::Organism> organism)
{
   beginResetModel();
   m_organism = organism;
   m_tetrahedron = (m_organism ?
      organism->selectedCell() :
      boost::optional<mesh::Tetrahedron>()
   );
   endResetModel();
}


bool CellModel::rootIsDecorated() const
{
   return true;
}


QVariant CellModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid() && m_organism && m_tetrahedron)
   {
      if (role == Qt::DisplayRole || role == Qt::EditRole)
      {
         const void * ptr = index.internalPointer();
         const int row = index.row();
         if (!ptr)
         {
            switch (row)
            {
               case CP_TETRAHEDRON:
                  _SWITCH_COLUMN(
                     QCoreApplication::translate(_context, _tetrahedron),
                     QString("[...]")
                  );
                  break;
               case CP_VOLUME:
                  _SWITCH_COLUMN(
                     QCoreApplication::translate(_context, _volume),
                     m_organism->mesh().tetrahedronVolume(*m_tetrahedron)
                  );
                  break;
               case CP_GENERATION:
                  switch (index.column())
                  {
                     case CID_PROPERTY:
                        return QCoreApplication::translate(
                           _context, _generation
                        );
                     case CID_VALUE:
                        if (const bio::Cell * cell = // FIXME: find;
                           m_organism->tetrahedronsMap().findAny(*m_tetrahedron)
                        )
                        {
                           return static_cast<int>(cell->generation());
                        }
                        break;
                  }
                  break;
               case CP_X:
                  switch (index.column())
                  {
                     case CID_PROPERTY:
                        return QString("x");
                     case CID_VALUE:
                        if (const bio::Cell * cell = // FIXME: find;
                           m_organism->tetrahedronsMap().findAny(*m_tetrahedron))
                        {
                           return static_cast<int>(cell->x());
                        }
                        break;
                  }
                  break;
               case CP_Y:
                  switch (index.column())
                  {
                     case CID_PROPERTY:
                        return QString("y");
                     case CID_VALUE:
                        if (const bio::Cell * cell = // FIXME: find;
                           m_organism->tetrahedronsMap().findAny(*m_tetrahedron))
                        {
                           return static_cast<int>(cell->y());
                        }
                        break;
                  }
                  break;
            }
         }
         else if (ptr == &m_mesh)
         {
            if (row == TP_A)
            {
               _SWITCH_COLUMN(
                  QString("a"),
                  static_cast<qulonglong>(m_tetrahedron->a)
               );
            }
            else if (row == TP_B)
            {
               _SWITCH_COLUMN(
                  QString("b"),
                  static_cast<qulonglong>(m_tetrahedron->b)
               );
            }
            else if (row == TP_C)
            {
               _SWITCH_COLUMN(
                  QString("c"),
                  static_cast<qulonglong>(m_tetrahedron->c)
               );
            }
            else if (row == TP_D)
            {
               _SWITCH_COLUMN(
                  QString("d"),
                  static_cast<qulonglong>(m_tetrahedron->d)
               );
            }
         }
         else
         {
            Q_ASSERT_X(0, __FUNCTION__, "Invalid internalPointer for index");
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags CellModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;
   if (index.isValid() && m_organism && m_tetrahedron)
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
   }
   return flags;
}


QModelIndex CellModel::index(
   int row,
   int column,
   const QModelIndex & parent
) const
{
   if (m_organism && m_tetrahedron && row >= 0 && column >= 0 &&
      column < ColumnCount)
   {
      if (!parent.isValid())
      {
         if (row < CellPropertyCount)
         {
            return createIndex(row, column);
         }
      }
      else if (!parent.internalPointer())
      {
         if (!parent.column())
         {
            switch (parent.row())
            {
               case CP_TETRAHEDRON:
                  if (row < TetrahedronPropertyCount)
                  {
                     return createIndex(
                        row,
                        column,
                        static_cast<void *>(&m_mesh)
                     );
                  }
                  break;
            }
         }
      }
   }
   return QModelIndex();
}


QModelIndex CellModel::parent(const QModelIndex & index) const
{
   if (index.isValid() && m_organism && m_tetrahedron)
   {
      const void * ptr = index.internalPointer();
      if (!ptr)
      {
         return QModelIndex();
      }
      else if (ptr == &m_mesh)
      {
         return createIndex(CP_TETRAHEDRON, 0);
      }
      Q_ASSERT_X(0, __FUNCTION__, "Invalid internalPointer for index");
   }
   return QModelIndex();
}


int CellModel::rowCount(const QModelIndex & parent) const
{
   if (m_organism && m_tetrahedron)
   {
      if (!parent.isValid())
      {
         return CellPropertyCount;
      }
      else if (!parent.internalPointer())
      {
         if (!parent.column())
         {
            switch (parent.row())
            {
               case CP_TETRAHEDRON:
                  return TetrahedronPropertyCount;
            }
         }
      }
   }
   return 0;
}
