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


#include <cassert>
#include <cstdio>
#include <GL/glew.h>


#include "Collection.hpp"
#include "descriptors.hpp"
#include "glsl/feedback_glslv.h"
#include "glsl/interior_glslv.h"
#include "glsl/interior_glslg.h"
#include "glsl/main_glslv.h"
#include "glsl/normal_glslv.h"
#include "glsl/normal_glslg.h"
#include "glsl/picking_glslf.h"
#include "glsl/picking_glslv.h"
#include "glsl/point_glslv.h"
#include "glsl/tool_glslv.h"


namespace shader {


/***************************************************************************
 *   Collection class implementation                                       *
 ***************************************************************************/


Collection::Collection(
   GLuint positionAttribIndex,
   GLuint normalAttribIndex,
   GLuint velocityAttribIndex
) : m_positionAttribIndex(positionAttribIndex),
   m_normalAttribIndex(normalAttribIndex),
   m_velocityAttribIndex(velocityAttribIndex),
   m_interiorVertexShader(0),
   m_interiorGeometryShader(0),
   m_interiorDesc(0),
   m_mainVertexShader(0),
   m_mainDesc(0),
   m_pickingVertexShader(0),
   m_pickingFragmentShader(0),
   m_pickingDesc(0),
   m_pointVertexShader(0),
   m_pointDesc(0),
   m_toolVertexShader(0),
   m_toolDesc(0),
   m_normalVertexShader(0),
   m_normalGeometryShader(0),
   m_normalDesc(0),
   m_feedbackVertexShader(0),
   m_feedbackDesc(0)
{
}


Collection::~Collection()
{
   if (m_feedbackDesc)
   {
      glDeleteProgram(m_feedbackDesc->program);
      delete m_feedbackDesc;
   }
   glDeleteShader(m_feedbackVertexShader);

   if (m_normalDesc)
   {
      glDeleteProgram(m_normalDesc->program);
      delete m_normalDesc;
   }
   glDeleteShader(m_normalGeometryShader);
   glDeleteShader(m_normalVertexShader);

   if (m_toolDesc)
   {
      glDeleteProgram(m_toolDesc->program);
      delete m_toolDesc;
   }
   glDeleteShader(m_toolVertexShader);

   if (m_pointDesc)
   {
      glDeleteProgram(m_pointDesc->program);
      delete m_pointDesc;
   }
   glDeleteShader(m_pointVertexShader);

   if (m_pickingDesc)
   {
      glDeleteProgram(m_pickingDesc->program);
      delete m_pickingDesc;
   }
   glDeleteShader(m_pickingFragmentShader);
   glDeleteShader(m_pickingVertexShader);

   if (m_mainDesc)
   {
      glDeleteProgram(m_mainDesc->program);
      delete m_mainDesc;
   }
   glDeleteShader(m_mainVertexShader);

   if (m_interiorDesc)
   {
      glDeleteProgram(m_interiorDesc->program);
      delete m_interiorDesc;
   }
   glDeleteShader(m_interiorGeometryShader);
   glDeleteShader(m_interiorVertexShader);
}


bool Collection::initialize()
{
   if (!initializeInteriorShaderProgram()) return false;
   if (!initializeMainShaderProgram()) return false;
   if (!initializePickingShaderProgram()) return false;
   if (!initializePointShaderProgram()) return false;
   if (!initializeToolShaderProgram()) return false;
   if (!initializeNormalShaderProgram()) return false;
   if (!initializeFeedbackShaderProgram()) return false;
   return true;
}


bool Collection::initializeInteriorShaderProgram()
{
   assert(!m_interiorVertexShader);
   assert(!m_interiorGeometryShader);
   assert(!m_interiorDesc);

   static const GLchar * vertexSource = INTERIOR_GLSLV;

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexSource, NULL);
   glCompileShader(vertexShader);

   GLint success = GL_FALSE;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(vertexShader, 1024, NULL, log);
      printf("Interior vertex shader compile error: %s\n", log);
      glDeleteShader(vertexShader);
      return false;
   }

   static const GLchar * geometrySource = INTERIOR_GLSLG;

   GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
   glShaderSource(geometryShader, 1, &geometrySource, NULL);
   glCompileShader(geometryShader);

   success = GL_FALSE;
   glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(geometryShader, 1024, NULL, log);
      printf("Interior geometry shader compile error: %s\n", log);
      glDeleteShader(geometryShader);
      glDeleteShader(vertexShader);
      return false;
   }

   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glAttachShader(program, geometryShader);
   glBindAttribLocation(program, m_positionAttribIndex, "in_position");
   glLinkProgram(program);
   success = GL_FALSE;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetProgramInfoLog(program, 1024, NULL, log);
      printf("Interior shader program link error: %s\n", log);
      glDeleteProgram(program);
      glDeleteShader(geometryShader);
      glDeleteShader(vertexShader);
      return false;
   }

   m_interiorVertexShader = vertexShader;
   m_interiorGeometryShader = geometryShader;
   m_interiorDesc = new InteriorShaderDesc(
      program,
      glGetUniformLocation(program, "MVPMatrix"),
      glGetUniformLocation(program, "dynamicVertexSampler"),
      glGetUniformLocation(program, "staticVertexSampler"),
      glGetUniformLocation(program, "connectionSampler")
   );
   return true;
}


bool Collection::initializeMainShaderProgram()
{
   assert(!m_mainVertexShader);
   assert(!m_mainDesc);

   static const GLchar * source = MAIN_GLSLV;

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &source, NULL);
   glCompileShader(vertexShader);

   GLint success = GL_FALSE;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(vertexShader, 1024, NULL, log);
      printf("Main vertex shader compile error: %s\n", log);
      glDeleteShader(vertexShader);
      return false;
   }

   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glBindAttribLocation(program, m_positionAttribIndex, "in_position");
   glBindAttribLocation(program, m_normalAttribIndex, "in_normal");
   glLinkProgram(program);
   success = GL_FALSE;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetProgramInfoLog(program, 1024, NULL, log);
      printf("Main shader program link error: %s\n", log);
      glDeleteProgram(program);
      glDeleteShader(vertexShader);
      return false;
   }

   m_mainVertexShader = vertexShader;
   m_mainDesc = new MainShaderDesc(
      program,
      glGetUniformLocation(program, "MVMatrix"),
      glGetUniformLocation(program, "MVPMatrix"),
      glGetUniformLocation(program, "lightSourcePos")
   );
   return true;
}


bool Collection::initializePickingShaderProgram()
{
   assert(!m_pickingVertexShader);
   assert(!m_pickingFragmentShader);
   assert(!m_pickingDesc);

   static const GLchar * vertexSource = PICKING_GLSLV;

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexSource, NULL);
   glCompileShader(vertexShader);

   GLint success = GL_FALSE;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(vertexShader, 1024, NULL, log);
      printf("Picking vertex shader compile error: %s\n", log);
      return false;
   }

   static const GLchar * fragmentSource = PICKING_GLSLF;

   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
   glCompileShader(fragmentShader);

   success = GL_FALSE;
   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(fragmentShader, 1024, NULL, log);
      printf("Picking fragment shader compile error: %s\n", log);
      return false;
   }

   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glAttachShader(program, fragmentShader);
   glBindAttribLocation(program, m_positionAttribIndex, "in_position");
   glBindFragDataLocation(
      program,
      0, // index of color buffers in glDrawBuffers();
      "out_pointColor"
   );
   glLinkProgram(program);
   success = GL_FALSE;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetProgramInfoLog(program, 1024, NULL, log);
      printf("Picking shader program link error: %s\n", log);
      return false;
   }

   m_pickingVertexShader = vertexShader;
   m_pickingFragmentShader = fragmentShader;
   m_pickingDesc = new PickingShaderDesc(
      program,
      glGetUniformLocation(program, "MVPMatrix")
   );
   return true;
}


bool Collection::initializePointShaderProgram()
{
   assert(!m_pointVertexShader);
   assert(!m_pointDesc);

   static const GLchar * source = POINT_GLSLV;

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &source, NULL);
   glCompileShader(vertexShader);

   GLint success = GL_FALSE;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(vertexShader, 1024, NULL, log);
      printf("Point vertex shader compile error: %s\n", log);
      return false;
   }

   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glBindAttribLocation(program, m_positionAttribIndex, "in_position");
   glLinkProgram(program);
   success = GL_FALSE;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetProgramInfoLog(program, 1024, NULL, log);
      printf("Point shader program link error: %s\n", log);
      return false;
   }

   m_pointVertexShader = vertexShader;
   m_pointDesc = new PointShaderDesc(
      program,
      glGetUniformLocation(program, "MVPMatrix"),
      glGetUniformLocation(program, "pointSize"),
      glGetUniformLocation(program, "pointColor"),
      glGetUniformLocation(program, "selectionColor")
   );
   return true;
}


bool Collection::initializeToolShaderProgram()
{
   assert(!m_toolVertexShader);
   assert(!m_toolDesc);

   static const GLchar * source = TOOL_GLSLV;

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &source, NULL);
   glCompileShader(vertexShader);

   GLint success = GL_FALSE;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(vertexShader, 1024, NULL, log);
      printf("Tool vertex shader compile error: %s\n", log);
      return false;
   }

   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glBindAttribLocation(program, m_positionAttribIndex, "in_position");
   glLinkProgram(program);
   success = GL_FALSE;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetProgramInfoLog(program, 1024, NULL, log);
      printf("Tool shader program link error: %s\n", log);
      return false;
   }

   m_toolVertexShader = vertexShader;
   m_toolDesc = new ToolShaderDesc(
      program,
      glGetUniformLocation(program, "MVPMatrix"),
      glGetUniformLocation(program, "edgeToolLength")
   );
   return true;
}


bool Collection::initializeNormalShaderProgram()
{
   assert(!m_normalVertexShader);
   assert(!m_normalGeometryShader);
   assert(!m_normalDesc);

   static const GLchar * vertexSource = NORMAL_GLSLV;

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexSource, NULL);
   glCompileShader(vertexShader);

   GLint success = GL_FALSE;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(vertexShader, 1024, NULL, log);
      printf("Normal vertex shader compile error: %s\n", log);
      return false;
   }

   static const GLchar * geometrySource = NORMAL_GLSLG;

   GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
   glShaderSource(geometryShader, 1, &geometrySource, NULL);
   glCompileShader(geometryShader);

   success = GL_FALSE;
   glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(geometryShader, 1024, NULL, log);
      printf("Normal geometry shader compile error: %s\n", log);
      return false;
   }

   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glAttachShader(program, geometryShader);
   glBindAttribLocation(program, m_positionAttribIndex, "in_position");
   glBindAttribLocation(program, m_normalAttribIndex, "in_normal");
   glLinkProgram(program);
   success = GL_FALSE;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetProgramInfoLog(program, 1024, NULL, log);
      printf("Normal shader program link error: %s\n", log);
      return false;
   }

   m_normalVertexShader = vertexShader;
   m_normalGeometryShader = geometryShader;
   m_normalDesc = new NormalShaderDesc(
      program,
      glGetUniformLocation(program, "MVPMatrix")
   );
   return true;
}


bool Collection::initializeFeedbackShaderProgram()
{
   assert(!m_feedbackVertexShader);
   assert(!m_feedbackDesc);

   static const GLchar * source = FEEDBACK_GLSLV;
   static const char * feedbackVaryings[] = {
      // The order within the buffer will be the same;
      "out_position", "out_normal", "out_velocity"
   };

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &source, NULL);
   glCompileShader(vertexShader);

   GLint success = GL_FALSE;
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetShaderInfoLog(vertexShader, 1024, NULL, log);
      printf("Feedback vertex shader compile error: %s\n", log);
      glDeleteShader(vertexShader);
      return false;
   }

   GLuint program = glCreateProgram();
   glAttachShader(program, vertexShader);
   glBindAttribLocation(program, m_positionAttribIndex, "in_position");
   glBindAttribLocation(program, m_normalAttribIndex, "in_normal");
   glBindAttribLocation(program, m_velocityAttribIndex, "in_velocity");
   glTransformFeedbackVaryings(
      program,
      sizeof(feedbackVaryings) / sizeof(const char *),
      feedbackVaryings,
      GL_INTERLEAVED_ATTRIBS
   );
   glLinkProgram(program);
   success = GL_FALSE;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   if (!success)
   {
      char log[2048] = "";
      glGetProgramInfoLog(program, 1024, NULL, log);
      printf("Feedback shader program link error: %s\n", log);
      glDeleteProgram(program);
      glDeleteShader(vertexShader);
      return false;
   }

   m_feedbackVertexShader = vertexShader;
   m_feedbackDesc = new FeedbackShaderDesc(
      program,
      glGetUniformLocation(program, "applyForces"),
      glGetUniformLocation(program, "dynamicVertexSampler"),
      glGetUniformLocation(program, "staticVertexSampler"),
      glGetUniformLocation(program, "edgeSampler"),
      glGetUniformLocation(program, "connectionSampler")
   );
   return true;
}


}
