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


#ifndef SHADERS_H
#define SHADERS_H


#include <GL/gl.h>


namespace shader {


struct InteriorShaderDesc;
struct MainShaderDesc;
struct PickingShaderDesc;
struct PointShaderDesc;
struct ToolShaderDesc;
struct NormalShaderDesc;
struct FeedbackShaderDesc;


/***************************************************************************
 *   Collection class declaration                                          *
 ***************************************************************************/


class Collection
{
   public:
      explicit Collection(
         GLuint positionAttribIndex,
         GLuint normalAttribIndex,
         GLuint velocityAttribIndex
      );
      virtual ~Collection();

      bool initialize();

      inline const InteriorShaderDesc * interiorShader() const;
      inline const MainShaderDesc * mainShader() const;
      inline const PickingShaderDesc * pickingShader() const;
      inline const PointShaderDesc * pointShader() const;
      inline const ToolShaderDesc * toolShader() const;
      inline const NormalShaderDesc * normalShader() const;
      inline const FeedbackShaderDesc * feedbackShader() const;

   private:
      bool initializeInteriorShaderProgram();
      bool initializeMainShaderProgram();
      bool initializePickingShaderProgram();
      bool initializePointShaderProgram();
      bool initializeToolShaderProgram();
      bool initializeNormalShaderProgram();
      bool initializeFeedbackShaderProgram();

      GLuint m_positionAttribIndex;
      GLuint m_normalAttribIndex;
      GLuint m_velocityAttribIndex;

      GLuint m_interiorVertexShader;
      GLuint m_interiorGeometryShader;
      InteriorShaderDesc * m_interiorDesc;

      GLuint m_mainVertexShader;
      MainShaderDesc * m_mainDesc;

      GLuint m_pickingVertexShader;
      GLuint m_pickingFragmentShader;
      PickingShaderDesc * m_pickingDesc;

      GLuint m_pointVertexShader;
      PointShaderDesc * m_pointDesc;

      GLuint m_toolVertexShader;
      ToolShaderDesc * m_toolDesc;

      GLuint m_normalVertexShader;
      GLuint m_normalGeometryShader;
      NormalShaderDesc * m_normalDesc;

      GLuint m_feedbackVertexShader;
      FeedbackShaderDesc * m_feedbackDesc;
};


inline const InteriorShaderDesc * Collection::interiorShader() const
{
   return m_interiorDesc;
}


inline const MainShaderDesc * Collection::mainShader() const
{
   return m_mainDesc;
}


inline const PickingShaderDesc * Collection::pickingShader() const
{
   return m_pickingDesc;
}


inline const PointShaderDesc * Collection::pointShader() const
{
   return m_pointDesc;
}


inline const ToolShaderDesc * Collection::toolShader() const
{
   return m_toolDesc;
}


inline const NormalShaderDesc * Collection::normalShader() const
{
   return m_normalDesc;
}


inline const FeedbackShaderDesc * Collection::feedbackShader() const
{
   return m_feedbackDesc;
}


}


#endif
