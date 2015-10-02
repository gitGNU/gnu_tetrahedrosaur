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


macro(generate_shader_ifndef_name GENERATED_NAME FILE_NAME)
   get_filename_component(_SHADER_NAME ${FILE_NAME} NAME)
   string(REPLACE "." "_" _SHADER_NAME ${_SHADER_NAME})
   string(TOUPPER ${_SHADER_NAME} _SHADER_NAME)
   set(${GENERATED_NAME} ${_SHADER_NAME})
endmacro(generate_shader_ifndef_name)


macro(generate_shader_define GENERATED_DEFINE FILE_NAME)
   get_filename_component(_SHADER_NAME ${FILE_NAME} NAME_WE)
   string(TOUPPER ${_SHADER_NAME} _SHADER_NAME)
   set(${GENERATED_DEFINE} ${_SHADER_NAME})
endmacro(generate_shader_define)


if(DEFINED INPUT)
   if(DEFINED OUTPUT)
      set(_IGNORE_FIRST_LINES 0)
      if(DEFINED IGNORE_FIRST_LINES)
         set(_IGNORE_FIRST_LINES ${IGNORE_FIRST_LINES})
      endif()
      generate_shader_ifndef_name(IFNDEF_NAME ${OUTPUT})
      generate_shader_define(SHADER_DEFINE ${OUTPUT})

      file(STRINGS "${INPUT}" SOURCE)
      file(WRITE "${OUTPUT}" "#ifndef ${IFNDEF_NAME}\n#define ${IFNDEF_NAME}\n\n\n#define ${SHADER_DEFINE} \"\\\n")
      set(_LINE_NUMBER 0)
      foreach(SOURCE_LINE ${SOURCE})
         math(EXPR _LINE_NUMBER "${_LINE_NUMBER}+1")
         if(_LINE_NUMBER GREATER _IGNORE_FIRST_LINES)
            string(REPLACE "\"" "\\\"" SOURCE_ESC_LINE "${SOURCE_LINE}")
            file(APPEND "${OUTPUT}" "${SOURCE_ESC_LINE}\\n\\\n")
            unset(SOURCE_ESC_LINE)
         endif()
      endforeach(SOURCE_LINE)
      file(APPEND "${OUTPUT}" "\"\n\n\n#endif\n")
      unset(_LINE_NUMBER)
      unset(_IGNORE_FIRST_LINES)
   else()
      message(FATAL_ERROR "GenerateShader: no output defined")
   endif()
else()
   message(FATAL_ERROR "GenerateShader: no input defined")
endif()
