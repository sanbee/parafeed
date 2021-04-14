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
foreach(source IN LISTS shccl_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()

# define the sh library and its sources
add_library(parafeed_sh
  ${sh_sources}
  ${BISON_ShellParser_OUTPUTS}
  ${FLEX_ShellScanner_OUTPUTS})
# link to readline and flex (libfl)
target_link_libraries(parafeed_sh
  PUBLIC PkgConfig::readline
  PRIVATE ${FLEX_LIBRARIES})
# compile definitions (like -DFOO)
target_compile_definitions(parafeed_sh PUBLIC HAVE_STRING_H GNUREADLINE)
# define include directories, at build time and for clients
target_include_directories(parafeed_sh PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/parafeed>)
# set some properties to get the desired library name and version
set_target_properties(parafeed_sh PROPERTIES
  VERSION 0.0.1
  SOVERSION 0
  EXPORT_NAME sh
  LIBRARY_OUTPUT_NAME sh)
# install target
install(TARGETS parafeed_sh
  EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
# target alias in parafeed namespace
add_library(parafeed::sh ALIAS parafeed_sh)

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
  clgetBVal.c
  cldbggetBVal.c
  clgetNBVal.c
  cldbggetNBVal.c
  clBreakStr.cc)
foreach(source IN LISTS shccl_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()

# define the shccl library and its sources
add_library(parafeed_shccl
  ${shccl_sources}
  shell.tab.h)
# link to readline
target_link_libraries(parafeed_shccl
  PUBLIC PkgConfig::readline)
# compile definitions (like -DFOO)
target_compile_definitions(parafeed_shccl PUBLIC HAVE_STRING_H GNUREADLINE)
# include directories, both at build and for clients
target_include_directories(parafeed_shccl PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/parafeed>)
# set some properties to get the desired library name and version
set_target_properties(parafeed_shccl PROPERTIES
  VERSION 0.0.1
  SOVERSION 0
  EXPORT_NAME shccl
  LIBRARY_OUTPUT_NAME shccl)
# install target
install(TARGETS parafeed_shccl
  EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
# target alias in parafeed namespace
add_library(parafeed::shccl ALIAS parafeed_shccl)

# add calc sub-directory
add_subdirectory(calc)

# define the parafeed_libraries export
install(EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed
  NAMESPACE parafeed::
  FILE parafeed_libraries.cmake)

# install all parafeed exports
# configure_file(parafeedConfig.cmake.in parafeedConfig.cmake @ONLY)
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/parafeedConfig.cmake
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/parafeed)
