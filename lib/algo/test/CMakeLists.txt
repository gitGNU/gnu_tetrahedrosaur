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


find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(SOURCES
   test_Alignment.cpp
   test_distance.cpp
   test_libalgo.cpp
   test_pairing.cpp
   test_roommates.cpp
)

include_directories(../src)
add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(test_libalgo ${SOURCES})
target_link_libraries(test_libalgo ${Boost_LIBRARIES} algo)
