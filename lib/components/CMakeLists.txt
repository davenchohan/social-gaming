
include(FetchContent)

add_library(components
    src/LandingPage.cpp
    src/JoinGamePage.cpp
    src/CreateGamePage.cpp
    src/MobileLandingPage.cpp
    src/CreateFromExistingPage.cpp
    src/CreateGameSessionPage.cpp
    src/GameComponent.cpp
    src/TestGameComponent.cpp
    src/MainPageContent.cpp
)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)


FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(components
  PRIVATE backendClasses
  PRIVATE backendcomponents
  PRIVATE networking
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE constants
)
target_include_directories(components
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(components
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)
if (NETWORKING_EMSCRIPTEN_BUILD)
target_compile_options(components
    PRIVATE
      -s USE_PTHREADS   
  )
  # NOTE: emscripten intercepts linker arguments, so passing custom
  #  arguments means that we need to remove the compiler specific flags
  #  that CMake would otherwise add in.
  set (CMAKE_CXX_LINKER_WRAPPER_FLAG " ")
  target_link_options(components
    PRIVATE
      "LINKER:-sUSE_PTHREADS,-sASYNCIFY,-sPROXY_TO_PTHREAD"
  )
  endif()
