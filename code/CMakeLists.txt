#
# define parser and scanner targets
#
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
  isBool.cc
  yyerror.c)
foreach(source IN LISTS sh_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()

# define the sh library and its sources
add_library(parafeed_sh STATIC
  ${sh_sources}
  ${BISON_ShellParser_OUTPUTS}
  ${FLEX_ShellScanner_OUTPUTS})
# link to readline and flex (libfl)
target_link_libraries(parafeed_sh
  PUBLIC parafeed_shccl PkgConfig::readline
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
  OUTPUT_NAME sh)
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
#  sed -e "s/__REPLACE_ME_WITH_AWK_PROG_NAME__/awk/" callbacks.c > callbacks_awk.c
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
  clBreakStr.cc
  GetStdString.cc)
foreach(source IN LISTS shccl_sources)
  set_property(SOURCE ${source} PROPERTY LANGUAGE CXX)
endforeach()

# define the shccl library and its sources
add_library(parafeed_shccl STATIC
  ${shccl_sources}
  shell.tab.h)
# link to readline
target_link_libraries(parafeed_shccl
  PUBLIC parafeed_sh parafeed_calc PkgConfig::readline)
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
  OUTPUT_NAME shccl)
# install target
install(TARGETS parafeed_shccl
  EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
# target alias in parafeed namespace
add_library(parafeed::shccl ALIAS parafeed_shccl)

# add calc sub-directory
add_subdirectory(calc)

#
# create a single, composite library
#
add_library(parafeed_parafeed INTERFACE)
target_link_libraries(parafeed_parafeed
  INTERFACE parafeed_calc parafeed_shccl parafeed_sh)
set_target_properties(parafeed_parafeed PROPERTIES
  EXPORT_NAME parafeed)
# install target
install(TARGETS parafeed_parafeed
  EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
# target alias in parafeed namespace
add_library(parafeed::parafeed ALIAS parafeed_parafeed)

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
