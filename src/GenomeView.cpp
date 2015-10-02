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


#include <QtWidgets/QHeaderView>


#include "GenomeModel.hpp"
#include "GenomeView.hpp"


#include "bio/Genome.hpp"


/***************************************************************************
 *   GenomeView class implementation                                       *
 ***************************************************************************/


GenomeView::GenomeView(QWidget * parent)
   : QTableView(parent)
{
   setAlternatingRowColors(true);
   setSelectionBehavior(QAbstractItemView::SelectRows);
   setSelectionMode(QAbstractItemView::SingleSelection);
   verticalHeader()->hide();
   verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void GenomeView::setModel(QAbstractItemModel * model)
{
   m_genomeModel = dynamic_cast<GenomeModel *>(model);
   QTableView::setModel(model);
}

void GenomeView::selectionChanged(
   const QItemSelection & selected,
   const QItemSelection & deselected
)
{
   QTableView::selectionChanged(selected, deselected);
   const QModelIndexList selection = selected.indexes();
   if (!selection.isEmpty() && m_genomeModel)
   {
      const GenomeModel::Entry entry = m_genomeModel->entry(selection.front());
      emit selectionChanged(entry.genome, entry.left, entry.right);
   }
   else
   {
      emit selectionChanged(
         boost::shared_ptr<const bio::Genome>(),
         boost::none,
         boost::none
      );
   }
}
