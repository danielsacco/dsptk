#include "convolution.h"
#include <algorithm>
#include <iterator>

namespace dsptk {
	std::vector<double> convolve(const std::vector<double>& input, const std::vector<double>& kernel) {

		if(input.size() == 0 || kernel.size() == 0) return std::vector<double>(0);

		size_t resultSize = input.size() + kernel.size() - 1;
		auto result = std::vector<double>(resultSize, 0.0);

		for (int i = 0; i < input.size(); i++) {
			for (int j = 0; j < kernel.size(); j++) {
				result[i + j] += input[i] * kernel[j];
			}
		}

		return result;
	}

	std::vector<double> convolve_in(const std::vector<double>& input, const std::vector<double>& kernel) {
		return convolve(input, kernel);
	}

	std::vector<double> convolve_out(const std::vector<double>& input, const std::vector<double>& kernel) {
		if (input.size() == 0 || kernel.size() == 0) return std::vector<double>(0);
		int resultSize = input.size() + kernel.size() - 1;
		auto result = std::vector<double>(resultSize, 0.);

		for (int i = 0; i < resultSize; i++) {
			// Sum each relevant point into the result
			for (int j = 0; j < kernel.size(); j++) {
				int input_index = i - j;
				if (input_index >= 0 && input_index < input.size())
					result[i] += kernel[j] * input[input_index];
			}
		}

		return result;
	}

}