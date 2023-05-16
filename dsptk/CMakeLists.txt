# Library Makelist

project(dsptk_lib)

add_library(dsptk STATIC
	"detector.h"
	"detector.cc"
	"dynamics.h" 
	"dynamics.cc" 
	"conversions.h"
	"filters.h" 
	"filters.cc"
	"constants.h"
)

install(FILES 
	"detector.h" "dynamics.h" "conversions.h" "filters.h" "constants.h" DESTINATION include
)
