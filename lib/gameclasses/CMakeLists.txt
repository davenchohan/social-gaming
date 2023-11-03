add_library(backendClasses)

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