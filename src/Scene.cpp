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


#include "utils3d/projection.hpp"


#include "Scene.hpp"


Scene::Scene()
   : m_projectionMatrix(
      utils3d::perspectiveProjectionMatrix(30.0f, 1, 1, 1.0f, 200.0f)
   ),
   m_modelViewMatrix(utils3d::translationMatrix(0.0f, 0.0f, 0.0f))
{
}


void Scene::setProjectionMatrix(const dt::Matrixf &m)
{
   m_projectionMatrix = m;
}


dt::Matrixf Scene::projectionMatrix() const
{
   return m_projectionMatrix;
}


void Scene::setModelViewMatrix(const dt::Matrixf &m)
{
   m_modelViewMatrix = m;
}


dt::Matrixf Scene::modelViewMatrix() const
{
   return m_modelViewMatrix;
}


dt::Matrixf Scene::modelViewProjectionMatrix() const
{
   return boost::numeric::ublas::prod(m_projectionMatrix, m_modelViewMatrix);
}
