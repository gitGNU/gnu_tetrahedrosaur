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
find_package(OpenGL REQUIRED GLU)
find_package(GLEW)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(HEADERS
   Application.hpp
   Arcball.hpp
   CellModel.hpp
   custom_enums.hpp
   Figure3D.hpp
   GeneModel.hpp
   GenomeModel.hpp
   GenomeTab.hpp
   GuiOrganismDesc.hpp
   MainWindowTab.hpp
   MutationParamsDialog.hpp
   Organism3D.hpp
   OrganismPixelBuffer.hpp
   ProbabilitySpinBox.hpp
   PropertyModel.hpp
   PropertyView.hpp
   Scene.hpp
   SharedGLWidget.hpp
   tools3D.hpp
   translation.hpp
   TriangleModel.hpp
   VertexModel.hpp
   Viewport.hpp
)

set(MOC_HEADERS
   ChromosomeDiffModel.hpp
   DevelopmentEngine.hpp
   EdgeModel.hpp
   FigureViewport.hpp
   GenomeView.hpp
   MainWindow.hpp
   MainWindowMenuBar.hpp
   MatingWidget.hpp
   NewProjectDialog.hpp
   OffspringModel.hpp
   OrganismMenu.hpp
   OrganismTab.hpp
   OrganismViewport.hpp
   PopulationModel.hpp
   PopulationView.hpp
   Project.hpp
   ProjectLoadingDialog.hpp
   SelectionTab.hpp
)

set(SOURCES
   Application.cpp
   Arcball.cpp
   CellModel.cpp
   ChromosomeDiffModel.cpp
   DevelopmentEngine.cpp
   EdgeModel.cpp
   Figure3D.cpp
   FigureViewport.cpp
   GeneModel.cpp
   GenomeModel.cpp
   GenomeTab.cpp
   GenomeView.cpp
   GuiOrganismDesc.cpp
   MainWindow.cpp
   MainWindowMenuBar.cpp
   MainWindowTab.cpp
   MatingWidget.cpp
   MutationParamsDialog.cpp
   NewProjectDialog.cpp
   OffspringModel.cpp
   Organism3D.cpp
   OrganismMenu.cpp
   OrganismPixelBuffer.cpp
   OrganismTab.cpp
   OrganismViewport.cpp
   PopulationModel.cpp
   PopulationView.cpp
   ProbabilitySpinBox.cpp
   Project.cpp
   ProjectLoadingDialog.cpp
   PropertyModel.cpp
   PropertyView.cpp
   Scene.cpp
   SelectionTab.cpp
   SharedGLWidget.cpp
   tetrahedrosaur.cpp
   tools3D.cpp
   translation.cpp
   TriangleModel.cpp
   VertexModel.cpp
   Viewport.cpp
)

set (RESOURCES
   ../res/resources.qrc
)

include_directories(${Boost_INCLUDE_DIR})
include_directories(../lib/algo/include)
include_directories(../lib/bio/include)
include_directories(../lib/datatypes/include)
include_directories(../lib/mesh/include)
include_directories(../lib/shader/include)
include_directories(../lib/utils3d/include)

qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
qt5_add_resources(QRC_SOURCES ${RESOURCES})

add_executable(${PROJECT_NAME}
   ${HEADERS}
   ${MOC_SOURCES}
   ${QRC_SOURCES}
   ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
   bio
   mesh
   shader
   utils3d
   algo
   datatypes
   ${OPENGL_gl_LIBRARY}
   ${OPENGL_glu_LIBRARY}
   ${GLEW_LIBRARY}
   ${Qt5OpenGL_LIBRARIES}
   ${Qt5Widgets_LIBRARIES}
)
