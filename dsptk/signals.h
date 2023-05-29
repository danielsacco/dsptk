#pragma once

#include <vector>

namespace dsptk {

	std::vector<double> sin(double freq, int numberOfSamples, double gain = 1.);

	std::vector<double> sin(double freq, double samplerate, int numberOfSamples, double gain = 1.);
}
