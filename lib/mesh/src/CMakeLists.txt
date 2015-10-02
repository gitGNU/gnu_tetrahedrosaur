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
   BuddingHelper.hpp
   BuddingParams.hpp
   ColorWrappedLists.hpp
   Connections.hpp
   Edge.hpp
   Figure.hpp
   GLMesh.hpp
   MemoryModification.hpp
   Mesh.hpp
   SelectionHelper.hpp
   StructureModification.hpp
   Tetrahedron.hpp
   Triangle.hpp
   TrianglesMap.hpp
   Vertex.hpp
)

set(SOURCES
   BuddingHelper.cpp
   BuddingParams.cpp
   ColorWrappedLists.cpp
   Connections.cpp
   Edge.cpp
   Figure.cpp
   GLMesh.cpp
   MemoryModification.cpp
   Mesh.cpp
   SelectionHelper.cpp
   StructureModification.cpp
   Tetrahedron.cpp
   Triangle.cpp
   TrianglesMap.cpp
   Vertex.cpp
)

include_directories(../../datatypes/include)
include_directories(../../shader/include)
include_directories(../../utils3d/include)

add_library(mesh STATIC ${HEADERS} ${SOURCES})
