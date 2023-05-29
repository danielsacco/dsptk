#include "filters.h"
#include "constants.h"
#include <cmath>

namespace dsptk {

	// Solution reference: https://www.musicdsp.org/en/latest/Filters/135-dc-filter.html
	DCBlocker::DCBlocker(double freq, double sRate)
		: frequency{ freq }
		, samplerate{ sRate }
	{
		SetR();
	}

	double DCBlocker::ProcessSample(double input)
	{
		double output = input - lastInput + R * lastOutput;

		lastInput = input;
		lastOutput = output;

		return output;
	}

	void DCBlocker::UpdateSamplerate(double sRate) {
		samplerate = sRate;
		SetR();
	}

	void DCBlocker::UpdateFrequency(double freq) {
		frequency = freq;
		SetR();
	}

	inline void DCBlocker::SetR()
	{
		R = 1 - (DOUBLE_PI * frequency / samplerate);
	}

	// Single Pole Recursive Filters. Digital Signal Processing Steven W. Smith Page 322
	SinglePoleLowPass::SinglePoleLowPass(double freq, double sRate)
		: frequency{ freq }
		, samplerate{ sRate }
	{
		CalculateConstants();
	}

	double SinglePoleLowPass::ProcessSample(double input)
	{
		double output = a0 * input + b1 * lastOutput;

		lastOutput = output;

		return output;
	}

	void SinglePoleLowPass::UpdateSamplerate(double sRate) {
		samplerate = sRate;
		CalculateConstants();
	}

	void SinglePoleLowPass::UpdateFrequency(double freq) {
		frequency = freq;
		CalculateConstants();
	}

	inline void SinglePoleLowPass::CalculateConstants()
	{
		b1 = std::exp( - (DOUBLE_PI * frequency / samplerate));
		a0 = 1 - b1;
	}


	// Single Pole Recursive Filters. Digital Signal Processing Steven W. Smith Page 322
	SinglePoleHiPass::SinglePoleHiPass(double freq, double sRate)
		: frequency{ freq }
		, samplerate{ sRate }
	{
		CalculateConstants();
	}

	double SinglePoleHiPass::ProcessSample(double input)
	{
		double output = a0 * input + a1 * lastInput + b1 * lastOutput;

		lastInput = input;
		lastOutput = output;

		return output;
	}

	void SinglePoleHiPass::UpdateSamplerate(double sRate) {
		samplerate = sRate;
		CalculateConstants();
	}

	void SinglePoleHiPass::UpdateFrequency(double freq) {
		frequency = freq;
		CalculateConstants();
	}

	inline void SinglePoleHiPass::CalculateConstants()
	{
		b1 = std::exp(-(DOUBLE_PI * frequency / samplerate));
		a0 = (1 + b1) / 2.;
		a1 = - a0;
	}


	double MeanSquare(const std::vector<double>& input) {

		if (input.size() < 2) return 0.;

		double ms = 0.;

		for (double value : input) {
			ms += value * value;
		}
		return ms / (double)(input.size() - 1);
	}

	double RootMeanSquare(const std::vector<double>& input) {
		return std::sqrt(MeanSquare(input));
	}

}	// End namespace dsptk