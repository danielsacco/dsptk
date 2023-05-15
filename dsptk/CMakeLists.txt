# Library Makelist

project(dsptk_lib)

add_library(dsptk STATIC
  dsptk.cpp
  dsptk.hpp
)

install(FILES dsptk.hpp DESTINATION include)
