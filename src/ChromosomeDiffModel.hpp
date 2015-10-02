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


#ifndef CHROMOSOMEDIFFMODEL_HPP
#define CHROMOSOMEDIFFMODEL_HPP


#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>


#include <QtCore/QAbstractTableModel>


#include "datatypes/biology.hpp"


namespace bio {
class Instruction;
class InstructionEquals;
class Genome;
}


/***************************************************************************
 *   ChromosomeDiffModel class declaration                                 *
 ***************************************************************************/


class ChromosomeDiffModel : public QAbstractTableModel
{
   Q_OBJECT

   public:
      explicit ChromosomeDiffModel(
         Qt::Orientation orientation,
         QObject * parent = 0
      );
      virtual ~ChromosomeDiffModel();

      void setOrientation(Qt::Orientation orientation);

      virtual int columnCount(
         const QModelIndex & parent = QModelIndex()
      ) const;
      virtual QVariant data(
         const QModelIndex & index,
         int role = Qt::DisplayRole
      ) const;
      virtual Qt::ItemFlags flags(const QModelIndex & index) const;
      virtual QVariant headerData(
         int section,
         Qt::Orientation orientation,
         int role = Qt::DisplayRole
      ) const;
      virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

   public slots:
      void setChromosomes(
         boost::shared_ptr<const bio::Genome> genome,
         const boost::optional<dt::ChromosomeId> &left,
         const boost::optional<dt::ChromosomeId> &right
      );

   private:
      enum ROW_COLUMN_ID
      {
         RCID_LEFT = 0,
         RCID_RIGHT,
         RowColumnCount
      };

      struct Item
      {
         explicit Item(
            const bio::Instruction * left,
            const bio::Instruction * right
         );

         const bio::Instruction * left;
         const bio::Instruction * right;
      };

      boost::shared_ptr<const bio::Genome> m_genome;
      boost::scoped_ptr<const bio::InstructionEquals> m_instructionEquals;
      std::vector<Item> m_items;
      Qt::Orientation m_orientation;
};


#endif
