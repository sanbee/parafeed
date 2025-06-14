# -*- cmake -*-
# /*
#  * Copyright (c) 2000-2021, 2022 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
#  *
#  * This program is free software; you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation; either version 2 of the License, or
#  * (at your option) any later version.
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program; if not, write to the Free Software
#  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#  *
#  */
#
# create a single, composite library
#
add_library(parafeed)
set_target_properties(parafeed PROPERTIES
  VERSION ${MAJOR_VERSION}.0.0
  SOVERSION ${MAJOR_VERSION}
  EXPORT_NAME parafeed
  OUTPUT_NAME parafeed)
message("subdir: PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
message("subdir: PROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}")
message("subdir: CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message("subdir: CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")

# link to readline
if (GNUREADLINE)
#target_link_libraries(parafeed_parafeed
#  PUBLIC PkgConfig::readline)
  target_link_libraries(parafeed PUBLIC ${READLINE_LIB})
  # compile definitions (like -DFOO)
  target_compile_definitions(parafeed PUBLIC HAVE_STRING_H GNUREADLINE)
else()
  # compile definitions (like -DFOO)
  target_compile_definitions(parafeed PUBLIC HAVE_STRING_H)
endif()
# define include directories, at build time and for clients
target_include_directories(parafeed PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
# install target
set(header_list
  boolError.h
  clbool.h
  clconvert.h
  clError.h
  clgetBaseCode.h
  clgetenv.h
  clGlobals.h
  cl.h
  clhashdefines.h
  clinteract.h
  cllib.h
  clshelldefs.h
  clsh.h
  clstring.h
  clparseVal.h
  ErrorObj.h
  rl_interface.h
  setAutoDefaults.h
  shell.h
  support.h
  iosupport.h
  SymbTab.h
  clparseVal.h
  clgetValp.h
  clgetValp.cc
)
#set_target_properties(parafeed_parafeed PROPERTIES PUBLIC_HEADER ${header_list})

install(TARGETS parafeed
  EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR})

foreach(s ${header_list})
  install(FILES ${s} DESTINATION  ${CMAKE_INSTALL_INCLUDEDIR})
endforeach()
# target alias in parafeed namespace
add_library(parafeed::parafeed ALIAS parafeed)

#
# define parser and scanner targets
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
bison_target(ShellParser
  shell.y
  ${CMAKE_CURRENT_BINARY_DIR}/shell.tab.cc
  COMPILE_FLAGS "-p sh_ -o shell.tab.cc"
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/shell.tab.h)
#  set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/shell.tab.c PROPERTY LANGUAGE CXX))
flex_target(ShellScanner
  shell.l
  ${CMAKE_CURRENT_BINARY_DIR}/shell.cc
  COMPILE_FLAGS "-Ce -Psh_ -o shell.cc")
add_flex_bison_dependency(ShellScanner ShellParser)
target_sources(parafeed
  PRIVATE ${BISON_ShellParser_OUTPUTS} ${FLEX_ShellScanner_OUTPUTS})

# not sure why, but calc bison and flex stuff has to be in this
# directory when using target_sources()
bison_target(CalcParser
  calc/calc.y
  ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c
  COMPILE_FLAGS "-p calc_"
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/y.tab.h)
flex_target(CalcScanner
  calc/lex.l
  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c
  COMPILE_FLAGS "-Pcalc_")
add_flex_bison_dependency(CalcScanner CalcParser)
target_sources(parafeed
  PRIVATE ${BISON_CalcParser_OUTPUTS} ${FLEX_CalcScanner_OUTPUTS})

#
# sh library
#

# we need to tell CMake that the sources are actually C++
set(sh_sources
  CopySymb.cc
  AddCmd.cc
  AddNode.cc
  AddVar.cc
  AllocSymb.cc
  AddQKey.cc
  DeleteCmd.cc
  DeleteVar.cc
  DeleteNode.cc
  DeleteTab.cc
  FreeSymb.cc
  SearchNode.cc
  SearchSymb.cc
  TransferTab.cc
  PrintVals.cc
  SetVar.cc
  SetPrompt.cc
  SetSymbTab.cc
  SetCmdTab.cc
  yyerror.cc
  minmatch.cc
  isBool.cc)
foreach(source IN LISTS sh_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()
target_sources(parafeed PRIVATE ${sh_sources})

# Commands to process callbacks.cc file and write out the
# callbacks_awk.cc file to compile.  The input source file uses the
# "awk" command and the code below is to set the actual command for
# running "awk".  Ultimately, once the need to use awk goes away in
# the source code, callbacks.cc will be directly compiled. Till then,
# the following is required.
add_custom_command(
   OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/callbacks_awk.cc
   COMMAND sed -e "s/__REPLACE_ME_WITH_AWK_PROG_NAME__/awk/" ${CMAKE_CURRENT_SOURCE_DIR}/callbacks.cc > ${CMAKE_CURRENT_SOURCE_DIR}/callbacks_awk.cc
   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/callbacks.cc
   COMMENT "Preprocessing callbacks.cc"
   VERBATIM)

#
# shccl library
#

# we need to tell CMake that the sources are actually C++
set(shccl_sources
  ParseCmdLine.cc
  clparseVal.cc
  InstallSymb.cc
  clgetSVal.cc
  clgetNSVal.cc
  clgetFullVal.cc

  clgetValp.cc
  clStartInteractive.cc
  clRestartShell.cc
  clfInteractive.cc
  clgetNVals.cc
  clstring.cc
  clstrtok.cc
  clgetOptsList.cc
  clclearOptsList.cc
  clgetCommandLine.cc
  clgetOpt.cc
  clstrtStream.cc
  clloadConfig.cc
  callbacks_awk.cc
  support.cc
  clgetInputFile.cc
  clgetOutputFile.cc
  clgetNOpts.cc
  clCleanUp.cc
  clReset.cc
  clgetConfigFile.cc
  clSaneExit.cc
  clThrowError.cc
  clRetry.cc
  clSigHandler.cc
  clIsInteractive.cc
  ErrorObj.cc
  rl_interface.cc
  clSetOptions.cc
  clTextColouring.cc
  clBreakStr.cc
  GetStdString.cc)
foreach(source IN LISTS shccl_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()
target_sources(parafeed PRIVATE ${shccl_sources})

# add calc sub-directory
add_subdirectory(calc)

# define the parafeed_libraries export
install(EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed
  NAMESPACE parafeed::
  FILE parafeed_libraries.cmake)

add_executable(tstcpp tstcpp.cc)
target_link_libraries(tstcpp parafeed)
add_executable(tst2 test/tst2.cc)
target_link_libraries(tst2 parafeed)
add_executable(tst3 test/tst3.cc)
target_link_libraries(tst3 parafeed)

# install all parafeed exports
install(FILES parafeedConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed)
