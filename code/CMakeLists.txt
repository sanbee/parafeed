#
# create a single, composite library
#
add_library(parafeed_parafeed)
set_target_properties(parafeed_parafeed PROPERTIES
  VERSION 0.0.1
  SOVERSION 0
  EXPORT_NAME parafeed
  OUTPUT_NAME parafeed)
# link to readline
target_link_libraries(parafeed_parafeed
  PUBLIC PkgConfig::readline)
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
bison_target(ShellParser
  shell.y
  ${CMAKE_CURRENT_BINARY_DIR}/shell.tab.c
  COMPILE_FLAGS "-p sh_"
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/shell.tab.h)
flex_target(ShellScanner
  shell.l
  ${CMAKE_CURRENT_BINARY_DIR}/shell.c
  COMPILE_FLAGS "-Ce -Psh_")
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
  CopySymb.c
  AddCmd.c
  AddNode.c
  AddVar.c
  AllocSymb.c
  AddQKey.c
  DeleteCmd.c
  DeleteVar.c
  DeleteNode.c
  DeleteTab.c
  FreeSymb.c
  SearchNode.c
  SearchSymb.c
  TransferTab.c
  PrintVals.c
  SetVar.c
  SetPrompt.c
  SetSymbTab.c
  SetCmdTab.c
  yyerror.c
  minmatch.c
  isBool.cc)
foreach(source IN LISTS sh_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()
target_sources(parafeed_parafeed PRIVATE ${sh_sources})

#
# shccl library
#

# we need to tell CMake that the sources are actually C++
set(shccl_sources
  ParseCmdLine.c
  clparseVal.c
  InstallSymb.c
  clgetFVal.c
  clgetIVal.c
  clgetSVal.c
  clgetFullVal.c
  clgetNFVal.c
  clgetNIVal.c
  clgetNSVal.c
  clgetNVals.c
  cldbggetFVal.c
  cldbggetIVal.c
  cldbggetSVal.c
  cldbggetNFVal.c
  cldbggetNIVal.c
  cldbggetNSVal.c
  cldbggetFullVal.c
  clStartInteractive.c
  clRestartShell.c
  clfInteractive.c
  clstring.c
  clgetOptsList.c
  clclearOptsList.c
  clgetCommandLine.c
  clgetOpt.c
  clstrtStream.c
  clloadConfig.c
  callbacks.c
  support.c
  clgetInputFile.c
  clgetOutputFile.c
  clgetNOpts.c
  clCleanUp.c
  clReset.c
  clgetConfigFile.c
  clSaneExit.c
  clThrowError.c
  clRetry.c
  clSigHandler.c
  clIsInteractive.c
  ErrorObj.cc
  rl_interface.cc
  clSetOptions.cc
  clTextColouring.cc
  clgetBVal.cc
  cldbggetBVal.c
  clgetNBVal.c
  cldbggetNBVal.c
  clBreakStr.cc)
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
