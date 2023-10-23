
add_executable(hotserver
  hotserver.cpp
)

add_executable(plrunner
  plRunner.cpp
)

set_target_properties(hotserver
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

set_target_properties(plrunner
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(hotserver
  networking
  components
  exceptions
  backendClasses
  backendcomponents
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(plrunner
  networking
  components
  exceptions
  backendClasses
  backendcomponents
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS hotserver
  RUNTIME DESTINATION bin
)

install(TARGETS plrunner
  RUNTIME DESTINATION bin
)

