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
#include "GeneModel.hpp"
#include "GenomeModel.hpp"
#include "GenomeTab.hpp"
#include "GenomeView.hpp"
#include "GuiOrganismDesc.hpp"


#include "bio/Genome.hpp"


GenomeTab::GenomeTab(QWidget * parent)
   : MainWindowTab(parent), m_genomeModel(0), m_geneModel(0)
{
   m_genomeModel = new GenomeModel(this);
   m_geneModel = new GeneModel(this);

   ChromosomeDiffModel * chromosomeDiffModel =
      new ChromosomeDiffModel(Qt::Vertical, this);

   GenomeView * genomeView = new GenomeView(this);
   genomeView->setModel(m_genomeModel);

   QTableView * chromosomeDiffView = new QTableView(this);
   chromosomeDiffView->setShowGrid(false);
   chromosomeDiffView->horizontalHeader()->show();
   chromosomeDiffView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   chromosomeDiffView->verticalHeader()->hide();
   chromosomeDiffView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   chromosomeDiffView->setModel(chromosomeDiffModel);

   QTableView * geneView = new QTableView(this);
   geneView->horizontalHeader()->show();
   geneView->verticalHeader()->hide();
   geneView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   geneView->setModel(m_geneModel);

   QHBoxLayout * layout = new QHBoxLayout(this);
   layout->addWidget(genomeView, 1);
   layout->addWidget(chromosomeDiffView, 1);
   layout->addWidget(geneView, 2);

   connect(genomeView, SIGNAL(selectionChanged(
         boost::shared_ptr<const bio::Genome>,
         const boost::optional<dt::ChromosomeId> &,
         const boost::optional<dt::ChromosomeId> &)),
      chromosomeDiffModel, SLOT(setChromosomes(
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
      m_geneModel->setGenome(desc->genome);
      std::cout << *desc << std::endl;
   }
}
