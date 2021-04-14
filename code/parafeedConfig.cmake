cmake_minimum_required(VERSION 3.18)

# FIXME
find_package(GSL)
find_package(LAPACK)
find_package(Boost 1.41.0 REQUIRED
  COMPONENTS program_options system)
find_package(SQLite3)
find_package(LibXml2)
find_package(LibXslt)
find_package(PkgConfig)
pkg_search_module(wcslib
  IMPORTED_TARGET wcslib)
pkg_search_module(cfitsio
  IMPORTED_TARGET cfitsio)

# PkgConfig::wcslib provides wcslib sub-directory for header files,
# while CASA (and probably others) prefers to keep the wcslib
# directory prefix. Fix it here
get_target_property(wcslib_include_dirs
  PkgConfig::wcslib INTERFACE_INCLUDE_DIRECTORIES)
get_filename_component(wcslib_include_dirs "${wcslib_include_dirs}" PATH)
set_target_properties(
  PkgConfig::wcslib
  PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${wcslib_include_dirs}")

# parafeed libraries configurations
include(${CMAKE_CURRENT_LIST_DIR}/parafeed_libraries.cmake)
