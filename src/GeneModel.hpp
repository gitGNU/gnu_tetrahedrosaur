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


#ifndef GENEMODEL_HPP
#define GENEMODEL_HPP


#include <boost/shared_ptr.hpp>


#include <QtCore/QAbstractTableModel>


namespace bio {
class Genome;
}


/***************************************************************************
 *   GeneModel class declaration                                           *
 ***************************************************************************/


class GeneModel : public QAbstractTableModel
{
   public:
      explicit GeneModel(QObject * parent = 0);

      void setGenome(boost::shared_ptr<const bio::Genome> genome);

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
         CID_GENERATION = 0,
         CID_X,
         CID_Y,
         CID_BUD_ABC,
         CID_BUD_ACD,
         CID_BUD_ADB,
         CID_BUD_BCD,
         ColumnCount
      };

      boost::shared_ptr<const bio::Genome> m_genome;
};


#endif
