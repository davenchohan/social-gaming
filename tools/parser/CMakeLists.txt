include (cmake/CPM.cmake)

CPMAddPackage(
    NAME cpp-tree-sitter
    GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
    GIT_TAG v0.0.2
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
        rule_interpreter.cpp
)

target_link_libraries(parser_test
    tree-sitter-json
    tree-sitter-socialgaming
    cpp-tree-sitter
    nlohmann_json::nlohmann_json
    executionnodes
    datatypes
    backendClasses
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



add_executable(variable_parser)
target_sources(variable_parser
    PRIVATE
        variable_parser.cpp
)

target_link_libraries(variable_parser
    tree-sitter-json
    tree-sitter-socialgaming
    cpp-tree-sitter
)

set_target_properties(variable_parser
    PROPERTIES CXX_STANDARD 17
)


target_include_directories(variable_parser
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)


set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gameSpecs")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/lib")
file(COPY ${SOURCE_DIR} DESTINATION ${DESTINATION_DIR})