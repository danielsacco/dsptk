#pragma once
#include <vector>
#include <cmath>

namespace dsptk {

	class DCBlocker final
	{
	public:
		DCBlocker(double frequency, double samplerate);

		double ProcessSample(double input);

		void UpdateSamplerate(double samplerate);

		void UpdateFrequency(double frequency);

	private:
		double frequency;
		double samplerate;
		double lastInput = .0;
		double lastOutput = .0;
		double R;

		inline void SetR();
	};

	class SinglePoleLowPass final
	{
	public:
		SinglePoleLowPass(double frequency, double samplerate);

		double ProcessSample(double input);

		void UpdateSamplerate(double samplerate);

		void UpdateFrequency(double frequency);

	private:
		double frequency;
		double samplerate;
		double lastOutput = .0;
		double a0, b1;

		inline void CalculateConstants();
	};

	class SinglePoleHiPass final
	{
	public:
		SinglePoleHiPass(double frequency, double samplerate);

		double ProcessSample(double input);

		void UpdateSamplerate(double samplerate);

		void UpdateFrequency(double frequency);

	private:
		double frequency;
		double samplerate;
		double lastInput = .0;
		double lastOutput = .0;
		double a0, a1, b1;

		inline void CalculateConstants();
	};

	class BandPassFilter final
	{
	public:
		BandPassFilter(double frequency, double bandwidth, double samplerate);

		double ProcessSample(double input);

		void UpdateSamplerate(double samplerate);

		void UpdateFrequency(double frequency);

		void UpdateBandwidth(double bandwidth);

	private:
		double frequency;
		double samplerate;
		double bandwidth;
		double in1 = .0;
		double in2 = .0;
		double out1 = .0;
		double out2 = .0;
		// Filter constants
		double a0, a1, a2, b1, b2;

		inline void CalculateConstants();
	};


	double MeanSquare(const std::vector<double>& input);

	double RootMeanSquare(const std::vector<double>& input);

}	// End namespace dsptk