#pragma once

#include <vector>

namespace dsptk {

	/**
	* /brief Standard convolution operation, just for practicing C++.
	*/
	std::vector<double> convolve(const std::vector<double>&, const std::vector<double>&);

	/**
	* /brief Standard convolution operation using input side algorithm, just for practicing C++.
	*/
	std::vector<double> convolve_in(const std::vector<double>&, const std::vector<double>&);

	/**
	* /brief Standard convolution operation using output side algorithm, just for practicing C++.
	*/
	std::vector<double> convolve_out(const std::vector<double>&, const std::vector<double>&);

}