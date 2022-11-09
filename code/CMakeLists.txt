# -*- cmake -*-
#
# create a single, composite library
#
add_library(parafeed_parafeed)
set_target_properties(parafeed_parafeed PROPERTIES
  VERSION 1.0.0
  SOVERSION 0
  EXPORT_NAME parafeed
  OUTPUT_NAME parafeed)
# link to readline
#target_link_libraries(parafeed_parafeed
#  PUBLIC PkgConfig::readline)
target_link_libraries(parafeed_parafeed
  PUBLIC libreadline.so)
# compile definitions (like -DFOO)
target_compile_definitions(parafeed_parafeed PUBLIC HAVE_STRING_H GNUREADLINE)
# define include directories, at build time and for clients
target_include_directories(parafeed_parafeed PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/parafeed>)
# install target
install(TARGETS parafeed_parafeed
  EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
# target alias in parafeed namespace
add_library(parafeed::parafeed ALIAS parafeed_parafeed)

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
target_sources(parafeed_parafeed
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
target_sources(parafeed_parafeed
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
target_sources(parafeed_parafeed PRIVATE ${sh_sources})

#add_custom_command(
#   OUTPUT callbacks_awk.cc
#   COMMAND sed -e "s/__REPLACE_ME_WITH_AWK_PROG_NAME__/awk/" callbacks.cc > callbacks_awk.cc
#   MAIN_DEPENDENCY callbacks.cc
#   COMMENT "Preprocessing callbacks.cc"
#   VERBATIM)

#
# shccl library
#

# we need to tell CMake that the sources are actually C++
set(shccl_sources
  ParseCmdLine.cc
  clparseVal.cc
  InstallSymb.cc
  clgetFVal.cc
  clgetIVal.cc
  clgetSVal.cc
  clgetFullVal.cc
  clgetNFVal.cc
  clgetNIVal.cc
  clgetNSVal.cc
  clgetNVals.cc
  cldbggetFVal.cc
  cldbggetIVal.cc
  cldbggetSVal.cc
  cldbggetNFVal.cc
  cldbggetNIVal.cc
  cldbggetNSVal.cc
  cldbggetFullVal.cc
  clStartInteractive.cc
  clRestartShell.cc
  clfInteractive.cc
  clstring.cc
  clstrtok.cc
  clgetOptsList.cc
  clclearOptsList.cc
  clgetCommandLine.cc
  clgetOpt.cc
  clstrtStream.cc
  clloadConfig.cc
  callbacks.cc
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
  clgetBVal.cc
  cldbggetBVal.cc
  clgetNBVal.cc
  cldbggetNBVal.cc
  clBreakStr.cc
  GetStdString.cc)
foreach(source IN LISTS shccl_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()
target_sources(parafeed_parafeed PRIVATE ${shccl_sources})

# add calc sub-directory
add_subdirectory(calc)

# define the parafeed_libraries export
install(EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed
  NAMESPACE parafeed::
  FILE parafeed_libraries.cmake)

add_executable(tstcpp tstcpp.cc)
target_link_libraries(tstcpp parafeed_parafeed)

# install all parafeed exports
install(FILES parafeedConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed)
