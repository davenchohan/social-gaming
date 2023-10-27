add_executable(plExample
  ParserLibraryExample.cpp
)

set_target_properties(plExample
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(plExample
  networking
  components
  exceptions
  backendClasses
  backendcomponents
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS plExample
  RUNTIME DESTINATION bin
)

