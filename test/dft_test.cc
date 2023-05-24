#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <array>

#include "dsptk/dft.h"

namespace dft {
	TEST(Dft, ResultSizeIsCorrect) {

		int inputSize = 16;
		std::vector<double> input(inputSize, 0.);
		int expectedResultSize = 1 + inputSize / 2;

		std::array<std::vector<double>, 2> result = dsptk::real_dft_analysis(input);

		std::vector<double> reX = result[0];
		EXPECT_EQ(reX.size(), expectedResultSize);

		std::vector<double> imX = result[1];
		EXPECT_EQ(imX.size(), expectedResultSize);
	}

	TEST(Dft, UnitStepSpectrum) {

		std::vector<double> input{1., 1., 1., 1., 1., 1., 1., 1.};
		std::vector<double> expectedResult{8., 0., 0., 0., 0.};

		std::array<std::vector<double>, 2> result = dsptk::real_dft_analysis(input);

		std::vector<double> reX = result[0];
		EXPECT_EQ(reX, expectedResult);

	}

	TEST(Dft, ImpulseSpectrum) {

		std::vector<double> input{1., 0., 0., 0., 0., 0., 0., 0.};
		std::vector<double> expectedResult{1., 1., 1., 1., 1.};

		std::array<std::vector<double>, 2> result = dsptk::real_dft_analysis(input);

		std::vector<double> reX = result[0];
		EXPECT_EQ(reX, expectedResult);

	}

	TEST(Dft, FMaxSpectrum) {

		std::vector<double> input{1., -1., 1., -1., 1., -1., 1., -1.};
		std::vector<double> expectedResult{0., 0., 0., 0., 8.};

		std::array<std::vector<double>, 2> result = dsptk::real_dft_analysis(input);

		std::vector<double> reX = result[0];
		EXPECT_EQ(reX, expectedResult);
	}
}