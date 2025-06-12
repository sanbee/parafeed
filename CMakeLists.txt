cmake_minimum_required(VERSION 3.17)
set(MAJOR_VERSION 1)
# build dependencies
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# link dependencies
find_package(PkgConfig REQUIRED)

# C++17 is required for this project.
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(CMAKE_CXX_STANDARD_REQUIRED "parafeed switch CMAKE_CXX_STANDARD_REQUIRED set to OFF" OFF)

# # Set the default for using the GNU readline and histroy package
# if(NOT DEFINED GNUREADLINE)
#   set(GNUREADLINE ON)
# endif()
option(GNUREADLINE "parafeed switch GNUREADLINE set to ON" ON)

project(
  parafeed
  VERSION ${MAJOR_VERSION}.0.0
  LANGUAGES CXX C)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# Set a default build type if none was specified
set(default_build_type Release)
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type Debug)
endif()

# find_package() will load CMAKE_MODULE_PATH/FindReadline.cmake if 
# it can't find Readline.  FindReadline.cmake will be looked for
# in CMAKE_MODULE_PATH.  FindReadline.cmake is sensitive to 
# CMAKE_PREFIX_PATH setting.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (GNUREADLINE)
  find_package(PkgConfig)
  find_package(Readline QUIET)
endif()

if(READLINE_FOUND)
  message(STATUS "Found Readline via find_package()")
  include_directories(${READLINE_INCLUDE_DIRS})
#  link_directories(${READLINE_LIBRARY_DIRS})
  link_libraries(${READLINE_LIBRARIES})
else()
    if (GNUREADLINE)
      message(WARNING "EEEeeeeks! GNU Readline package could not by found.  Setting GNUREADLINE=OFF")
    endif()
    set(GNUREADLINE OFF)
endif()

# if (GNUREADLINE)
# 	message("parafeed: Using GNU readline library")
# 	pkg_search_module(readline REQUIRED IMPORTED_TARGET readline)
# 	# get_target_property(_readline_interface_include_dirs
# 	#   PkgConfig::readline INTERFACE_INCLUDE_DIRECTORIES)
# 	add_library(readline SHARED IMPORTED)
# 	#set_target_properties(readline PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libreadline.so)
# 	set_target_properties(readline PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/include)
# endif()

# Set the default for using the GNU readline and histroy package

if (GNUREADLINE)
  find_package(PkgConfig)
  pkg_check_modules(READLINE readline)
endif()

if(READLINE_FOUND)
  message(STATUS "Found Readline via pkg-config")
else()
  #
  # Search for the path to dir called "readline" using find_path.  If
  # found, include this via include_directories() below.  This ends up
  # as -I<PATH> in the generated files for make.
  find_path(READLINE_INCLUDE_DIRS readline
    HINTS ${READLINE_ROOT_DIR} PATH_SUFFIXES usr usr/include)

  # Search for the dir where library "readline" may be found.  The name
  # is translated to platform-specific library file name.  If found, add
  # that path via link_directories() below.  This ends up as -L<PATH> in
  # the generated files for make.
  find_library(READLINE_LIB readline)
  #  HINTS ${READLINE_ROOT_DIR} usr PATH_SUFFIXES lib lib64)

  if (GNUREADLINE
      AND NOT READLINE_INCLUDE_DIR STREQUAL READLINE_INCLUDE_DIR-NOTFOUND
      AND NOT READLINE_LIB_DIR STREQUAL READLINE_LIB_DIR-NOTFOUND)

    get_filename_component(READLINE_LIBRARY_DIRS ${READLINE_LIB} DIRECTORY)

    message(STATUS "parafeed: READLINE_INCLUDE_DIRS=" ${READLINE_INCLUDE_DIRS})
    message(STATUS "parafeed: READLINE_LIBRARY_DIRS=" ${READLINE_LIBRARY_DIRS})
    set(READLINE_FOUND ON)
    # include_directories (${READLINE_INCLUDE_DIR})
    # link_directories(${READLINE_LIB_DIR})
  else()
    if (GNUREADLINE)
      message(WARNING "EEEeeeeks! GNU Readline package could not by found.  Setting GNUREADLINE=OFF")
    endif()
    set(GNUREADLINE OFF)
  endif()
endif()

if (READLINE_FOUND)
  include_directories(${READLINE_INCLUDE_DIRS})
  link_directories(${READLINE_LIBRARY_DIRS})
  link_libraries(${READLINE_LIBRARIES})
endif()
# if (GNUREADLINE)
# 	message("parafeed: Using GNU readline library")
# 	pkg_search_module(readline REQUIRED IMPORTED_TARGET readline)
# 	# get_target_property(_readline_interface_include_dirs
# 	#   PkgConfig::readline INTERFACE_INCLUDE_DIRECTORIES)
# 	add_library(readline SHARED IMPORTED)
# 	#set_target_properties(readline PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libreadline.so)
# 	set_target_properties(readline PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/include)
# endif()

# Set the default for using the GNU readline and histroy package

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


# Allow setting the ABI on commandline.  If not set on commandline,
# use complier's default ABI.
if (DEFINED _GLIBCXX_USE_CXX11_ABI)
  list(APPEND CMAKE_MESSAGE_INDENT "     ")
  message("Building with -D_GLIBCXX_USE_CXX11_ABI=${_GLIBCXX_USE_CXX11_ABI}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=${_GLIBCXX_USE_CXX11_ABI}")
else()
  list(APPEND CMAKE_MESSAGE_INDENT "     ")
  message("Building with the complier default ABI setting")
endif()


# work around a problem in which the readline.pc file declares
# "Requires.private: termcap", which prevents setting the
# INTERFACE_INCLUDE_DIRECTORIES property of PkgConfig::readline
#if(NOT _readline_interface_include_dirs)
#  # search for the include directory based on the library location
#  get_target_property(_readline_interface_link_libs
#    PkgConfig::readline INTERFACE_LINK_LIBRARIES)
#  get_filename_component(_readline_interface_link_libs
#    "${_readline_interface_link_libs}" DIRECTORY)
#  get_filename_component(_readline_interface_link_libs
#    "${_readline_interface_link_libs}" DIRECTORY)
#  set(_readline_interface_include_dirs
#    "${_readline_interface_link_libs}/include")
#  # basically now library_path/../../include, check for
#  # readline/readline.h in this directory
#  find_file(_has_readline_h
#    readline.h
#    HINTS "${_readline_interface_include_dirs}/readline")
#  if(_has_readline_h)
#    # found it, set the property
#    message(STATUS
#      "Applying workaround for readline.pc private termcap dependence")
#    set_target_properties(PkgConfig::readline
#      PROPERTIES
#      INTERFACE_INCLUDE_DIRECTORIES "${_readline_interface_include_dirs}")
#  endif()
#endif()

# code sub-directory
add_subdirectory(code)
message("top: CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("top: CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")

# Enable testing support
option(ENABLE_TESTING "Enable unit testing" OFF)
if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(code/test/unittest)
endif()

# Add subdirectories

# install all parafeed exports
include(CMakePackageConfigHelpers)
write_basic_package_version_file(parafeedConfigVersion.cmake
  VERSION 0
  COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/parafeedConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed)
# Generate the parafeed.pc file
configure_file(parafeed.pc.in ${CMAKE_INSTALL_LIBDIR}/pkgconfig/parafeed.pc @ONLY)
