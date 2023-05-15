#include <vector>
#include "dynamics.h"
#include "conversions.h"

namespace dsptk {


    double GainReductionComputer::Compute(double sample) {

        double gainReduction = 0.;

        if (sample <= kneeStart) {
            gainReduction = 0;                             // No reduction
        }
        else if (sample < kneeEnd) {
            // Quadratic interpolation for gain reduction
            double factor = (sample - kneeStart) / kneeWidth;
            factor *= factor;
            double delta = sample - kneeStart;
            gainReduction = factor * delta * reductionFactor;   // Gain Reduction in dBs
        }
        else {
            // Normal gain reduction
            double delta = sample - threshold;
            gainReduction = delta * reductionFactor;   // Gain Reduction in dBs
        }

        return gainReduction;
    }

    void GainReductionComputer::SetThreshold(double threshold)
    {
        GainReductionComputer::threshold = threshold;
        CalculateKneeLimits();
    }

    void GainReductionComputer::SetRatio(double ratio)
    {
        GainReductionComputer::ratio = ratio;
        CalculateReductionFactor();
    }

    void GainReductionComputer::SetKneeWidth(double kneeWidth)
    {
        GainReductionComputer::kneeWidth = kneeWidth;
        CalculateKneeLimits();
    }

    void GainReductionComputer::CalculateKneeLimits()
    {
        kneeStart = threshold - kneeWidth / 2.;
        kneeEnd = threshold + kneeWidth / 2.;
    }

    void GainReductionComputer::CalculateReductionFactor()
    {
        reductionFactor = (1. - ratio) / ratio;
    }


    Compressor::Compressor(double threshold, double ratio, double kneeWidth, double sampleRate, double attackMs, double releaseMs)
        : grDetector{ sampleRate, attackMs, releaseMs }
        , reductionComputer{ threshold, ratio, kneeWidth }
    {
    }

    void Compressor::ProcessBlock(double* input, double* sidechain, double* output, double* vcaGain, int nFrames)
    {
        std::vector<double> localBuffer(nFrames);

        // Log of control (sidechain or input) signal
        double* controlSignal = sidechain ? sidechain : input;
        for (int s = 0; s < nFrames; s++) {
            localBuffer[s] = AmpToDB(controlSignal[s]);
        }

        // Pass log of control signal through gain curve
        // Here we have the control signal converted to dBs between -infinite and zero (or greater)
        for (int s = 0; s < nFrames; s++) {
            vcaGain[s] = reductionComputer.Compute(localBuffer[s]);
        }

        // Back to linear for feeding the detector
        for (int s = 0; s < nFrames; s++) {
            vcaGain[s] = DBToAmp(vcaGain[s]);
        }

        // Attack/Release post gain curve
        for (int s = 0; s < nFrames; s++) {
            // Here we have a gain factor between 0dB and -inf, so we need to invert the input to the detector and its output.
            vcaGain[s] = 1. - grDetector.ProcessSample(1. - vcaGain[s]);
        }

        // Apply the gain profile
        for (int s = 0; s < nFrames; s++) {
            output[s] = input[s] * vcaGain[s];
        }

    }

    void Compressor::SetSampleRate(double sampleRate)
    {
        grDetector.setSampleRate(sampleRate);
    }

    void Compressor::SetAttackTime(double attackMs)
    {
        grDetector.setAttackTime(attackMs);
    }

    void Compressor::SetReleaseTime(double releaseMs)
    {
        grDetector.setReleaseTime(releaseMs);
    }

    void Compressor::SetThreshold(double threshold)
    {
        reductionComputer.SetThreshold(threshold);
    }

    void Compressor::SetRatio(double ratio)
    {
        reductionComputer.SetRatio(ratio);
    }

    void Compressor::SetKneeWidth(double kneeWidth)
    {
        reductionComputer.SetKneeWidth(kneeWidth);
    }

}	// End namespace dsptk