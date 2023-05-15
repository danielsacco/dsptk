#pragma once

#include "detector.h"

namespace dsptk {

    class GainReductionComputer {
    public:
        GainReductionComputer(double threshold, double ratio, double kneeWidth)
            : threshold(threshold), ratio(ratio), kneeWidth(kneeWidth)
        {
            CalculateKneeLimits();
            CalculateReductionFactor();
        };

        double Compute(double);

        void SetThreshold(double threshold);
        void SetRatio(double ratio);
        void SetKneeWidth(double kneeWidth);

    private:
        double threshold;
        double ratio;
        double kneeWidth;

        double kneeStart;
        double kneeEnd;
        double reductionFactor;

        void CalculateKneeLimits();
        void CalculateReductionFactor();
    };

    class Compressor {
    public:
        Compressor(double threshold, double ratio, double kneeWidth, double sampleRate, double attackMs, double releaseMs);

        void ProcessBlock(double* input, double* sidechain, double* output, double* grMeter, int nFrames);

        void SetSampleRate(double sampleRate);
        void SetAttackTime(double attackTime);
        void SetReleaseTime(double releaseTime);

        void SetThreshold(double threshold);
        void SetRatio(double ratio);
        void SetKneeWidth(double kneeWidth);

    private:
        DecoupledPeakDetector grDetector;
        GainReductionComputer reductionComputer;
    };

}	// End namespace dsptk

