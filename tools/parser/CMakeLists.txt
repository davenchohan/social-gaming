include (cmake/CPM.cmake)

CPMAddPackage(
    NAME cpp-tree-sitter
    GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
    GIT_TAG v0.0.1
)

add_grammar_from_repo(tree-sitter-json
    https://github.com/tree-sitter/tree-sitter-json.git
    0.19.0
)

add_executable(parser_test)
target_sources(parser_test
    PRIVATE
        parser_test.cpp
)
target_link_libraries(parser_test
    tree-sitter-json
    cpp-tree-sitter
)

set_target_properties(parser_test
    PROPERTIES CXX_STANDARD 17
)
