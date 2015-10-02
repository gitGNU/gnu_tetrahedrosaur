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


#ifndef GENOMEMODEL_HPP
#define GENOMEMODEL_HPP


#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>


#include <QtCore/QAbstractTableModel>


#include "datatypes/biology.hpp"


namespace bio {
class Genome;
}


/***************************************************************************
 *   GenomeModel class declaration                                         *
 ***************************************************************************/


class GenomeModel : public QAbstractTableModel
{
   public:
      struct Entry
      {
         boost::shared_ptr<const bio::Genome> genome;
         boost::optional<dt::ChromosomeId> left;
         boost::optional<dt::ChromosomeId> right;
      };

      explicit GenomeModel(QObject * parent = 0);

      boost::shared_ptr<const bio::Genome> genome() const;
      void setGenome(boost::shared_ptr<const bio::Genome> genome);

      Entry entry(const QModelIndex & index) const;

      virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
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

   private:
      enum COLUMN_ID
      {
         CID_LEFT = 0,
         CID_RIGHT,
         CID_MATCH,
         ColumnCount
      };

      boost::shared_ptr<const bio::Genome> m_genome;
};


#endif
