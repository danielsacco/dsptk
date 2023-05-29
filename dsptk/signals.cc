#include "signals.h"
#include "constants.h"
#include <cmath>

namespace dsptk {
	
	/* @brief creates a vector filled with a sine signal.
	* 
	* @param freq center frequency expressed as a fraction of the sample rate (0.0 to 0.5)
	* @param numberOfSamples number of samples in the vector.
	* @param gain the peak amplitude of the signal, 1.0 by default.
	* 
	* @return a vector filled with numberOfSamples of a signal at freq
	*/
	std::vector<double> sin(double freq, int numberOfSamples, double gain) {
		
		std::vector<double> signal(numberOfSamples);
		double k = dsptk::DOUBLE_PI * freq;
		for (int i = 0; i < signal.size(); i++) {
			signal[i] = std::sin(k * i) * gain;
		}

		return signal;
	}

	/* @brief creates a vector filled with a sine signal.
	*
	* @param freq center frequency in Hz
	* @param samplerate sampling frecuency in Hz
	* @param numberOfSamples number of samples in the vector.
	* @param gain the peak amplitude of the signal, 1.0 by default.
	*
	* @return a vector filled with numberOfSamples of a signal at freq
	*/
	std::vector<double> sin(double freq, double samplerate, int numberOfSamples, double gain) {
		return sin(freq/samplerate, numberOfSamples, gain);
	}
}