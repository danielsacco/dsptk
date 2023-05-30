#include "filters.h"
#include "constants.h"
#include <cmath>

namespace dsptk {

	// Solution reference: https://www.musicdsp.org/en/latest/Filters/135-dc-filter.html
	DCBlocker::DCBlocker(double freq, double sRate) : Filter{freq, sRate}
	{
		CalculateConstants();
	}

	double DCBlocker::ProcessSample(double input)
	{
		double output = input - lastInput + R * lastOutput;

		lastInput = input;
		lastOutput = output;

		return output;
	}

	inline void DCBlocker::CalculateConstants()
	{
		R = 1 - (DOUBLE_PI * mFrequency / mSamplerate);
	}

	// Single Pole Recursive Filters. Digital Signal Processing Steven W. Smith Page 322
	SinglePoleLowPass::SinglePoleLowPass(double freq, double sRate) : Filter{ freq, sRate }
	{
		CalculateConstants();
	}

	double SinglePoleLowPass::ProcessSample(double input)
	{
		double output = a0 * input + b1 * lastOutput;

		lastOutput = output;

		return output;
	}

	inline void SinglePoleLowPass::CalculateConstants()
	{
		b1 = std::exp( - (DOUBLE_PI * mFrequency / mSamplerate));
		a0 = 1 - b1;
	}


	// Single Pole Recursive Filters. Digital Signal Processing Steven W. Smith Page 322
	SinglePoleHiPass::SinglePoleHiPass(double freq, double sRate) : Filter{ freq, sRate }
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

	inline void SinglePoleHiPass::CalculateConstants()
	{
		b1 = std::exp(-(DOUBLE_PI * mFrequency / mSamplerate));
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

	BandPassFilter::BandPassFilter(double frequency, double bandwidth, double samplerate)
		: BandFilter{ frequency, bandwidth, samplerate }
	{
		CalculateConstants();
	}

	double BandPassFilter::ProcessSample(double input)
	{
		double output = a0 * input + a1 * in1 + a2 * in2 + b1 * out1 + b2 * out2;

		// Shift samples
		out2 = out1;
		out1 = output;
		in2 = in1;
		in1 = input;

		return output;
	}

	inline void BandPassFilter::CalculateConstants()
	{
		double cosFactor = 2 * std::cos(DOUBLE_PI * mFrequency / mSamplerate);
		double R = 1 - 3 * mBandwidth / mSamplerate;
		double RR = R * R;

		b1 = R * cosFactor;
		b2 = -RR;

		double K = (1 - b1 + RR) / (2 - cosFactor);

		a0 = 1 - K;
		a1 = (K - R) * cosFactor;
		a2 = RR - K;

	}

	BandRejectFilter::BandRejectFilter(double frequency, double bandwidth, double samplerate)
		: BandFilter{ frequency, bandwidth, samplerate }
	{
		CalculateConstants();
	}

	double BandRejectFilter::ProcessSample(double input)
	{
		double output = a0 * input + a1 * in1 + a2 * in2 + b1 * out1 + b2 * out2;

		// Shift samples
		out2 = out1;
		out1 = output;
		in2 = in1;
		in1 = input;

		return output;
	}

	inline void BandRejectFilter::CalculateConstants()
	{
		double cosFactor = 2 * std::cos(DOUBLE_PI * mFrequency / mSamplerate);
		double R = 1 - 3 * mBandwidth / mSamplerate;
		double RR = R * R;

		b1 = R * cosFactor;
		b2 = -RR;

		double K = (1 - b1 + RR) / (2 - cosFactor);

		a0 = K;
		a1 = - K * cosFactor;
		a2 = K;

	}

	Filter::Filter(double frequency, double samplerate)
		: mFrequency{frequency}
		, mSamplerate{samplerate}
	{
	}

	void Filter::UpdateSamplerate(double samplerate)
	{
		mSamplerate = samplerate;
		CalculateConstants();
	}

	void Filter::UpdateFrequency(double frequency)
	{
		mFrequency = frequency;
		CalculateConstants();
	}

	BandFilter::BandFilter(double frequency, double bandwidth, double samplerate)
		:Filter{ frequency, samplerate }
		, mBandwidth{ bandwidth }
	{
	}

	void BandFilter::UpdateBandwidth(double bandwidth)
	{
		mBandwidth = bandwidth;
		CalculateConstants();
	}

}	// End namespace dsptk