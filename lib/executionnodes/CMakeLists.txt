add_library(executionnodes
    src/Nodes.cpp
    src/ExpressNodes.cpp
    src/Tree.cpp
)

target_include_directories(executionnodes
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(executionnodes
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

install(TARGETS executionnodes
  ARCHIVE DESTINATION lib
)
