#pragma once

namespace dsptk {

	/** 
	* \brief Base definition for a signal detector. Peak and RMS detectors should derive from this class.
	*/
	class Detector {
	public:
		/**
		* \brief Creates a signal detector.
		* 
		* \param sampleRate the signal sample rate expressed as samples per second.
		* \param attackTime the attack time expressed in seconds.
		* \param releaseTime the release time expressed in seconds.
		*/
		Detector(double sampleRate, double attackTime, double releaseTime);

		/**attackTime
		* \brief Process a signal sample.
		* 
		* \param input the next sample to be processed.
		* \return the detector output after processing the sample.
		*/
		virtual double ProcessSample(double input) = 0;

		void setSampleRate(double sampleRate);
		void setAttackTime(double attackTime);
		void setReleaseTime(double releaseTime);

	protected:
		void calculateFactors();

		double sampleRate;
		double attackTime;
		double releaseTime;
		double attackFactor = .9;
		double releaseFactor = .8;

	};

	/**
	* /brief Decoupled Peak Detector.
	* 
	* Decoupled Peak Detector implementation as described in
	* http://c4dm.eecs.qmul.ac.uk/audioengineering/compressors/documents/report.pdf Massberg/Reiss pages 30 - 32
	* Udo Zolzer DAFX 2nd Ed. page 230
	*/
	class DecoupledPeakDetector : virtual public Detector {

	public:
		using Detector::Detector;
		DecoupledPeakDetector(double sampleRate, double attackTime, double releaseTime) : Detector(sampleRate, attackTime, releaseTime) {};
		virtual double ProcessSample(double input) override;

	private:
		double lastOutput = .0;
	};

}	// End namespace dsptk