
add_executable(hotserver
  hotserver.cpp
)

set_target_properties(hotserver
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

install(TARGETS hotserver
  RUNTIME DESTINATION bin
)

