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


static const char * _context = "GeneModel";
static const char * _generation = QT_TRANSLATE_NOOP("GeneModel", "Generation");
static const char * _budAbc = QT_TRANSLATE_NOOP("GeneModel", "Bud ABC");
static const char * _budAcd = QT_TRANSLATE_NOOP("GeneModel", "Bud ACD");
static const char * _budAdb = QT_TRANSLATE_NOOP("GeneModel", "Bud ADB");
static const char * _budBcd = QT_TRANSLATE_NOOP("GeneModel", "Bud BCD");


#include "GeneModel.hpp"
#include "translation.hpp"


#include "bio/Config.hpp"
#include "bio/Genome.hpp"
#include "bio/InstructionSet.hpp"


namespace {


bool _getBudString(
   const bio::Instruction & instr,
   const bio::Config & config,
   bio::Opcode bud,
   bio::Opcode nobud,
   QString & str
)
{
   const bio::Opcode op = config.opcode(instr.cmd());
   if (op == bud)
   {
      static const dt::Float deg = 180.0f / M_PI;
      const int16_t ang = instr.i16().get();
      dt::Float r = config.budTopRadius.convert(instr.i8());
      dt::Float p = config.budTopPolarAngle.convert(dt::Int8(0xFF & ang)) * deg;
      dt::Float a = config.budTopAzimuthalAngle.convert(
         dt::Int8(0xFF & (ang >> 8))
      ) * deg;
      str = QString("r = %1, p = %2, a = %3").arg(r, 0, 'f', 2)
         .arg(p, 0, 'f', 2).arg(a, 0, 'f', 2);
      return true;
   }
   else if (op == nobud)
   {
      str = "no bud";
      return true;
   }
   return false;
}


QString _budToString(
   const std::vector<bio::Instruction> & code,
   const bio::Config & config,
   bio::Opcode bud,
   bio::Opcode nobud,
   size_t hint = 0
)
{
   QString str;
   const size_t count = code.size();
   for (size_t i = hint; i < count; ++i)
   {
      if (_getBudString(code[i], config, bud, nobud, str))
      {
         return str;
      }
   }
   for (size_t i = 0, h = std::min(hint, count); i < h; ++i)
   {
      if (_getBudString(code[i], config, bud, nobud, str))
      {
         return str;
      }
   }
   return str;
}


} // anonymous namespace;


/***************************************************************************
 *   GeneModel class implementation                                        *
 ***************************************************************************/


GeneModel::GeneModel(QObject * parent)
   : QAbstractTableModel(parent)
{
}


void GeneModel::setGenome(boost::shared_ptr<const bio::Genome> genome)
{
   beginResetModel();
   m_genome = genome;
   endResetModel();
}


int GeneModel::columnCount(const QModelIndex &) const
{
   return ColumnCount;
}


QVariant GeneModel::data(const QModelIndex & index, int role) const
{
   if (index.isValid() && m_genome)
   {
      if (role == Qt::DisplayRole)
      {
         const size_t row = index.row();
         const std::vector<bio::Gene> & genes = m_genome->genes();
         if (row < genes.size())
         {
            const bio::Gene & gene = genes[row];
            switch (index.column())
            {
               case CID_GENERATION:
                  return QString::fromStdString(
                     gene.generationCondition.toString()
                  );
               case CID_X:
                  return QString::fromStdString(gene.xCondition.toString());
               case CID_Y:
                  return QString::fromStdString(gene.yCondition.toString());
               case CID_BUD_ABC:
                  return _budToString(
                     gene.responses,
                     *m_genome->config(),
                     bio::OP_RBABC,
                     bio::OP_RNBABC,
                     0
                  );
               case CID_BUD_ACD:
                  return _budToString(
                     gene.responses,
                     *m_genome->config(),
                     bio::OP_RBACD,
                     bio::OP_RNBACD,
                     1
                  );
               case CID_BUD_ADB:
                  return _budToString(
                     gene.responses,
                     *m_genome->config(),
                     bio::OP_RBADB,
                     bio::OP_RNBADB,
                     2
                  );
               case CID_BUD_BCD:
                  return _budToString(
                     gene.responses,
                     *m_genome->config(),
                     bio::OP_RBADB,
                     bio::OP_RNBADB,
                     3
                  );
            }
         }
      }
   }
   return QVariant();
}


Qt::ItemFlags GeneModel::flags(const QModelIndex & index) const
{
   Qt::ItemFlags flags = Qt::NoItemFlags;
   if (index.isValid() && m_genome)
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
   }
   return flags;
}


QVariant GeneModel::headerData(
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
            case CID_GENERATION:
               return TSLC(_generation);
            case CID_X:
               return "X";
            case CID_Y:
               return "Y";
            case CID_BUD_ABC:
               return TSLC(_budAbc);
            case CID_BUD_ACD:
               return TSLC(_budAcd);
            case CID_BUD_ADB:
               return TSLC(_budAdb);
            case CID_BUD_BCD:
               return TSLC(_budBcd);
         }
      }
   }
   return QVariant();
}


int GeneModel::rowCount(const QModelIndex & parent) const
{
   if (m_genome && !parent.isValid())
   {
      return m_genome->genes().size();
   }
   return 0;
}
