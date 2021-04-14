bison_target(CalcParser
  calc.y
  ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/y.tab.h)
flex_target(CalcScanner
  lex.l
  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
add_flex_bison_dependency(CalcScanner CalcParser)

add_library(parafeed_calc
  calcinit.c
  ${BISON_CalcParser_OUTPUTS}
  ${FLEX_CalcScanner_OUTPUTS})
target_link_libraries(parafeed_calc
  PRIVATE ${FLEX_LIBRARIES})
target_include_directories(parafeed_calc PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
set_target_properties(parafeed_calc PROPERTIES
  VERSION 0.0.1
  SOVERSION 0
  EXPORT_NAME calc
  LIBRARY_OUTPUT_NAME calc)
# install target
install(TARGETS parafeed_calc
  EXPORT parafeed_libraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR})
# target alias in parafeed namespace
add_library(parafeed::calc ALIAS parafeed_calc)
