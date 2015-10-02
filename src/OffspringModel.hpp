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


#ifndef OFFSPRINGMODEL_HPP
#define OFFSPRINGMODEL_HPP


#include <vector>


#include <QtGui/QPixmap>


#include "PopulationModel.hpp"


class DevelopmentEngine;


namespace mesh {
class Figure;
}


/***************************************************************************
 *   OffspringModel class declaration                                      *
 ***************************************************************************/


class OffspringModel : public PopulationModel
{
   Q_OBJECT

   public:
      explicit OffspringModel(QObject * parent = 0);
      virtual ~OffspringModel();

      bool isReady() const;

      void append(
         const std::vector<boost::shared_ptr<GuiOrganismDesc> > & offsprings
      );
      void clear();
      boost::shared_ptr<GuiOrganismDesc> takeOrganismDesc(
         const QModelIndex & index
      );

      virtual boost::shared_ptr<const GuiOrganismDesc> organismDesc(
         const QModelIndex & index
      ) const;

      virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
      virtual QVariant data(
         const QModelIndex & index,
         int role = Qt::DisplayRole
      ) const;
      virtual Qt::ItemFlags flags(const QModelIndex & index) const;
      virtual bool removeColumns(
         int column,
         int count,
         const QModelIndex & parent = QModelIndex()
      );
      virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

   private slots:
      void emitIndexStarted(size_t itemIndex);
      void setItemProgress(size_t itemIndex, int progress);
      void setItemFigure(
         size_t itemIndex,
         boost::shared_ptr<mesh::Figure> figure,
         QPixmap portrait
      );

   signals:
      void indexStarted(const QModelIndex & index);
      void finished();

   private:
      struct Item
      {
         explicit Item();
         ~Item();

         boost::shared_ptr<GuiOrganismDesc> desc;
         uint8_t progress;
      };

      DevelopmentEngine * m_engine;
      size_t m_baseItemIndex;
      std::vector<Item> m_data;
};


#endif
