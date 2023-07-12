#pragma once
#include <vector>
#include <cmath>
#include <memory>
#include "dsptypes.h"

namespace dsptk {

	/**
	 * @brief  Base class for all filters.
	*/
	class Filter {
	public:
		/**
		 * @brief Base Filter Constructor.
		 * @param frequency the operating frequency in Hz.
		 * @param samplerate the signal sample rate in samples/second.
		*/
		Filter(double frequency, double samplerate);

		/**
		 * @brief Process a sample of the signal.
		 * @param input the current sample.
		 * @return the current filter output.
		*/
		virtual double ProcessSample(double input) = 0;

		/**
		 * @brief Updates the sample rate of the signal to be filtered. 
				  In case the new sample rate is different from the current one
				  the filter should recalculate its constants and reset its state.
		 * @param samplerate the new sample rate in samples/second.
		*/
		void UpdateSamplerate(double samplerate);

		/**
		 * @brief Updates the operating frequency.
				  In case the new frequency is different from the current one
				  the filter should recalculate its constants.
		 * @param frequency the operating frequency in Hz.
		*/
		void UpdateFrequency(double frequency);

	protected:
		double mFrequency;
		double mSamplerate;

		inline virtual void CalculateConstants() = 0;

	};

	/**
	 * @brief Bank of filters connected in series.
	*/
	class FilterBank {
	public:
		/**
		 * @brief Adds a filter to the bank.
		 * @param filter the filter to be added.
		*/
		void AddFilter(std::shared_ptr<Filter>& filter) {
			filters.push_back(filter);
		}

		/**
		 * @brief Removes a filter from the bank at a fixed position.
		 * In case the position is illegal, the function does nothing.
		 * @param position the position of the filter to be removed.
		*/
		void RemoveFilterAt(int position) {
			if (position >= 0 && position < filters.size())
				filters.erase(filters.begin() + position);
		}

		/**
		 * @brief Process a sample of the signal through all the filters in the bank.
		 * @param input the current sample.
		 * @return the current filter output.
		*/
		double ProcessSample(double input) {
			double output = input;
			for (auto& filter : filters) {
				output = filter->ProcessSample(output);
			}
			return output;
		}

		/**
		 * @brief Updates the sample rate of all the filters in the bank.
		 * It just call UpdateSamplerate on each filter.
		 * @param samplerate the new sample rate in samples/second.
		*/
		void UpdateSamplerate(double samplerate) {
			for (auto& filter : filters) {
				filter->UpdateSamplerate(samplerate);
			}
		}

	private:
		std::vector<std::shared_ptr<Filter>> filters;
	};

	/**
	 * @brief Base class for bandpass/bandreject filters.
	*/
	class BandFilter : public Filter
	{
	public:
		/**
		 * @brief Constructor of a Band<pass-reject>filter.
		 * @param frequency the center frequency in Hz.
		 * @param bandwidth the bandwith in Hz.
		 * @param samplerate the operating sample rate.
		*/
		BandFilter(double frequency, double bandwidth, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
		virtual double ProcessSample(double input) = 0;

		/**
		 * @brief Updates the bandwidth of the filter.
		 * @param bandwidth the bandwidth in Hz.
		*/
		void UpdateBandwidth(double bandwidth);

	protected:
		/**
		 * @brief Bandwidth in Hz
		*/
		double mBandwidth;
	};

	/**
	 * @brief Parametric filter.
	 * 
	 * Bandwith is set at 3dB below gain for boost, 3dB above gain for cut.
	* When gain is below 3dB, bandwith is set at the aritmethic media between 0dB and gain.
	* 
	 * Reference gain G0 is fixed at 0dB.
	* 
	 * @see <a href="http://eceweb1.rutgers.edu/~orfanidi/intro2sp/2e/">
		Sophocles Orfanidis - Introduction to Signal Processing - Second Edition section 12.4
		</a>
	*/
	class ParametricFilter : public BandFilter
	{
	public:
		/**
		 * @brief Creates a Parametric filter.
		 * @param frequency the 3dB cut/boost frequency.
		 * @param bandwidth the bandwidth in Hz.
		 * @param gain the boost/cut gain in dB.
		 * @param samplerate the operating sample rate.
		*/
		ParametricFilter(double frequency, double bandwidth, DB gain, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
		double ProcessSample(double input) override;

		/**
		 * @brief Updates the boost/cut gain in dBs.
		 * @param gain the boost/cut gain in dBs.
		*/
		void UpdateGain(DB gain);

	private:
		// Filter state
		double w0 = .0;
		double w1 = .0;
		double w2 = .0;
		// Filter constants: Should be calculated at construction time and on parameters update.
		double b0, b1, b2, a1, a2;

		// Gain in dBs
		DB mGain;

		inline void CalculateConstants() override;

		/** Calculates the beta factor.
		*/
		double CalculateBeta(double centerGain, double referenceGain, double bw);

	};

	/**
	 * @brief Low pass shelving filter.
	 * 
	 * @see <a href="http://eceweb1.rutgers.edu/~orfanidi/intro2sp/2e/">
		Sophocles Orfanidis - Introduction to Signal Processing - Second Edition section 12.4.1
		</a>
	*/
	class LowPassShelvingFilter : public Filter
	{
	public:
		/**
		 * @brief Creates a Low pass shelving filter
		 * @param frequency the cutoff frequency.
		 * @param gain the shelf boost/cut gain.
		 * @param samplerate the operating sample rate.
		*/
		LowPassShelvingFilter(double frequency, DB gain, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
		double ProcessSample(double input) override;

		/**
		 * @brief Updates the shelf boost/cut gain in dBs.
		 * @param gain the shelf boost/cut gain in dBs.
		*/
		void UpdateGain(DB gain);

	protected:
		inline void CalculateConstants() override;

	private:
		// Filter state
		double w0 = .0;
		double w1 = .0;

		// Filter constants: Should be calculated at construction time and on parameters update.
		double b0, b1, a1;

		// Gain in dBs
		DB mGain;

		/** Calculates the beta factor.
*/
		double CalculateBeta(double centerGain, double referenceGain, double cutBoostFreq);

	};

	/**
	 * @brief Hi pass shelving filter.
	 *
	 * @see <a href="http://eceweb1.rutgers.edu/~orfanidi/intro2sp/2e/">
		Sophocles Orfanidis - Introduction to Signal Processing - Second Edition section 12.4.1
		</a>
	 * 
	*/
	class HiPassShelvingFilter : public Filter
	{
	public:
		/**
		 * @brief Creates a Hi pass shelving filter
		 * @param frequency the cutoff frequency.
		 * @param gain the shelf boost/cut gain.
		 * @param samplerate the operating sample rate.
		*/
		HiPassShelvingFilter(double frequency, DB gain, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
		double ProcessSample(double input) override;

		/**
		 * @brief Updates the shelf boost/cut gain in dBs.
		 * @param gain the shelf boost/cut gain in dBs.
		*/
		void UpdateGain(DB gain);

	protected:
		inline void CalculateConstants() override;

	private:
		// Filter state
		double w0 = .0;
		double w1 = .0;

		// Filter constants: Should be calculated at construction time and on parameters update.
		double b0, b1, a1;

		// Gain in dBs
		DB mGain;

		/** Calculates the beta factor.
		*/
		double CalculateBeta(double centerGain, double referenceGain, double cutBoostFreq);

	};

	/**
	 * @brief Simple DC Blocking filter.
	*/
	class DCBlocker : public Filter
	{
	public:
		/**
		 * @brief Creates a DCBlocker filter.
		 * @param frequency the cutoff frequency.
		 * @param samplerate the signal sample rate in samples/second.
		*/
		DCBlocker(double frequency, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
		double ProcessSample(double input) override;

	private:
		double lastInput = .0;
		double lastOutput = .0;
		double R;

		inline void CalculateConstants() override;
	};

	/**
	 * @brief Single pole low pass filter.
	*/
	class SinglePoleLowPass : public Filter
	{
	public:
		/**
		 * @brief Creates a single pole low pass filter.
		 * @param frequency the cutoff frequency.
		 * @param samplerate the signal sample rate in samples/second.
		*/
		SinglePoleLowPass(double frequency, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
		double ProcessSample(double input) override;

	private:
		double lastOutput = .0;
		double a0, b1;

		inline void CalculateConstants() override;
	};

	/**
	 * @brief Single pole hi pass filter.
	*/
	class SinglePoleHiPass : public Filter
	{
	public:
		/**
		 * @brief Creates a single pole hi pass filter.
		 * @param frequency the cutoff frequency.
		 * @param samplerate the signal sample rate in samples/second.
		*/
		SinglePoleHiPass(double frequency, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
		double ProcessSample(double input) override;

	private:
		double lastInput = .0;
		double lastOutput = .0;
		double a0, a1, b1;

		inline void CalculateConstants() override;
	};

	/**
	 * @brief Band pass filter.
	*/
	class BandPassFilter : public BandFilter
	{
	public:
		/**
		 * @brief Creates a band pass filter.
		 * @param frequency the center frequency in Hz.
		 * @param bandwidth the bandwidth in Hz
		 * @param samplerate the signal sample rate in samples/second.
		*/
		BandPassFilter(double frequency, double bandwidth, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
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

	/**
	 * @brief Band reject filter.
	*/
	class BandRejectFilter : public BandFilter
	{
	public:
		/**
		 * @brief Creates a band reject filter.
		 * @param frequency the center frequency in Hz.
		 * @param bandwidth the bandwidth in Hz
		 * @param samplerate the signal sample rate in samples/second.
		*/
		BandRejectFilter(double frequency, double bandwidth, double samplerate);

		/**
		 * @copydoc Filter::ProcessSample()
		*/
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