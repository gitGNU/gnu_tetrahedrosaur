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


#version 150

in vec4 in_position;
in vec4 in_normal;
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform vec3 lightSourcePos;


vec4 diffuseLighting(vec4 pos, vec3 norm)
{
   vec3 vertex_in_modelview_space = (MVMatrix * pos).xyz;
   vec3 normalized_normal = normalize(mat3(MVMatrix) * norm);
   vec3 normalized_vertex_to_light_vector = normalize(lightSourcePos - vertex_in_modelview_space);
   float DiffuseTerm = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);
   vec4 resultColor = vec4(1.0, 1.0, 1.0, 1.0) * DiffuseTerm;
   resultColor[3] = 1.0;
   return resultColor;
}

void main()
{
   vec4 position = vec4(in_position.xyz, 1.0);
   gl_Position = MVPMatrix * position;
   gl_FrontColor = diffuseLighting(position, in_normal.xyz);
}
