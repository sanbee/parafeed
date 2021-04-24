cmake_minimum_required(VERSION 3.18)
find_package(PkgConfig REQUIRED)
pkg_search_module(readline REQUIRED readline)

# parafeed libraries configurations
include(${CMAKE_CURRENT_LIST_DIR}/parafeed_libraries.cmake)
