add_subdirectory(gtest)

add_executable(runAllTests
# Add tests here
    dummy.cpp
)

target_link_libraries(runAllTests
  PRIVATE
    GTest::gmock GTest::gtest GTest::gtest_main
    networking
)

add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)


