include (include/CPM.cmake)

CPMAddPackage(
    NAME nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

add_library(backendcomponents
    src/ParserLibrary.cpp
)

target_include_directories(backendcomponents
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(backendcomponents
    nlohmann_json::nlohmann_json
)

set_target_properties(backendcomponents
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

install(TARGETS backendcomponents
  ARCHIVE DESTINATION lib
)