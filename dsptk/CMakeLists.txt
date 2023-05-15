# Library Makelist

project(dsptk_lib)

add_library(dsptk STATIC
	"detector.h"
	"detector.cc"
	"dynamics.h" 
	"dynamics.cc" 
	"conversions.h"
)

install(FILES 
	"detector.h" "dynamics.h" "conversions.h" DESTINATION include
)
