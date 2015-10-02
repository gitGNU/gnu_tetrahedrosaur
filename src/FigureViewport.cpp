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


#include "Figure3D.hpp"
#include "FigureViewport.hpp"


/***************************************************************************
 *   FigureViewport class implementation                                   *
 ***************************************************************************/


FigureViewport::FigureViewport(QWidget *parent)
   : Viewport(parent)
{
}


FigureViewport::~FigureViewport()
{
}


boost::shared_ptr<const mesh::Figure> FigureViewport::figure() const
{
   return (m_figure3d ? m_figure3d->figure() : 0);
}


void FigureViewport::setFigure(boost::shared_ptr<const mesh::Figure> figure)
{
   if (!m_figure3d || m_figure3d->figure().get() != figure.get())
   {
      makeCurrent();
      m_figure3d.reset(figure ? new Figure3D(figure) : 0);
      updateGL();
   }
}


void FigureViewport::paintGL()
{
   Viewport::paintGL();
   if (m_figure3d)
   {
      m_figure3d->paintGL(m_scene.get());
   }
}
