#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "dsptk/detector.h"


namespace detector {
	namespace decoupled {

		// Test Values
		const double sampleRate = 1000.;
		const double attackTime = 0.1;
		const double releaseTime = 1.0;
		const int attackSamples = (int)std::round(sampleRate * attackTime);
		const int releaseSamples = (int)std::round(sampleRate * releaseTime);
		const double sampleValue = 100.;
		const double tenPercentOfSample = sampleValue * .1;
		const double ninetyPercentOfSample = sampleValue * .9;

		// 0.1% error in detector's output value
		const double expectedDetectorError = sampleValue * .1 / 100.;

		TEST(DecoupledPeakDetectorOperation, DetectorShoulInitInZero) {
			dsptk::DecoupledPeakDetector sut{ sampleRate, attackTime, releaseTime };

			EXPECT_EQ(.0, sut.ProcessSample(0));

		}

		TEST(DecoupledPeakDetectorOperation, DetectorJumpsFrom10To90InAttackTime) {
			dsptk::DecoupledPeakDetector sut{ sampleRate, attackTime, releaseTime };

			// Fill detector around 10% of the sampleValue
			for (int i = 0; i < attackSamples * 10; i++) {
				sut.ProcessSample(tenPercentOfSample);
			}
			ASSERT_NEAR(sut.ProcessSample(tenPercentOfSample), tenPercentOfSample, tenPercentOfSample * .01);

			// With the detector preloaded at 10% feed it with the whole sample value
			// during Attack Time.
			for (int i = 0; i < attackSamples -1; i++) {
				sut.ProcessSample(sampleValue);
			}

			ASSERT_NEAR(sut.ProcessSample(sampleValue), sampleValue * .9 , expectedDetectorError);
		}

		TEST(DecoupledPeakDetectorOperation, DetectorShouldRectifySignal) {
			dsptk::DecoupledPeakDetector sut{ sampleRate, attackTime, releaseTime };

			// Fill detector around 10% of the sampleValue
			for (int i = 0; i < attackSamples * 10; i++) {
				sut.ProcessSample(tenPercentOfSample);
			}
			ASSERT_NEAR(sut.ProcessSample(tenPercentOfSample), tenPercentOfSample, tenPercentOfSample * .01);

			// With the detector preloaded at 10% feed it with the whole sample value
			// during Attack Time.
			for (int i = 0; i < attackSamples - 1; i++) {
				sut.ProcessSample(i % 2 ? sampleValue : -sampleValue);
			}

			ASSERT_NEAR(sut.ProcessSample(100.), sampleValue * .9, expectedDetectorError);
		}

		TEST(DecoupledPeakDetectorOperation, DetectorReturnsAround10InReleaseTime) {
			// Use a fast attackTime so we get around 90 % before releasing
			dsptk::DecoupledPeakDetector sut{ sampleRate, attackTime/10., releaseTime };

			// Fill detector around 90% of the sampleValue
			for (int i = 0; i < attackSamples; i++) {
				sut.ProcessSample(ninetyPercentOfSample);
			}
			ASSERT_NEAR(sut.ProcessSample(ninetyPercentOfSample), ninetyPercentOfSample, ninetyPercentOfSample * .01);

			// With the detector preloaded at 90% feed it with 0 during Release Time.
			for (int i = 0; i < releaseSamples -1; i++) {
				sut.ProcessSample(0.);
			}

			// Assert that releasing has arrived almost at 10%
			ASSERT_NEAR(sut.ProcessSample(0.), sampleValue * .1, expectedDetectorError);
		}

	}
}

