# Library Makelist

project(dsptk_lib)

add_library(dsptk STATIC
	"detector.h"
	"detector.cc"
	"dynamics.h" 
	"dynamics.cc" 
	"filters.h" 
	"filters.cc"
	"constants.h"
	"convolution.h"
	"convolution.cc"
 "dft.h" "dft.cc" "signals.h" "signals.cc" "dsptypes.h" "dspliterals.h")

install(FILES 
	"detector.h" 
	"dynamics.h"
	"filters.h" 
	"constants.h" 
	"convolution.h" 
	"dft.h" 
	"signals.h" 
	"dsptypes.h" 
	"dspliterals.h" DESTINATION include
)
