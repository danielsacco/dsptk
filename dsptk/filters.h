#pragma once
#include <vector>
#include <cmath>
#include <memory>

namespace dsptk {

	/* @brief Base class for all filters. 
	*/
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

	/* @brief Bank of filters connected in series.
	* TODO !!!!
	*/
	class FilterBank {
	public:
		void AddFilter(std::shared_ptr<Filter>& filter) {
			filters.push_back(filter);
		}

		void RemoveFilterAt(int position) {
			if (position < filters.size())
				filters.erase(filters.begin() + position);
		}

		double ProcessSample(double input) {
			double output = input;
			for (auto& filter : filters) {
				output = filter->ProcessSample(output);
			}
			return output;
		}

		void UpdateSamplerate(double samplerate) {
			for (auto& filter : filters) {
				filter->UpdateSamplerate(samplerate);
			}
		}

	private:
		std::vector<std::shared_ptr<Filter>> filters;
	};

	/** @brief Base class for bandpass/bandreject filters.
	*/
	class BandFilter : public Filter
	{
	public:
		BandFilter(double frequency, double bandwidth, double samplerate);

		/**
		* Bandwidth expressed in Hz, it should be internally calculated the "digital frequency"
		*/
		void UpdateBandwidth(double bandwidth);

		// This class does not implement the processing, is just a base class for bandpass bandreject filters
		virtual double ProcessSample(double input) = 0;

	protected:
		/**
		* Bandwidth expressed in Hz, it should be internally calculated the "digital frequency"
		*/
		double mBandwidth;
	};

	/* @brief Parametric filter
	* Bandwith set at 3dB below gain for boost, 3dB above gain for cut.
	* When gain is below 3dB, bandwith is set at the aritmethic media between 0dB and gain.
	* 
	* Reference gain G0 is fixed as 0dB.
	* 
	* Implementation according Sophocles Orfanidis - Introduction to Signal Processing - Second Edition section 12.4
	*/
	class ParametricFilter : public BandFilter
	{
	public:
		ParametricFilter(double frequency, double bandwidth, double initialGainDB, double samplerate);

		double ProcessSample(double input) override;

		void UpdateGainDB(double gainDB);

	private:
		// Filter state
		double w0 = .0;
		double w1 = .0;
		double w2 = .0;
		// Filter constants: Should be calculated at construction time and on parameters update.
		double b0, b1, b2, a1, a2;

		// Gain in dBs
		double mGainDB;

		inline void CalculateConstants() override;

		/** Calculates the beta factor.
		*/
		double CalculateBeta(double centerGain, double referenceGain, double bw);

	};

	/* @brief LowPass Shelving Filter filter
	* TODO!!!
	*/
	class LowPassShelvingFilter : public Filter
	{
	public:
		LowPassShelvingFilter(double frequency, double initialGainDB, double samplerate);

		double ProcessSample(double input) override;

		void UpdateGainDB(double gainDB);

	protected:
		inline void CalculateConstants() override;

	private:
		// Filter state
		double w0 = .0;
		double w1 = .0;

		// Filter constants: Should be calculated at construction time and on parameters update.
		double b0, b1, a1;

		// Gain in dBs
		double mGainDB;

		/** Calculates the beta factor.
*/
		double CalculateBeta(double centerGain, double referenceGain, double cutBoostFreq);

	};

	/* @brief HiPass Shelving Filter filter
	* TODO!!!
	*/
	class HiPassShelvingFilter : public Filter
	{
	public:
		HiPassShelvingFilter(double frequency, double initialGainDB, double samplerate);

		double ProcessSample(double input) override;

		void UpdateGainDB(double gainDB);

	protected:
		inline void CalculateConstants() override;

	private:
		// Filter state
		double w0 = .0;
		double w1 = .0;

		// Filter constants: Should be calculated at construction time and on parameters update.
		double b0, b1, a1;

		// Gain in dBs
		double mGainDB;

		/** Calculates the beta factor.
		*/
		double CalculateBeta(double centerGain, double referenceGain, double cutBoostFreq);

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

	class BandPassFilter : public BandFilter
	{
	public:
		BandPassFilter(double frequency, double bandwidth, double samplerate);

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