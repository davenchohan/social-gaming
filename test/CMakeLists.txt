add_subdirectory(gtest)

add_executable(runAllTests
# Add tests here
    dummy.cpp
    CustomExceptionsTests.cpp
    ParserLibraryTests.cpp
)

target_link_libraries(runAllTests
  PRIVATE
    GTest::gmock GTest::gtest GTest::gtest_main
    networking
    exceptions
    backendcomponents
)

set_target_properties(runAllTests
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)


