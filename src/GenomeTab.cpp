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


#include <iostream>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>


#include "ChromosomeDiffModel.hpp"
#include "GenomeModel.hpp"
#include "GenomeTab.hpp"
#include "GenomeView.hpp"
#include "GuiOrganismDesc.hpp"


#include "bio/Genome.hpp"


GenomeTab::GenomeTab(QWidget * parent)
   : MainWindowTab(parent),
   m_genomeModel(0),
   m_chromosomeDiffView(0),
   m_chromosomeDiffModel(0)
{
   m_genomeModel = new GenomeModel(this);

   m_chromosomeDiffModel = new ChromosomeDiffModel(Qt::Vertical, this);

   GenomeView * genomeView = new GenomeView(this);
   genomeView->setModel(m_genomeModel);

   m_chromosomeDiffView = new QTableView(this);
   m_chromosomeDiffView->setShowGrid(false);
   m_chromosomeDiffView->horizontalHeader()->show();
   m_chromosomeDiffView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   m_chromosomeDiffView->verticalHeader()->hide();
   m_chromosomeDiffView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

   m_chromosomeDiffView->setModel(m_chromosomeDiffModel);

   QHBoxLayout * layout = new QHBoxLayout(this);
   layout->addWidget(genomeView, 1);
   layout->addWidget(m_chromosomeDiffView, 1);

   connect(genomeView, SIGNAL(selectionChanged(
         boost::shared_ptr<const bio::Genome>,
         const boost::optional<dt::ChromosomeId> &,
         const boost::optional<dt::ChromosomeId> &)),
      m_chromosomeDiffModel, SLOT(setChromosomes(
         boost::shared_ptr<const bio::Genome>,
         const boost::optional<dt::ChromosomeId> &,
         const boost::optional<dt::ChromosomeId> &))
   );
}


GenomeTab::~GenomeTab()
{
}


void GenomeTab::setOrganismDesc(boost::shared_ptr<const GuiOrganismDesc> desc)
{
   if (desc)
   {
      m_genomeModel->setGenome(desc->genome);
      std::cout << *desc << std::endl;
   }
}
