#pragma once
#include <vector>
#include <cmath>

namespace dsptk {

	class Filter {
	public:
		Filter(double frequency, double samplerate);

		virtual double ProcessSample(double input) = 0;

		void UpdateSamplerate(double samplerate);
		void UpdateFrequency(double frequency);

	protected:
		double mFrequency;
		double mSamplerate;

		inline virtual void CalculateConstants() = 0;

	};

	class DCBlocker : public Filter
	{
	public:
		DCBlocker(double frequency, double samplerate);

		double ProcessSample(double input) override;

	private:
		double lastInput = .0;
		double lastOutput = .0;
		double R;

		inline void CalculateConstants() override;
	};

	class SinglePoleLowPass : public Filter
	{
	public:
		SinglePoleLowPass(double frequency, double samplerate);

		double ProcessSample(double input) override;

	private:
		double lastOutput = .0;
		double a0, b1;

		inline void CalculateConstants() override;
	};

	class SinglePoleHiPass : public Filter
	{
	public:
		SinglePoleHiPass(double frequency, double samplerate);

		double ProcessSample(double input) override;

	private:
		double lastInput = .0;
		double lastOutput = .0;
		double a0, a1, b1;

		inline void CalculateConstants() override;
	};

	class BandFilter : public Filter 
	{
	public:
		BandFilter(double frequency, double bandwidth, double samplerate);

		void UpdateBandwidth(double bandwidth);

	protected:
		double mBandwidth;
	};

	class BandPassFilter : public BandFilter
	{
	public:
		BandPassFilter(double frequency, double bandwidth, double samplerate);

		double ProcessSample(double input);

	private:
		double in1 = .0;
		double in2 = .0;
		double out1 = .0;
		double out2 = .0;
		// Filter constants
		double a0, a1, a2, b1, b2;

		inline void CalculateConstants() override;
	};

	class BandRejectFilter : public BandFilter
	{
	public:
		BandRejectFilter(double frequency, double bandwidth, double samplerate);

		double ProcessSample(double input) override;


	private:
		double in1 = .0;
		double in2 = .0;
		double out1 = .0;
		double out2 = .0;
		// Filter constants
		double a0, a1, a2, b1, b2;

		inline void CalculateConstants() override;
	};

	double MeanSquare(const std::vector<double>& input);

	double RootMeanSquare(const std::vector<double>& input);

}	// End namespace dsptk