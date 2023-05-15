#include "detector.h"

#include <algorithm>
#include <cmath>

namespace dsptk {

    double DecoupledPeakDetector::ProcessSample(double input)
    {
        const double x = std::abs(input);

        const double k = x > lastOutput ? attackFactor : releaseFactor;

        lastOutput += k * (x - lastOutput);

        if (isnan(lastOutput)) {
            lastOutput = 0.;
        }

        return lastOutput;
    }


    /*
    * Base detector class.
    */
    Detector::Detector(double sampleRate, double attackTime, double releaseTime)
        : sampleRate{ sampleRate }, attackTime{ attackTime }, releaseTime{ releaseTime }
    {
        calculateFactors();
    }

    void Detector::calculateFactors()
    {
        attackFactor = 1. - std::exp(-2.2 / (attackTime * sampleRate));     // Rise Time 10%/90%
        releaseFactor = 1. - std::exp(-2.2 / (releaseTime * sampleRate));   // Release Time 90%/10%
    }

    void Detector::setSampleRate(double sampleRate)
    {
        if (sampleRate != Detector::sampleRate)
        {
            Detector::sampleRate = sampleRate;
            calculateFactors();
        }
    }

    void Detector::setAttackTime(double attackTime)
    {
        if (attackTime != Detector::attackTime)
        {
            Detector::attackTime = attackTime;
            calculateFactors();
        }
    }

    void Detector::setReleaseTime(double releaseTime)
    {
        if (releaseTime != Detector::releaseTime)
        {
            Detector::releaseTime = releaseTime;
            calculateFactors();
        }
    }

}	// End namespace dsptk