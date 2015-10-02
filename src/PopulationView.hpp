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


#ifndef POPULATIONVIEW_HPP
#define POPULATIONVIEW_HPP


#include <boost/shared_ptr.hpp>


#include <QtCore/QPointer>
#include <QtWidgets/QTableView>
QT_FORWARD_DECLARE_CLASS(QMenu)


class PopulationModel;


namespace mesh {
class Figure;
}


/***************************************************************************
 *   PopulationView class declaration                                      *
 ***************************************************************************/


class PopulationView : public QTableView
{
   Q_OBJECT

   public:
      explicit PopulationView(QWidget * parent = 0);

      virtual QSize sizeHint() const;

      virtual void setModel(QAbstractItemModel * model);

      boost::shared_ptr<const mesh::Figure> selectedFigure() const;
      bool isSomethingSelected() const;
      void clearSelection();

   public slots:
      void scrollToIndex(const QModelIndex & index);

   protected:
      virtual void contextMenuEvent(QContextMenuEvent * event);
      virtual void selectionChanged(
         const QItemSelection & selected,
         const QItemSelection & deselected
      );

   private slots:
      void openOrganism();
      void openGenome();
      void updateOrientation();

   signals:
      void selectionChanged();

   private:
      QPointer<PopulationModel> m_populationModel;
      QMenu * m_contextMenu;
      QModelIndex m_contextMenuOriginIndex;
};


#endif
