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


set(GENERATE_SHADERS_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})
macro(generate_shaders GENERATED_SHADERS)
   foreach(_SHADER ${ARGN})
      string(REPLACE ".." "__" _SHADER_FULL_NAME ${_SHADER})
      get_filename_component(_SHADER_PATH ${_SHADER_FULL_NAME} PATH)
      get_filename_component(_SHADER_NAME ${_SHADER_FULL_NAME} NAME)
      string(REPLACE "." "_" _SHADER_NAME ${_SHADER_NAME})
      file(MAKE_DIRECTORY ${_SHADER_PATH})

      set(_INPUT_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/${_SHADER})
      set(_OUTPUT_SHADER ${CMAKE_CURRENT_BINARY_DIR}/${_SHADER_PATH}/${_SHADER_NAME}.h)

      add_custom_command(
         OUTPUT ${_OUTPUT_SHADER}
         COMMAND ${CMAKE_COMMAND} -DINPUT=${_INPUT_SHADER} -DOUTPUT=${_OUTPUT_SHADER} -DIGNORE_FIRST_LINES=18 -P "${GENERATE_SHADERS_CURRENT_LIST_DIR}/GenerateShader.cmake"
         DEPENDS ${_INPUT_SHADER}
      )

      set(_GENERATED_SHADERS ${_GENERATED_SHADERS} ${_OUTPUT_SHADER})
   endforeach(_SHADER)
   set(${GENERATED_SHADERS} ${_GENERATED_SHADERS})
endmacro(generate_shaders)
