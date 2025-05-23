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
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
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
	"${CMAKE_SOURCE_DIR}/code") 
target_link_libraries(ParafeedTests GTest::gtest_main parafeed)


# Add the test executable as a test target
add_test(NAME ParafeedTests COMMAND ParafeedTests)


