#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>

#include "dsptk/convolution.h"


namespace convolution {
	namespace inputAlgorithm {
		TEST(Convolution, ResultSizeIsCorrect) {

			std::vector<double> input { 1., 0., 0., 0., 0. };
			std::vector<double> kernel { 1., 0., 0. };

			std::vector<double> result = dsptk::convolve(input, kernel);
			EXPECT_EQ(kernel.size() + input.size() - 1, result.size());
		}

		TEST(Convolution, ImpulseImpulseResult) {

			std::vector<double> input{ 1., 0., 0., 0., 0. };
			std::vector<double> kernel{ 1., 0., 0. };

			// First sample in result should be 1, the rest 0
			std::vector<double> expectedResult(input.size() + kernel.size() - 1, 0.);
			expectedResult[0] = 1.;

			std::vector<double> result = dsptk::convolve(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}

		TEST(Convolution, StepImpulseResult) {

			std::vector<double> input{ 1., 0., 0., 0. };
			std::vector<double> kernel{ 1., 1., 1. };

			// First 3 samples in result should be 1, the rest 0
			std::vector<double> expectedResult{ 1., 1., 1., 0., 0., 0. };

			std::vector<double> result = dsptk::convolve(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}

		TEST(Convolution, ImpulseShiftResult) {

			std::vector<double> input{ 1., 0., 0., 0. };
			std::vector<double> kernel{ 0., 0., 1. };

			// 3rd sample should be one
			std::vector<double> expectedResult{ 0., 0., 1., 0., 0., 0. };

			std::vector<double> result = dsptk::convolve(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}

		TEST(Convolution, AverageFilter) {

			std::vector<double> input{ 1., 2., 1., 2., 1., 2., 1., 2., 1., 2., 1., 2.};
			std::vector<double> kernel{ .25, .25, .25, .25};

			// First and last 3 samples are useless, the rest should be 1.5 (Average between 1 and 2)
			std::vector<double> expectedResult{ 0.25, 0.75, 1, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.25, 0.75, 0.5 };

			std::vector<double> result = dsptk::convolve(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}


		TEST(Convolution, FirstDifference) {

			std::vector<double> input{ 1., 0., 2., 1., 0., 0., 1., 0., 1., 0., 0., 0.};
			std::vector<double> kernel{ 1., -1., 0., 0.};

			std::vector<double> expectedResult{ 1., -1., 2., -1., -1., 0., 1., -1., 1., -1., 0., 0., 0., 0., 0. };

			std::vector<double> result = dsptk::convolve(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}

		TEST(Convolution, InputSizeShouldBeGreaterThanZero) {
			// TODO
		}

		TEST(Convolution, KernelSizeShouldBeGreaterThanZero) {
			// TODO
		}
	}

	namespace outputAlgorithm {
		TEST(ConvolutionOut, ResultSizeIsCorrect) {

			std::vector<double> input { 1., 0., 0., 0., 0. };
			std::vector<double> kernel { 1., 0., 0. };

			std::vector<double> result = dsptk::convolve_out(input, kernel);
			EXPECT_EQ(kernel.size() + input.size() - 1, result.size());
		}

		TEST(ConvolutionOut, ImpulseImpulseResult) {

			std::vector<double> input{ 1., 0., 0., 0., 0. };
			std::vector<double> kernel{ 1., 0., 0. };

			// First sample in result should be 1, the rest 0
			std::vector<double> expectedResult(input.size() + kernel.size() - 1, 0.);
			expectedResult[0] = 1.;

			std::vector<double> result = dsptk::convolve_out(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}

		TEST(ConvolutionOut, StepImpulseResult) {

			std::vector<double> input{ 1., 0., 0., 0. };
			std::vector<double> kernel{ 1., 1., 1. };

			// First 3 samples in result should be 1, the rest 0
			std::vector<double> expectedResult{ 1., 1., 1., 0., 0., 0. };

			std::vector<double> result = dsptk::convolve_out(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}

		TEST(ConvolutionOut, ImpulseShiftResult) {

			std::vector<double> input{ 1., 0., 0., 0. };
			std::vector<double> kernel{ 0., 0., 1. };

			// 3rd sample should be one
			std::vector<double> expectedResult{ 0., 0., 1., 0., 0., 0. };

			std::vector<double> result = dsptk::convolve_out(input, kernel);
			EXPECT_EQ(expectedResult, result);
		}

	}

}

