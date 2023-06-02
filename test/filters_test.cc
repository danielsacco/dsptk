#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <algorithm>
#include "dsptk/filters.h"
#include "dsptk/constants.h"
#include "dsptk/signals.h"

namespace filters {

	// Test Values
	const double sampleRate = 1000.;

	// Helper function to produce an output vector with the same size as the input
	// processed by a filter
	std::vector<double> ProduceOutput(const std::vector<double>& input, dsptk::Filter& filter) {
		std::vector<double> output(input.size());
		std::transform(input.begin(), input.end(), output.begin(),
			[&](double in) {
				return filter.ProcessSample(in);
			}
		);
		return output;
	}



	namespace dcblocker {

		TEST(DCBlocker, WhenZeroInputShouldZeroOutput) {
			dsptk::DCBlocker sut(20., sampleRate);

			std::vector<double> input(10, 0.);

			auto output = ProduceOutput(input, sut);

			EXPECT_EQ(output, input);
		}

		TEST(DCBlocker, WhenStepInputShouldDropOutTowardsZero) {
			dsptk::DCBlocker sut(100., sampleRate);

			std::vector<double> input(10, 1.);

			auto output = ProduceOutput(input, sut);

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

			auto output = ProduceOutput(input, sut);

			// Expect the last 20 samples DC value near zero
			double DCValue = 0.;
			for (auto i = output.size() -1; i > output.size() - 21; i--) {
				DCValue += output[i];
			}

			EXPECT_NEAR(0., DCValue, 1e-10);
		}
	}

	namespace single_pole_lowpass {
		
		TEST(SinglePoleLowPass, WhenZeroInputShouldZeroOutput) {
			dsptk::SinglePoleLowPass sut(200., sampleRate);

			std::vector<double> input(10, 0.);

			auto output = ProduceOutput(input, sut);

			EXPECT_EQ(output, input);
		}

		TEST(SinglePoleLowPass, WhenStepInputShouldStepFromZeroToOne) {
			dsptk::SinglePoleLowPass sut(10., sampleRate);

			std::vector<double> input(1000, 1.);

			auto output = ProduceOutput(input, sut);

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

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input), .001);

		}

		TEST(SinglePoleLowPass, WhenFreqIsFcShouldReduceAbout3dB) {
			double fc = 100.;
			double fTest = 100.;

			dsptk::SinglePoleLowPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input)/2., .01);

		}

		TEST(SinglePoleLowPass, WhenFreqAboveFcShouldReduceMoreThan3dB) {
			double fc = 100.;
			double fTest = 200.;

			dsptk::SinglePoleLowPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);
			
			auto output = ProduceOutput(input, sut);

			EXPECT_LT(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2.);

		}

	}

	namespace single_pole_hipass {
		TEST(SinglePoleHiPass, WhenZeroInputShouldZeroOutput) {
			dsptk::SinglePoleHiPass sut(200., sampleRate);

			std::vector<double> input(10, 0.);

			auto output = ProduceOutput(input, sut);

			EXPECT_EQ(output, input);
		}

		TEST(SinglePoleHiPass, WhenStepInputShouldStepFromOneToZero) {
			dsptk::SinglePoleHiPass sut(10., sampleRate);

			std::vector<double> input(1000, 1.);

			auto output = ProduceOutput(input, sut);

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

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input), .001);

		}

		TEST(SinglePoleHiPass, WhenFreqIsFcShouldReduceAbout3dB) {
			double fc = 100.;
			double fTest = 100.;

			dsptk::SinglePoleHiPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2., .02);

		}

		TEST(SinglePoleHiPass, WhenFreqBelowFcShouldReduceMoreThan3dB) {
			double fc = 200.;
			double fTest = 100.;

			dsptk::SinglePoleHiPass sut(fc, sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 1000);

			auto output = ProduceOutput(input, sut);

			EXPECT_LT(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2.);

		}


	}

	namespace bandpass {
		TEST(BandPassFilter, WhenZeroInputShouldZeroOutput) {
			dsptk::BandPassFilter sut(200., 10., sampleRate);

			std::vector<double> input(10, 0.);

			auto output = ProduceOutput(input, sut);

			EXPECT_EQ(output, input);
		}

		TEST(BandPassFilter, WhenFreqAboveFcShouldReduceNearZero) {
			double fc = 200.;
			double fTest = 450.;

			dsptk::BandPassFilter sut(fc, 10., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), 0., .0001);

		}

		TEST(BandPassFilter, WhenFreqBelowFcShouldReduceNearZero) {
			double fc = 300.;
			double fTest = 50.;

			dsptk::BandPassFilter sut(fc, 10., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), 0., .0001);

		}

		TEST(BandPassFilter, WhenFreqIsFcShouldNotReduce) {
			double fc = 250.;
			double fTest = 250.;

			dsptk::BandPassFilter sut(fc, 50., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input), .001);

		}

		TEST(BandPassFilter, WhenFreqIsAtBwShouldReduce3dB) {
			double fc = 250.;
			double fTest = 275.;

			dsptk::BandPassFilter sut(fc, 50., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2., .005);

		}

	}

	namespace bandreject {
		TEST(BandRejectFilter, WhenZeroInputShouldZeroOutput) {
			dsptk::BandRejectFilter sut(200., 10., sampleRate);

			std::vector<double> input(10, 0.);

			auto output = ProduceOutput(input, sut);

			EXPECT_EQ(output, input);
		}

		TEST(BandRejectFilter, WhenFreqAboveFcShouldNotReduce) {
			double fc = 200.;
			double fTest = 450.;

			dsptk::BandRejectFilter sut(fc, 10., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input), .0005);

		}

		TEST(BandRejectFilter, WhenFreqBelowFcShouldNotReduce) {
			double fc = 300.;
			double fTest = 50.;

			dsptk::BandRejectFilter sut(fc, 10., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input), .0001);

		}

		TEST(BandRejectFilter, WhenFreqIsFcShouldReduceNearZero) {
			double fc = 250.;
			double fTest = 250.;

			dsptk::BandRejectFilter sut(fc, 50., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), 0., .001);

		}

		TEST(BandRejectFilter, WhenFreqIsAtBwShouldReduce3dB) {
			double fc = 250.;
			double fTest = 275.;

			dsptk::BandRejectFilter sut(fc, 50., sampleRate);

			// Fill input with a sine signal at fTest
			auto input = dsptk::sin(fTest, sampleRate, 10000);

			auto output = ProduceOutput(input, sut);

			EXPECT_NEAR(dsptk::MeanSquare(output), dsptk::MeanSquare(input) / 2., .005);

		}

	}

	namespace parametric {
		TEST(ParametricFilter, WhenZeroInputShouldZeroOutput) {
			dsptk::ParametricFilter sut(200., 10., 0., sampleRate);

			std::vector<double> input(10, 0.);

			auto output = ProduceOutput(input, sut);

			EXPECT_EQ(output, input);
		}

		namespace boost9dB {
			TEST(ParametricFilterBoost9, WhenFreqIsFcShouldBoost) {
				double fc = 250.;
				double bw = 20.;
				double boostDB = 9.;

				dsptk::ParametricFilter sut(fc, bw, boostDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double resultBoost = 10 * std::log10(outputMs / inputMs);

				EXPECT_NEAR(resultBoost, boostDB, .01);
			}

			TEST(ParametricFilterBoost9, WhenFreqIsAtBwShouldBoost3dBLess) {
				double fc = 250.;
				double bw = 20.;
				double boostDB = 9.;

				dsptk::ParametricFilter sut(fc, bw, boostDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc - bw / 2.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double resultBoost = 10 * std::log10(outputMs / inputMs);

				EXPECT_NEAR(resultBoost, boostDB - 3., .05);
			}

			TEST(ParametricFilterBoost9, WhenFreqIsFarAwayFromFcShouldNotBoost) {
				double fc = 250.;
				double bw = 20.;
				double boostDB = 9.;

				dsptk::ParametricFilter sut(fc, bw, boostDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc + 100.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double resultBoost = 10 * std::log10(outputMs / inputMs);

				EXPECT_LT(resultBoost, 1.);
			}


		}

		namespace boost2dB {
			TEST(ParametricFilterBoost2, WhenFreqIsFcShouldBoost) {
				double fc = 250.;
				double bw = 20.;
				double boostDB = 2.;

				dsptk::ParametricFilter sut(fc, bw, boostDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double resultBoost = 10 * std::log10(outputMs / inputMs);

				EXPECT_NEAR(resultBoost, boostDB, .01);
			}

			TEST(ParametricFilterBoost2, WhenFreqIsAtBwShouldBoostLessThan2) {
				double fc = 250.;
				double bw = 20.;
				double boostDB = 2.;

				dsptk::ParametricFilter sut(fc, bw, boostDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc - bw / 2.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double resultBoost = 10 * std::log10(outputMs / inputMs);

				EXPECT_LT(resultBoost, boostDB);
			}

			TEST(ParametricFilterBoost2, WhenFreqIsFarAwayFromFcShouldNotBoost) {
				double fc = 250.;
				double bw = 20.;
				double boostDB = 2.;

				dsptk::ParametricFilter sut(fc, bw, boostDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc + 100.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double resultBoost = 10 * std::log10(outputMs / inputMs);

				EXPECT_LT(resultBoost, 0.1);
			}

		}

		namespace cut9dB {


			TEST(ParametricFilterCut9, WhenFreqIsFcShouldCut) {
				double fc = 250.;
				double bw = 20.;
				double cutDB = -9.;

				dsptk::ParametricFilter sut(fc, bw, cutDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double result = 10 * std::log10(outputMs / inputMs);

				EXPECT_NEAR(result, cutDB, .02);
			}

			TEST(ParametricFilterCut9, WhenFreqIsAtBwShouldCut3dBLess) {
				double fc = 250.;
				double bw = 20.;
				double cutDB = -9.;

				dsptk::ParametricFilter sut(fc, bw, cutDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc - bw / 2.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double result = 10 * std::log10(outputMs / inputMs);

				EXPECT_NEAR(result, cutDB + 3., .05);
			}

			TEST(ParametricFilterCut9, WhenFreqIsFarAwayFromFcShouldNotCut) {
				double fc = 250.;
				double bw = 20.;
				double cutDB = -9.;

				dsptk::ParametricFilter sut(fc, bw, cutDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc + 100.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double result = 10 * std::log10(outputMs / inputMs);

				EXPECT_LT(result, 1.);
			}


		}

		namespace cut2dB {
			TEST(ParametricFilterCut2, WhenFreqIsFcShouldCut) {
				double fc = 250.;
				double bw = 20.;
				double cutDB = -2.;

				dsptk::ParametricFilter sut(fc, bw, cutDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double result = 10 * std::log10(outputMs / inputMs);

				EXPECT_NEAR(result, cutDB, .01);
			}

			TEST(ParametricFilterCut2, WhenFreqIsAtBwShouldCutLessThan2) {
				double fc = 250.;
				double bw = 20.;
				double cutDB = -2.;

				dsptk::ParametricFilter sut(fc, bw, cutDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc - bw / 2.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double result = 10 * std::log10(outputMs / inputMs);

				EXPECT_GT(result, cutDB);
			}

			TEST(ParametricFilterCut2, WhenFreqIsFarAwayFromFcShouldNotCut) {
				double fc = 250.;
				double bw = 20.;
				double cutDB = -2.;

				dsptk::ParametricFilter sut(fc, bw, cutDB, sampleRate);

				// Fill input with a sine signal at fTest
				double fTest = fc + 100.;
				auto input = dsptk::sin(fTest, sampleRate, 10000);

				auto output = ProduceOutput(input, sut);

				double inputMs = dsptk::MeanSquare(input);
				double outputMs = dsptk::MeanSquare(output);
				double result = 10 * std::log10(outputMs / inputMs);

				EXPECT_LT(result, 0.1);
			}

		}

	}
}