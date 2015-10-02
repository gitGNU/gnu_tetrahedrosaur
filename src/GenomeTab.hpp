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


#ifndef GENOMETAB_HPP
#define GENOMETAB_HPP


#include <boost/shared_ptr.hpp>


QT_FORWARD_DECLARE_CLASS(QTableView)


#include "MainWindowTab.hpp"
class ChromosomeDiffModel;
class GenomeModel;
struct GuiOrganismDesc;


class GenomeTab : public MainWindowTab
{
   public:
      explicit GenomeTab(QWidget * parent = 0);
      virtual ~GenomeTab();

      void setOrganismDesc(boost::shared_ptr<const GuiOrganismDesc> desc);

   private:
      GenomeModel * m_genomeModel;
      QTableView * m_chromosomeDiffView;
      ChromosomeDiffModel * m_chromosomeDiffModel;
};


#endif
