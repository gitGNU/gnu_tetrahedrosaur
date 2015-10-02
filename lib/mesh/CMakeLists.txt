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


find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)

set(TEST_SUITS
   libmesh_BuddingHelper
   libmesh_ColorWrappedLists
   libmesh_Connections
   libmesh_MemoryModification
   libmesh_Mesh
   libmesh_Triangle
   libmesh_TrianglesMap
)

enable_testing()
foreach(TEST_SUIT ${TEST_SUITS})
   add_test("test_${TEST_SUIT}" test/test_libmesh "--run_test=suite_${TEST_SUIT}")
endforeach()
