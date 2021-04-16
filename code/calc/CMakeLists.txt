target_sources(parafeed_parafeed PRIVATE calcinit.c)
target_include_directories(parafeed_parafeed PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
