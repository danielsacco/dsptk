#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <algorithm>
#include "dsptk/filters.h"
#include "dsptk/constants.h"
#include "dsptk/signals.h"

namespace filters {

	// Test Values
	const double sampleRate = 1000.;

	namespace dcblocker {

		TEST(DCBlocker, WhenZeroInputShouldZeroOutput) {
			dsptk::DCBlocker sut(20., sampleRate);

			std::vector<double> input(10, 0.);
			std::vector<double> output(input.size());

			std::transform(input.begin(), input.end(), output.begin(), 
				[&](double in) {
					return sut.ProcessSample(in); 
				}
			);

			EXPECT_EQ(output, input);
		}

		TEST(DCBlocker, WhenStepInputShouldDropOutTowardsZero) {
			dsptk::DCBlocker sut(100., sampleRate);

			std::vector<double> input(10, 1.);
			std::vector<double> output(input.size());

			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_EQ(output[0], input[0]);

			// Expect a decaying curve o(n) < o(n-1)
			double lastValue = output[0];
			for (int i = 1; i < output.size(); i++) {
				EXPECT_LT(output[i], lastValue);
				lastValue = output[i];
			}

			// The last value should be close to zero
			EXPECT_NEAR(output[output.size() - 1], 0., 0.001);
		}

		TEST(DCBlocker, WhenDCBiasedInputShouldReduceDCOutput) {
			dsptk::DCBlocker sut(1., sampleRate);

			std::vector<double> input(10000);
			for (int i = 0; i < input.size(); i++) {
				switch (i % 4) {
				case 0:
				case 2:
					input[i] = 0.;
					break;
				case 1: 
					input[i] = 1.5;
					break;
				case 3:
					input[i] = -.5;
					break;
				}
			}

			std::vector<double> output(input.size());

			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			// Expect the last 20 samples DC value near zero
			double DCValue = 0.;
			for (int i = output.size() -1; i > output.size() - 21; i--) {
				DCValue += output[i];
			}

			EXPECT_NEAR(0., DCValue, 1e-10);
		}
	}

	namespace single_pole_lowpass {
		
		TEST(SinglePoleLowPass, WhenZeroInputShouldZeroOutput) {
			dsptk::SinglePoleLowPass sut(200., sampleRate);

			std::vector<double> input(10, 0.);
			std::vector<double> output(input.size());

			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_EQ(output, input);
		}

		TEST(SinglePoleLowPass, WhenStepInputShouldStepFromZeroToOne) {
			dsptk::SinglePoleLowPass sut(10., sampleRate);

			std::vector<double> input(1000, 1.);
			std::vector<double> output(input.size());

			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			// The first value should be close to zero
			EXPECT_NEAR(output[0], 0., 0.1);

			// The last value should be close to one
			EXPECT_NEAR(output[output.size() - 1], 1., 0.001);
		}

		TEST(SinglePoleLowPass, WhenFreqBelowFcShouldNotReduce) {
			double fc = 400.;
			double fTest = 20.;

			dsptk::SinglePoleLowPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			// Process through filter to obtain the output
			std::vector<double> output(input.size());
			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input), .001);

		}

		TEST(SinglePoleLowPass, WhenFreqIsFcShouldReduceAbout3dB) {
			double fc = 100.;
			double fTest = 100.;

			dsptk::SinglePoleLowPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			// Process through filter to obtain the output
			std::vector<double> output(input.size());
			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input)/2., .01);

		}

		TEST(SinglePoleLowPass, WhenFreqAboveFcShouldReduceMoreThan3dB) {
			double fc = 100.;
			double fTest = 200.;

			dsptk::SinglePoleLowPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);
			
			// Process through filter to obtain the output
			std::vector<double> output(input.size());
			std::transform(input.begin(), input.end(), output.begin(),
				[&](auto in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_LT(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2.);

		}

	}

	namespace single_pole_hipass {
		TEST(SinglePoleHiPass, WhenZeroInputShouldZeroOutput) {
			dsptk::SinglePoleHiPass sut(200., sampleRate);

			std::vector<double> input(10, 0.);
			std::vector<double> output(input.size());

			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_EQ(output, input);
		}

		TEST(SinglePoleHiPass, WhenStepInputShouldStepFromOneToZero) {
			dsptk::SinglePoleHiPass sut(10., sampleRate);

			std::vector<double> input(1000, 1.);
			std::vector<double> output(input.size());

			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			// The first value should be close to one
			EXPECT_NEAR(output[0], 1., 0.1);

			// The last value should be close to zero
			EXPECT_NEAR(output[output.size() - 1], 0., 0.001);
		}

		TEST(SinglePoleHiPass, WhenFreqAboveFcShouldNotReduce) {
			double fc = 20.;
			double fTest = 400.;

			dsptk::SinglePoleHiPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			// Process through filter to obtain the output
			std::vector<double> output(input.size());
			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input), .001);

		}

		TEST(SinglePoleHiPass, WhenFreqIsFcShouldReduceAbout3dB) {
			double fc = 100.;
			double fTest = 100.;

			dsptk::SinglePoleHiPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			// Process through filter to obtain the output
			std::vector<double> output(input.size());
			std::transform(input.begin(), input.end(), output.begin(),
				[&](double in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2., .02);

		}

		TEST(SinglePoleHiPass, WhenFreqBelowFcShouldReduceMoreThan3dB) {
			double fc = 200.;
			double fTest = 100.;

			dsptk::SinglePoleHiPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			// Process through filter to obtain the output
			std::vector<double> output(input.size());
			std::transform(input.begin(), input.end(), output.begin(),
				[&](auto in) {
					return sut.ProcessSample(in);
				}
			);

			EXPECT_LT(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2.);

		}


	}
}