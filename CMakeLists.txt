cmake_minimum_required(VERSION 3.17)
project(
  parafeed
  VERSION 0.0.1
  LANGUAGES CXX C)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# Set a default build type if none was specified
set(default_build_type Release)
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type Debug)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS
    "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
    STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set a default install prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT
    AND NOT WIN32
    AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(CMAKE_INSTALL_PREFIX
    "/opt/nrao.edu/${PROJECT_NAME}" CACHE PATH "..." FORCE)
endif()

# initialize install RPATH
if(APPLE)
  set(base @loader_path)
else()
  set(base $ORIGIN)
endif()
file(RELATIVE_PATH relDir
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_INSTALL_RPATH ${base} ${base}/${relDir})

# c++11 is required
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build dependencies
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# link dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(readline REQUIRED
  IMPORTED_TARGET readline)

# code sub-directory
add_subdirectory(code)

# install all parafeed exports
include(CMakePackageConfigHelpers)
write_basic_package_version_file(parafeedConfigVersion.cmake
  VERSION 0
  COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/parafeedConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed)
