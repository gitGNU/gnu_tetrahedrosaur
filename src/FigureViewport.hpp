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


#ifndef FIGUREVIEWPORT_HPP
#define FIGUREVIEWPORT_HPP


#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>


#include "Viewport.hpp"


class Figure3D;


namespace mesh {
class Figure;
}


/***************************************************************************
 *   FigureViewport class declaration                                      *
 ***************************************************************************/


class FigureViewport : public Viewport
{
   Q_OBJECT

   public:
      explicit FigureViewport(QWidget *parent = 0);
      virtual ~FigureViewport();

      boost::shared_ptr<const mesh::Figure> figure() const;

   public slots:
      void setFigure(boost::shared_ptr<const mesh::Figure> figure);

   protected:
      virtual void paintGL();

   private:
      boost::scoped_ptr<Figure3D> m_figure3d;
};


#endif
