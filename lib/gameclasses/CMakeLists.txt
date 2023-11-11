add_library(backendClasses)

include (../../../tools/parser/cmake/CPM.cmake)


CPMAddPackage(
    NAME cpp-tree-sitter
    GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
    GIT_TAG v0.0.1
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


set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/lib")
file(COPY ${SOURCE_DIR} DESTINATION ${DESTINATION_DIR})






target_sources(backendClasses
  PRIVATE
  AudienceMember.cpp
  Game.cpp
  GameConstant.cpp
  GameList.cpp
  GameSessionHandler.cpp
  GameVariable.cpp
  Player.cpp
  User.cpp
  GameSessionList.cpp
  GamesDatabase.cpp
)

target_include_directories(backendClasses
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(networking
PRIVATE
  executionnodes
)