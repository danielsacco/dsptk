#pragma once

#include <vector>
#include <array>

namespace dsptk {
	/**
	* /brief Discrete Fourier Transform by correlation, just for practicing C++.
	*/
	std::array<std::vector<double>, 2> real_dft_analysis(const std::vector<double>&);

}