include (cmake/CPM.cmake)

CPMAddPackage(
    NAME cpp-tree-sitter
    GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
    GIT_TAG v0.0.1
)

CPMAddPackage(
    NAME nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

add_grammar_from_repo(tree-sitter-json
    https://github.com/tree-sitter/tree-sitter-json.git
    0.19.0
)

add_grammar_from_repo(tree-sitter-socialgaming
    https://github.com/nsumner/tree-sitter-socialgaming.git
    0.0.1
)

add_executable(parser_test)
target_sources(parser_test
    PRIVATE
        parser_test.cpp
)

target_link_libraries(parser_test
    tree-sitter-json
    tree-sitter-socialgaming
    cpp-tree-sitter
    nlohmann_json::nlohmann_json
    executionnodes
    datatypes
)

set_target_properties(parser_test
    PROPERTIES CXX_STANDARD 17
)


target_include_directories(parser_test
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)


set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gameSpecs")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/lib")
file(COPY ${SOURCE_DIR} DESTINATION ${DESTINATION_DIR})