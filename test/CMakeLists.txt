# Tests Makelist

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  #GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1 # v1.13.0
  GIT_TAG        v1.13.0 # v1.13.0 <- probar con esto
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

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
gtest_discover_tests(dsptk_test DISCOVERY_MODE PRE_TEST)
