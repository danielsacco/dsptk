# Tests Makelist

include_directories(${PROJECT_SOURCE_DIR})

add_executable(
  dsptk_test
  "detector_test.cc"
  "convolution_test.cc"
  "dft_test.cc"
  "filters_test.cc"
  "db_test.cc"
)
target_link_libraries(
  dsptk_test
  GTest::gtest_main
  GTest::gmock_main
  dsptk
)


include(GoogleTest)
gtest_discover_tests(dsptk_test)
