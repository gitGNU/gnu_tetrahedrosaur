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
in vec4 in_velocity;
out vec4 out_position;
out vec4 out_normal;
out vec4 out_velocity;
uniform bool applyForces;
uniform isamplerBuffer staticVertexSampler;
uniform samplerBuffer edgeSampler;
uniform isamplerBuffer connectionSampler;
uniform samplerBuffer dynamicVertexSampler;

vec3 normal = vec3(0.0, 0.0, 0.0);
vec3 force = vec3(0.0, 0.0, 0.0);

vec3 safeNormalize(vec3 v, bool forceUnitLength)
{
   float len = v.length();
   if (len > 0.0)
   {
      return v / len;
   }
   return vec3(forceUnitLength ? 1.0 : 0.0, 0.0, 0.0);
}

void addNormal(int vertexID1, int vertexID2, int edgeID2, bool adjustNormal)
{
   vec3 pos0 = in_position.xyz;
   vec4 connectedVertex = texelFetch(dynamicVertexSampler, 3 * vertexID2);
   vec3 v2 = connectedVertex.xyz - pos0;
   float equilibriumLength = texelFetch(edgeSampler, edgeID2).x;
   float dl = (equilibriumLength - distance(pos0, connectedVertex.xyz));
   force += (safeNormalize(-v2, false) * dl);
   if (adjustNormal)
   {
      vec3 v1 = texelFetch(dynamicVertexSampler, 3 * vertexID1).xyz - pos0;
      normal += safeNormalize(cross(v1, v2), false);
   }
}

void getNormalsSum(int connection)
{
   if (connection < 0) return;

   bool adjustNormal = true;
   ivec4 first = texelFetch(connectionSampler, connection);
   ivec4 prev = first;
   int lastExternal = first.x;

   for (int i = 0; prev.w >= 0 && i < 65535; i ++)
   {
      ivec4 curr = texelFetch(connectionSampler, prev.w);
      if (curr.z > 0)
      {
         adjustNormal = false;
      }

      addNormal(prev.x, curr.x, curr.y, adjustNormal);
      prev = curr;

      if (adjustNormal)
      {
         lastExternal = curr.x;
      }
   }

   if (lastExternal != first.x)
   {
      addNormal(lastExternal, first.x, first.y, true);
   }
}

void main()
{
   getNormalsSum(texelFetch(staticVertexSampler, gl_VertexID).x);
   if (applyForces)
   {
      force -= 0.25 * in_velocity.xyz;
      float dt = 0.01;
      vec3 dr = in_velocity.xyz * dt;
      vec3 dv = (force / in_velocity.w) * dt;
      out_position = vec4(in_position.xyz + dr, in_position.w);
      out_normal = vec4(safeNormalize(normal, true), in_normal.w);
      out_velocity = vec4(in_velocity.xyz + dv, in_velocity.w);
   }
   else
   {
      out_position = in_position;
      out_normal = vec4(safeNormalize(normal, true), in_normal.w);
      out_velocity = in_velocity;
   }
}
