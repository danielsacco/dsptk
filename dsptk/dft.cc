#include "dft.h"
#include "constants.h"
#include <cmath>
#include <limits>

namespace dsptk {

	std::array<std::vector<double>, 2> real_dft_analysis(const std::vector<double>& signal) {

		const int reX = 0;
		const int imX = 1;

		const size_t N = signal.size();
		const size_t resultSize = 1 + N / 2;
		std::array < std::vector<double>, 2> result { 
			std::vector<double>(resultSize, 0.), 
			std::vector<double>(resultSize, 0.)
		};

		// Loop each frequency (k)
		for (int k = 0; k < resultSize; k++) {
			// For each sample, accumulate the basis function result
			for (int n = 0; n < N; n++) {
				double factor = (double)k * (double)n / (double)N;
				double whole;	// We will not use this value
				double fractional_factor = std::modf(factor, &whole);

				// Special phase cases:
				if (fractional_factor == 0.) {
					// cos(0) = 1; sin(0) = 0
					result[reX][k] += signal[n];
				}
				else if (fractional_factor == 0.25) {
					// cos(Pi/2) = 0; sin(Pi/2) = 1
					result[imX][k] -= signal[n];
				}
				else if (fractional_factor == 0.5) {
					// cos(Pi) = -1; sin(Pi) = 0
					result[reX][k] -= signal[n];
				}
				else if (fractional_factor == 0.75) {
					// cos(3Pi/2) = 0; sin(3Pi/2) = -1
					result[imX][k] += signal[n];
				}
				else {
					// Use sin/cos functions
					double phase = dsptk::DOUBLE_PI<double> * fractional_factor;
					result[reX][k] += signal[n] * std::cos(phase);
					result[imX][k] -= signal[n] * std::sin(phase);
				}

				// TODO: Check if this is the rigth way to get rid of the double "noise"
				// Clear accumulated value if around 0
				double reValue = result[reX][k];
				if (std::fabs(reValue) < std::numeric_limits<double>::epsilon() * 2.) {
					result[reX][k] = 0.;
				}
				double imValue = result[imX][k];
				if (std::fabs(imValue) < std::numeric_limits<double>::epsilon() * 2.) {
					result[imX][k] = 0.;
				}
			}
		}

		return result;
	}

}