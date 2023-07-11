# Root Makelist
cmake_minimum_required(VERSION 3.14)
project(dsptk
	VERSION 0.1
	DESCRIPTION "A collection of DSP Tools - Just for learning purposes."
)

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# GoogleTest requires at least C++14 but I'll use 17
set(CMAKE_CXX_STANDARD 17)


enable_testing()

add_subdirectory(dsptk)
add_subdirectory(test EXCLUDE_FROM_ALL)
add_subdirectory(docs)

install(TARGETS dsptk
	LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib COMPONENT Development
)
