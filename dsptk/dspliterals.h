#pragma once

#include "dsptypes.h"

namespace dsptk {

	namespace literals {

		DB operator ""_dB(const long double dBValue) {
			return DB(dBValue);
		}
	}

}



