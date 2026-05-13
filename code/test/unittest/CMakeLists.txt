# Copyright (c) 2026 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
project(ParafeedTests VERSION 1.0
          DESCRIPTION "Parafeed tests"
          LANGUAGES CXX)



option(ENABLE_TESTING "Enable unit testing" OFF)
if(ENABLE_TESTING)
  enable_testing()
endif()

find_package(GTest QUIET)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.12.1
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(googletest)
endif()

link_directories("${CMAKE_SOURCE_DIR}/build/code/")

# List of test source files
set(TEST_SOURCES
  test_parafeed_noninteractive.cpp
)

add_executable(ParafeedTests ${TEST_SOURCES})

# Link with Google Test
target_include_directories(ParafeedTests PRIVATE
	"${CMAKE_SOURCE_DIR}/code/test") 
target_link_libraries(ParafeedTests GTest::gtest_main parafeed)


set(TEST_SOURCES  test_parafeed_interactive.cpp)
add_executable(iParafeedTests ${TEST_SOURCES})
target_include_directories(iParafeedTests PRIVATE "${CMAKE_SOURCE_DIR}/code/test")
# Link with Google Test
target_link_libraries(iParafeedTests GTest::gtest_main parafeed)

# Add the test executable as a test target
add_test(NAME ParafeedTests COMMAND ParafeedTests)
add_test(NAME iParafeedTests COMMAND iParafeedTests)

