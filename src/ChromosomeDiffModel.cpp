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


static const char * _context = "ChromosomeDiffModel";
static const char * _left = QT_TRANSLATE_NOOP("ChromosomeDiffModel", "Left");
static const char * _right = QT_TRANSLATE_NOOP("ChromosomeDiffModel", "Right");


#include <QtCore/QCoreApplication>
#include <QtGui/QColor>


#include "ChromosomeDiffModel.hpp"


#include "algo/Alignment.hpp"
#include "bio/Config.hpp"
#include "bio/Genome.hpp"
#include "bio/InstructionSet.hpp"


/***************************************************************************
 *   ChromosomeDiffModel class implementation                              *
 ***************************************************************************/


ChromosomeDiffModel::ChromosomeDiffModel(
   Qt::Orientation orientation,
   QObject * parent
) : QAbstractTableModel(parent), m_orientation(orientation)
{
}


ChromosomeDiffModel::~ChromosomeDiffModel()
{
}


void ChromosomeDiffModel::setOrientation(Qt::Orientation orientation)
{
   beginResetModel();
   m_orientation = orientation;
   endResetModel();
}


int ChromosomeDiffModel::columnCount(const QModelIndex &) const
{
   return ((m_orientation == Qt::Vertical) ? RowColumnCount : m_items.size());
}


QVariant ChromosomeDiffModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid() && m_genome)
   {
      if (role == Qt::DisplayRole || role == Qt::BackgroundRole)
      {
         const size_t i = ((m_orientation == Qt::Vertical) ?
            index.row() : index.column()
         );
         const size_t j = ((m_orientation == Qt::Vertical) ?
            index.column() : index.row()
         );
         if (i < m_items.size())
         {
            const Item & item = m_items[i];
            if (role == Qt::DisplayRole)
            {
               switch (j)
               {
                  case RCID_LEFT:
                     if (item.left && m_instructionEquals)
                     {
                        return QString::fromStdString(
                           m_instructionEquals->toString(*item.left)
                        );
                     }
                     break;
                  case RCID_RIGHT:
                     if (item.right && m_instructionEquals)
                     {
                        return QString::fromStdString(
                           m_instructionEquals->toString(*item.right)
                        );
                     }
                     break;
               }
            }
            else if (role == Qt::BackgroundRole)
            {
               switch (j)
               {
                  case RCID_LEFT:
                     if (item.left && m_instructionEquals)
                     {
                        if (item.right && !m_instructionEquals->operator()(
                           *item.left, *item.right)
                        )
                        {
                           return QColor(0xdd, 0xee, 0xff);
                        }
                        if (!item.right)
                        {
                           return QColor(0xc1, 0xff, 0xc1);
                        }
                     }
                     break;
                  case RCID_RIGHT:
                     if (item.right && m_instructionEquals)
                     {
                        if (item.left && !m_instructionEquals->operator()(
                           *item.left, *item.right)
                        )
                        {
                           return QColor(0xdd, 0xee, 0xff);
                        }
                        if (!item.left)
                        {
                           return QColor(0xc1, 0xff, 0xc1);
                        }
                     }
                     break;
               }
            }
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags ChromosomeDiffModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;
   if (index.isValid() && m_genome)
   {
      flags = Qt::ItemIsEnabled;
   }
   return flags;
}


QVariant ChromosomeDiffModel::headerData(
   int section,
   Qt::Orientation orientation,
   int role
) const
{
   if (orientation != m_orientation)
   {
      if (role == Qt::DisplayRole)
      {
         switch (section)
         {
            case RCID_LEFT:
               return QCoreApplication::translate(_context, _left);
            case RCID_RIGHT:
               return QCoreApplication::translate(_context, _right);
         }
      }
   }
   return QVariant();
}


int ChromosomeDiffModel::rowCount(const QModelIndex & parent) const
{
   if (!parent.isValid())
   {
      return ((m_orientation == Qt::Vertical) ?
         m_items.size() : RowColumnCount
      );
   }
   return 0;
}


void ChromosomeDiffModel::setChromosomes(
   boost::shared_ptr<const bio::Genome> genome,
   const boost::optional<dt::ChromosomeId> &left,
   const boost::optional<dt::ChromosomeId> &right)
{
   beginResetModel();
   m_genome = genome;
   m_instructionEquals.reset(m_genome ?
      new bio::InstructionEquals(m_genome->config()) :
      nullptr
   );
   m_items.clear();
   if (m_genome)
   {
      Q_ASSERT(m_instructionEquals);
      const std::vector<bio::Chromosome> & chromosomes =
         m_genome->chromosomes();
      if (left && right)
      {
         const std::vector<bio::Instruction> &leftCode =
            chromosomes[left->get()].code();
         const std::vector<bio::Instruction> &rightCode =
            chromosomes[right->get()].code();
         const bio::Instruction *leftData = leftCode.data();
         const bio::Instruction *rightData = rightCode.data();
         const algo::Alignment alignment(
            leftCode,
            rightCode,
            *m_instructionEquals
         );
         m_items.reserve(std::max(alignment.leftSize(), alignment.rightSize()));
         algo::Alignment::const_iterator it = alignment.begin();
         algo::Alignment::const_iterator ite = alignment.end();
         for (; it != ite; ++it)
         {
            const boost::optional<size_t> left = it.left();
            const boost::optional<size_t> right = it.right();
            Q_ASSERT(left || right);
            m_items.push_back(Item(
               left ? (leftData + *left) : 0,
               right ? (rightData + *right) : 0
            ));
         }
         m_items.shrink_to_fit();
      }
      else if (left)
      {
         const std::vector<bio::Instruction> &leftCode =
            chromosomes[left->get()].code();
         const bio::Instruction *leftData = leftCode.data();
         const size_t count = leftCode.size();
         m_items.reserve(count);
         for (size_t i = 0; i < count; ++i)
         {
            m_items.push_back(Item(leftData + i, 0));
         }
      }
      else if (right)
      {
         const std::vector<bio::Instruction> &rightCode =
            chromosomes[right->get()].code();
         const bio::Instruction *rightData = rightCode.data();
         const size_t count = rightCode.size();
         m_items.reserve(count);
         for (size_t i = 0; i < count; ++i)
         {
            m_items.push_back(Item(0, rightData + i));
         }
      }
   }
   endResetModel();
}


ChromosomeDiffModel::Item::Item(
   const bio::Instruction * left,
   const bio::Instruction * right
) : left(left), right(right)
{}
