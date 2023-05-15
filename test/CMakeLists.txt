# Tests Makelist

include_directories(${PROJECT_SOURCE_DIR})

add_executable(
  dsptk_test
  dsptk.test.cpp
)
target_link_libraries(
  dsptk_test
  GTest::gtest_main
  dsptk
)


include(GoogleTest)
gtest_discover_tests(dsptk_test)
