#include "filters.h"
#include "constants.h"
#include <cmath>

namespace dsptk {

	Filter::Filter(double frequency, double samplerate)
		: mFrequency{ frequency }
		, mSamplerate{ samplerate }
	{
	}

	void Filter::UpdateSamplerate(double samplerate)
	{
		if (samplerate == mSamplerate) return;
		mSamplerate = samplerate;
		CalculateConstants();
	}

	void Filter::UpdateFrequency(double frequency)
	{
		if (frequency == mFrequency) return;
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
		if (mBandwidth == bandwidth) return;
		mBandwidth = bandwidth;
		CalculateConstants();
	}


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
		R = 1 - (DOUBLE_PI<double> * mFrequency / mSamplerate);
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
		b1 = std::exp( - (DOUBLE_PI<double> * mFrequency / mSamplerate));
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
		b1 = std::exp(-(DOUBLE_PI<double> * mFrequency / mSamplerate));
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
		double cosFactor = 2 * std::cos(DOUBLE_PI<double> * mFrequency / mSamplerate);
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
		double cosFactor = 2 * std::cos(DOUBLE_PI<double> * mFrequency / mSamplerate);
		double R = 1 - 3 * mBandwidth / mSamplerate;
		double RR = R * R;

		b1 = R * cosFactor;
		b2 = -RR;

		double K = (1 - b1 + RR) / (2 - cosFactor);

		a0 = K;
		a1 = - K * cosFactor;
		a2 = K;

	}

	ParametricFilter::ParametricFilter(double frequency, double bandwidth, DB gain, double samplerate)
		: BandFilter{ frequency, bandwidth, samplerate }
		, mGain{ gain }
	{
		CalculateConstants();
	}

	double ParametricFilter::ProcessSample(double input)
	{
		w0 = input - a1 * w1 - a2 * w2;
		double output = b0 * w0 + b1 * w1 + b2 * w2;

		// Update filter state
		w2 = w1;
		w1 = w0;

		return output;
	}

	void ParametricFilter::UpdateGain(DB gain)
	{
		if (gain == mGain) return;
		mGain = gain;
		CalculateConstants();
	}

	inline void ParametricFilter::CalculateConstants()
	{
		// Digital bandwidth
		double bw = DOUBLE_PI<double> * mBandwidth / mSamplerate;
		
		// Digital center frequency
		double fc = DOUBLE_PI<double> * mFrequency / mSamplerate;

		// Reference gain fixed at 0dB.
		double g0 = 1.;

		double linearGain = mGain.asLinearGain();

		// Beta factor
		double beta = CalculateBeta(linearGain, g0, bw);

		// Filter constants
		a1 = -2. * std::cos(fc) / (1. + beta);
		a2 = (1. - beta) / (1. + beta);
		b0 = (g0 + linearGain * beta) / (1. + beta);
		b1 = g0 * a1;
		b2 = (g0 - linearGain * beta) / (1. + beta);
	}

	/*
	* Calculates the beta factor according 
	* Sophocles Orfanidis - Introduction to Signal Processing - Second Edition 12.4.3
	*/
	double ParametricFilter::CalculateBeta(double centerGain, double referenceGain, double bw)
	{
		const double cut_boost = centerGain - referenceGain;

		// Square the center and reference gains
		const double gain2 = centerGain * centerGain;
		const double ref2 = referenceGain * referenceGain;

		double gbFactor = 1.;

		// We only need to calculate the gbFactor when:
		// - boost is more than 3dB above the reference
		// - cut is more than 3dB below the reference
		// the rest of the cases for aritmetic media between both gains yield 1 for the factor
		if ((cut_boost > 0) && (gain2 > 2. * ref2)) {
			// Boost is more than 3dB above the reference, set the bw 3dB below the peak
			double gb2 = gain2 / 2.;
			gbFactor = std::sqrt((gb2 - ref2) / (gain2 - gb2));
		}
		else if ((cut_boost < 0) && (gain2 < ref2 / 2.)) {
			// Cut is more than 3dB below the reference, set the bw 3dB above the notch
			double gb2 = 2. * gain2;
			gbFactor = std::sqrt((gb2 - ref2) / (gain2 - gb2));
		}

		return gbFactor * std::tan(bw / 2.);
	}

	LowPassShelvingFilter::LowPassShelvingFilter(double frequency, DB gain, double samplerate)
		: Filter{ frequency, samplerate }
		, mGain{ gain }
	{
		CalculateConstants();
	}

	double LowPassShelvingFilter::ProcessSample(double input)
	{
		w0 = input - a1 * w1;
		double output = b0 * w0 + b1 * w1;

		// Update filter state
		w1 = w0;

		return output;
	}

	void LowPassShelvingFilter::UpdateGain(DB gain)
	{
		if (gain == mGain) return;
		mGain = gain;
		CalculateConstants();
	}

	inline void LowPassShelvingFilter::CalculateConstants()
	{
		// Digital cut/boost frequency
		double fc = DOUBLE_PI<double> * mFrequency / mSamplerate;

		// Reference gain fixed at 0dB.
		double g0 = 1.;

		double linearGain = mGain.asLinearGain();

		// Beta factor
		double beta = CalculateBeta(linearGain, g0, fc);

		// Filter constants
		const double denominator = 1. + beta;
		a1 = -(1. - beta) / denominator;
		b0 = (g0 + linearGain * beta) / denominator;
		b1 = - (g0 - linearGain * beta) / denominator;
	}

	double LowPassShelvingFilter::CalculateBeta(double centerGain, double referenceGain, double cutBoostFreq)
	{
		const double cut_boost = centerGain - referenceGain;

		// Square the center and reference gains
		const double gain2 = centerGain * centerGain;
		const double ref2 = referenceGain * referenceGain;

		double gbFactor = 1.;

		// We only need to calculate the gbFactor when:
		// - boost is more than 3dB above the reference
		// - cut is more than 3dB below the reference
		// the rest of the cases for aritmetic media between both gains yield 1 for the factor
		if ((cut_boost > 0) && (gain2 > 2 * ref2)) {
			// Boost is more than 3dB above the reference, set the bw 3dB below the peak
			double gb2 = gain2 / 2.;
			gbFactor = std::sqrt((gb2 - ref2) / (gain2 - gb2));
		}
		else if ((cut_boost < 0) && (gain2 < ref2 / 2.)) {
			// Cut is more than 3dB below the reference, set the bw 3dB above the notch
			double gb2 = 2. * gain2;
			gbFactor = std::sqrt((gb2 - ref2) / (gain2 - gb2));
		}

		return gbFactor * std::tan(cutBoostFreq / 2.);
	}

	HiPassShelvingFilter::HiPassShelvingFilter(double frequency, DB gain, double samplerate)
		: Filter{ frequency, samplerate }
		, mGain{ gain }
	{
		CalculateConstants();
	}

	double HiPassShelvingFilter::ProcessSample(double input)
	{
		w0 = input - a1 * w1;
		double output = b0 * w0 + b1 * w1;

		// Update filter state
		w1 = w0;

		return output;
	}

	void HiPassShelvingFilter::UpdateGain(DB gain)
	{
		if (gain == mGain) return;
		mGain = gain;
		CalculateConstants();
	}

	inline void HiPassShelvingFilter::CalculateConstants()
	{
		// Digital cut/boost frequency
		double fc = DOUBLE_PI<double> * mFrequency / mSamplerate;

		// Reference gain fixed at 0dB.
		double g0 = 1.;

		double linearGain = mGain.asLinearGain();

		// Beta factor
		double beta = CalculateBeta(linearGain, g0, fc);

		// Filter constants
		const double denominator = 1. + beta;
		a1 = (1. - beta) / denominator;
		b0 = (g0 + linearGain * beta) / denominator;
		b1 = (g0 - linearGain * beta) / denominator;
	}

	double HiPassShelvingFilter::CalculateBeta(double centerGain, double referenceGain, double cutBoostFreq)
	{
		const double cut_boost = centerGain - referenceGain;

		// Square the center and reference gains
		const double gain2 = centerGain * centerGain;
		const double ref2 = referenceGain * referenceGain;

		double gbFactor = 1.;

		// We only need to calculate the gbFactor when:
		// - boost is more than 3dB above the reference
		// - cut is more than 3dB below the reference
		// the rest of the cases for aritmetic media between both gains yield 1 for the factor
		if ((cut_boost > 0) && (gain2 > 2. * ref2)) {
			// Boost is more than 3dB above the reference, set the bw 3dB below the peak
			double gb2 = gain2 / 2.;
			gbFactor = std::sqrt((gb2 - ref2) / (gain2 - gb2));
		}
		else if ((cut_boost < 0) && (gain2 < ref2 / 2.)) {
			// Cut is more than 3dB below the reference, set the bw 3dB above the notch
			double gb2 = 2 * gain2;
			gbFactor = std::sqrt((gb2 - ref2) / (gain2 - gb2));
		}

		return gbFactor / std::tan(cutBoostFreq / 2.);
	}

}	// End namespace dsptk