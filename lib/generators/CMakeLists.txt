add_library(generators)

target_sources(generators
  PRIVATE
  src/RandomIdGenerator.cpp
  src/RoomCodeGenerator.cpp
)

target_include_directories(generators
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)