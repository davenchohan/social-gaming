add_subdirectory(gtest)

add_executable(runAllTests
# Add tests here
    dummy.cpp
    CustomExceptionsTests.cpp
<<<<<<< HEAD
    GameSessionHandlerTests.cpp
=======
    ParserLibraryTests.cpp
    GameSessionHandlerTests.cpp
    GamesDatabaseTests.cpp
    GameClassTests.cpp
>>>>>>> error-kaixiny
)

target_link_libraries(runAllTests
  PRIVATE
    GTest::gmock GTest::gtest GTest::gtest_main
    networking
    exceptions
<<<<<<< HEAD
    gameclasses
=======
    backendcomponents
    backendClasses
)

set_target_properties(runAllTests
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
>>>>>>> error-kaixiny
)

add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)


