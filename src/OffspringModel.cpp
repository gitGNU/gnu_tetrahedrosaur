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


#include "custom_enums.hpp"
#include "DevelopmentEngine.hpp"
#include "GuiOrganismDesc.hpp"
#include "OffspringModel.hpp"


#include "mesh/Figure.hpp"


/***************************************************************************
 *   OffspringModel class implementation                                   *
 ***************************************************************************/


OffspringModel::OffspringModel(QObject * parent)
   : PopulationModel(Qt::Horizontal, parent),
   m_engine(0),
   m_baseItemIndex(0)
{
   m_engine = new DevelopmentEngine(this);
   connect(
      m_engine,
      SIGNAL(descStarted(size_t)),
      SLOT(emitIndexStarted(size_t))
   );
   connect(
      m_engine,
      SIGNAL(descProgressChanged(size_t, int)),
      SLOT(setItemProgress(size_t, int))
   );
   connect(
      m_engine,
      SIGNAL(descFinished(size_t, boost::shared_ptr<mesh::Figure>, QPixmap)),
      SLOT(setItemFigure(size_t, boost::shared_ptr<mesh::Figure>, QPixmap))
   );
   connect(m_engine, SIGNAL(allFinished()), SIGNAL(finished()));
}


OffspringModel::~OffspringModel()
{
}


bool OffspringModel::isReady() const
{
   return m_engine->isReady();
}


void OffspringModel::append(
   const std::vector<boost::shared_ptr<GuiOrganismDesc> > & offsprings
)
{
   const size_t count = offsprings.size();
   if (m_engine->isReady() && count)
   {
      const size_t oldSize = m_data.size();
      beginInsertColumns(QModelIndex(), oldSize, oldSize + (count - 1));
      m_baseItemIndex = oldSize;
      m_data.reserve(oldSize + count);
      for (boost::shared_ptr<GuiOrganismDesc> offspring : offsprings)
      {
         Item item;
         item.desc = offspring;
         item.desc->portrait = QPixmap(128, 128);
         item.desc->portrait.fill(QColor(0xbf, 0xbf, 0xbf));
         m_data.push_back(item);
      }
      endInsertColumns();

      m_engine->start(offsprings);
   }
}


void OffspringModel::clear()
{
   if (m_engine->isReady() && !m_data.empty())
   {
      beginResetModel();
      m_baseItemIndex = 0;
      m_data.clear();
      endResetModel();
   }
}


boost::shared_ptr<GuiOrganismDesc> OffspringModel::takeOrganismDesc(
   const QModelIndex & index
)
{
   if (index.isValid() && !index.parent().isValid())
   {
      const size_t col = index.column();
      if (col < m_data.size())
      {
         boost::shared_ptr<GuiOrganismDesc> desc = m_data[col].desc;
         if (removeColumn(col))
         {
            return desc;
         }
      }
   }
   return boost::shared_ptr<GuiOrganismDesc>();
}


boost::shared_ptr<const GuiOrganismDesc> OffspringModel::organismDesc(
   const QModelIndex & index
) const
{
   if (index.isValid())
   {
      const size_t col = index.column();
      if (col < m_data.size())
      {
         return m_data[col].desc;
      }
   }
   return boost::shared_ptr<const GuiOrganismDesc>();
}



int OffspringModel::columnCount(const QModelIndex & parent) const
{
   return (parent.isValid() ? 0 : m_data.size());
}


QVariant OffspringModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid())
   {
      if (role == Qt::DecorationRole)
      {
         const size_t col = index.column();
         if (col < m_data.size())
         {
            return m_data[col].desc->portrait;
         }
      }
      if (role == Custom::ProgressRole)
      {
         const size_t col = index.column();
         if (col < m_data.size())
         {
            return static_cast<int>(m_data[col].progress);
         }
      }
      else if (role == Custom::CloseIndicatorRole)
      {
         const size_t col = index.column();
         if (col < m_data.size())
         {
            return (m_data[col].desc->figure.get() != nullptr);
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags OffspringModel::flags(const QModelIndex & index) const
{
   return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


bool OffspringModel::removeColumns(
   int column,
   int count,
   const QModelIndex & parent
)
{
   if (m_engine->isReady() && count && !parent.isValid() &&
      column >= 0 && (column + count) <= static_cast<int>(m_data.size()))
   {
      beginRemoveColumns(parent, column, column + count - 1);
      m_data.erase(m_data.begin() + column, m_data.begin() + column + count);
      endRemoveColumns();
      return true;
   }
   return false;
}


int OffspringModel::rowCount(const QModelIndex & parent) const
{
   return (parent.isValid() ? 0 : 1);
}


void OffspringModel::emitIndexStarted(size_t itemIndex)
{
   const size_t ii = m_baseItemIndex + itemIndex;
   if (ii < m_data.size())
   {
      const QModelIndex idx = ((orientation() == Qt::Horizontal) ?
         index(0, ii) : index(ii, 0)
      );
      emit indexStarted(idx);
   }
}


void OffspringModel::setItemProgress(size_t itemIndex, int progress)
{
   const size_t ii = m_baseItemIndex + itemIndex;
   if (ii < m_data.size())
   {
      m_data[ii].progress = progress;
      const QModelIndex idx = index(0, ii);
      emit dataChanged(idx, idx);
   }
}


void OffspringModel::setItemFigure(
   size_t itemIndex,
   boost::shared_ptr<mesh::Figure> figure,
   QPixmap portrait
)
{
   const size_t ii = m_baseItemIndex + itemIndex;
   if (ii < m_data.size())
   {
      m_data[ii].desc->figure = figure;
      m_data[ii].desc->portrait = portrait;
      const QModelIndex idx = index(0, ii);
      emit dataChanged(idx, idx);
   }
}


OffspringModel::Item::Item()
   : progress(0)
{
}

OffspringModel::Item::~Item()
{
}
