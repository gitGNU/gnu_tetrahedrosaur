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


static const char * _context = "GenomeModel";
static const char * _left = QT_TRANSLATE_NOOP("GenomeModel", "Left");
static const char * _right = QT_TRANSLATE_NOOP("GenomeModel", "Right");
static const char * _match = QT_TRANSLATE_NOOP("GenomeModel", "Match");


#include "GenomeModel.hpp"
#include "translation.hpp"


#include "algo/pairing_pair.hpp"
#include "bio/Genome.hpp"
#include "bio/InstructionSet.hpp"


/***************************************************************************
 *   GenomeModel class implementation                                      *
 ***************************************************************************/


GenomeModel::GenomeModel(QObject * parent)
   : QAbstractTableModel(parent)
{
}


boost::shared_ptr<const bio::Genome> GenomeModel::genome() const
{
   return m_genome;
}


void GenomeModel::setGenome(boost::shared_ptr<const bio::Genome> genome)
{
   beginResetModel();
   m_genome = genome;
   endResetModel();
}


GenomeModel::Entry GenomeModel::entry(const QModelIndex & index) const
{
   Entry entry;
   if (index.isValid() && m_genome)
   {
      const size_t row = index.row();
      const std::vector<algo::pairing::Pair<float> > & pairs =
         m_genome->pairs();
      if (row < pairs.size())
      {
         const algo::pairing::Pair<float> & pair = pairs[row];
         entry.genome = m_genome;
         entry.left = dt::ChromosomeId(pair.left);
         if (pair.right)
         {
            entry.right = dt::ChromosomeId(*pair.right);
         }
      }
   }
   return entry;
}


int GenomeModel::columnCount(const QModelIndex &) const
{
   return ColumnCount;
}


QVariant GenomeModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid() && m_genome)
   {
      if (role == Qt::DisplayRole)
      {
         const size_t row = index.row();
         const std::vector<bio::Chromosome> & chromosomes =
            m_genome->chromosomes();
         const std::vector<algo::pairing::Pair<float> > & pairs =
            m_genome->pairs();
         if (row < pairs.size())
         {
            const algo::pairing::Pair<float> & pair = pairs[row];
            switch (index.column())
            {
               case CID_LEFT:
                  Q_ASSERT(pair.left < chromosomes.size());
                  return static_cast<int>(
                     chromosomes[pair.left].code().size()
                  );
               case CID_RIGHT:
                  if (pair.right)
                  {
                     Q_ASSERT(*pair.right < chromosomes.size());
                     return static_cast<int>(
                        chromosomes[*pair.right].code().size()
                     );
                  }
                  break;
               case CID_MATCH:
                  if (pair.metric)
                  {
                     return QString("%1%")
                        .arg((*pair.metric) * 100.0f, 0, 'f', 0);
                  }
                  break;
            }
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags GenomeModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;
   if (index.isValid() && m_genome)
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
   }
   return flags;
}


QVariant GenomeModel::headerData(
   int section,
   Qt::Orientation orientation,
   int role
) const
{
   if (orientation == Qt::Horizontal)
   {
      if (role == Qt::DisplayRole)
      {
         switch (section)
         {
            case CID_LEFT:
               return TSLC(_left);
            case CID_RIGHT:
               return TSLC(_right);
            case CID_MATCH:
               return TSLC(_match);
         }
      }
   }
   return QVariant();
}


int GenomeModel::rowCount(const QModelIndex & parent) const
{
   if (m_genome && !parent.isValid())
   {
      return m_genome->pairs().size();
   }
   return 0;
}
