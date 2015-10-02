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
   AverageGeneParams.hpp
   Cell.hpp
   Chromosome.hpp
   Config.hpp
   crossingover.hpp
   Gene.hpp
   GeneInitializer.hpp
   Genome.hpp
   InitialConditions.hpp
   InstructionSet.hpp
   mating.hpp
   Organism.hpp
   OrganismDesc.hpp
   RangeCondition.hpp
   TetrahedronsMap.hpp
)

set(SOURCES
   AverageGeneParams.cpp
   Cell.cpp
   Chromosome.cpp
   Config.cpp
   Gene.cpp
   GeneInitializer.cpp
   Genome.cpp
   InitialConditions.cpp
   InstructionSet.cpp
   mating.cpp
   Organism.cpp
   OrganismDesc.cpp
   TetrahedronsMap.cpp
)

include_directories(../../algo/include)
include_directories(../../datatypes/include)
include_directories(../../mesh/include)

add_library(bio STATIC ${HEADERS} ${SOURCES})
