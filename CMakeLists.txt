# Root Makelist
cmake_minimum_required(VERSION 3.19)
project(dsptk
	VERSION 0.1
	DESCRIPTION "A collection of DSP Tools - Just for learning purposes."
)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# GoogleTest requires at least C++14 but I'll use 17
set(CMAKE_CXX_STANDARD 17)

enable_testing()

# GoogleTest should not be installed with this project
option(INSTALL_GTEST "Enable installation of googletest." OFF)
option(INSTALL_GMOCK "Enable installation of googlemock." OFF)

add_subdirectory(dsptk)
add_subdirectory(test)
add_subdirectory(docs)

install(TARGETS dsptk
	LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib COMPONENT Development
)
