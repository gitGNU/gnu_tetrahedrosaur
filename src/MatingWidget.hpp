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


#ifndef MATINGWIDGET_HPP
#define MATINGWIDGET_HPP


#include <vector>


#include <boost/shared_ptr.hpp>


#include <QtGui/QPixmap>
#include <QtWidgets/QWidget>
QT_FORWARD_DECLARE_CLASS(QPushButton)


struct GuiOrganismDesc;


namespace bio {
struct MutationParams;
}


/***************************************************************************
 *   MatingWidget class declaration                                        *
 ***************************************************************************/


class MatingWidget : public QWidget
{
   Q_OBJECT

   public:
      explicit MatingWidget(QWidget * parent = 0);
      virtual ~MatingWidget();

      virtual QSize sizeHint() const;

      void setLeftParent(boost::shared_ptr<const GuiOrganismDesc> leftParent);
      void setRightParent(boost::shared_ptr<const GuiOrganismDesc> rightParent);

      std::vector<boost::shared_ptr<GuiOrganismDesc> > mate(
         size_t count,
         const bio::MutationParams & params
      ) const;

   public slots:
      void clear();
      void swap();

   protected:
      virtual void paintEvent(QPaintEvent * event);

   private:
      QPushButton * m_clearButton;
      QPushButton * m_swapButton;
      QPixmap m_leftPortrait;
      QPixmap m_rightPortrait;
      boost::shared_ptr<const GuiOrganismDesc> m_leftParent;
      boost::shared_ptr<const GuiOrganismDesc> m_rightParent;
};


#endif
