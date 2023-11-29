include (include/CPM.cmake)

CPMAddPackage(
    NAME nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

add_library(backendcomponents
    src/ParserLibrary.cpp
    src/ResponseQueue.cpp
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
    backendClasses
)

set_target_properties(backendcomponents
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

install(TARGETS backendcomponents
  ARCHIVE DESTINATION lib
)

if (NETWORKING_EMSCRIPTEN_BUILD)
target_compile_options(backendcomponents
    PRIVATE
      -s USE_PTHREADS   
  )
  # NOTE: emscripten intercepts linker arguments, so passing custom
  #  arguments means that we need to remove the compiler specific flags
  #  that CMake would otherwise add in.
  set (CMAKE_CXX_LINKER_WRAPPER_FLAG " ")
  target_link_options(backendcomponents
    PRIVATE
      "LINKER:-sUSE_PTHREADS,-sASYNCIFY,-sPROXY_TO_PTHREAD"
  )
  endif()