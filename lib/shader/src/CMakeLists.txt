#   Copyright (C) 2015 Andrey Timashov
#
#   This file is part of Tetrahedrosaur.
#
#   Tetrahedrosaur is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   Tetrahedrosaur is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#   General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with Tetrahedrosaur. If not, see <http://www.gnu.org/licenses/>


set(HEADERS
   descriptors.hpp
   Collection.hpp
)

set(SOURCES
   descriptors.cpp
   Collection.cpp
)

set(SHADERS
   glsl/feedback.glslv
   glsl/interior.glslg
   glsl/interior.glslv
   glsl/main.glslv
   glsl/normal.glslg
   glsl/normal.glslv
   glsl/picking.glslf
   glsl/picking.glslv
   glsl/point.glslv
   glsl/tool.glslv
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(GenerateShaders)
generate_shaders(SHADER_HEADERS ${SHADERS})

add_library(shader STATIC ${SHADER_HEADERS} ${HEADERS} ${SOURCES})
