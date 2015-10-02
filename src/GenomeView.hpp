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


#ifndef GENOMEVIEW_HPP
#define GENOMEVIEW_HPP


#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>


#include <QtCore/QPointer>
#include <QtWidgets/QTableView>


class GenomeModel;


#include "datatypes/biology.hpp"


namespace bio {
class Genome;
}


/***************************************************************************
 *   GenomeView class declaration                                          *
 ***************************************************************************/


class GenomeView : public QTableView
{
   Q_OBJECT

   public:
      explicit GenomeView(QWidget * parent = 0);

      virtual void setModel(QAbstractItemModel * model);

   protected:
      virtual void selectionChanged(
         const QItemSelection & selected,
         const QItemSelection & deselected
      );

   signals:
      void selectionChanged(
         boost::shared_ptr<const bio::Genome> genome,
         const boost::optional<dt::ChromosomeId> &left,
         const boost::optional<dt::ChromosomeId> &right
      );

   private:
      QPointer<GenomeModel> m_genomeModel;
};


#endif
