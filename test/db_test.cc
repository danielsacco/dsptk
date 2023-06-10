#include <gtest/gtest.h>

#include <cmath>
#include "dsptk/dsptypes.h"

namespace db {

	namespace building {
		TEST(DirectConstruction, whenCreatedFromLiteralShouldWork) {
			dsptk::DB sut = 12.3_dB;
			EXPECT_EQ(sut.asDB(), 12.3);
		}

		TEST(DirectConstruction, whenCreatedFromNegativeLiteralShouldWork) {
			dsptk::DB sut = -34.55_dB;
			EXPECT_EQ(sut.asDB(), -34.55);
		}

		TEST(DirectConstruction, whenCreatedWithoutValueShouldWork) {
			dsptk::DB sut;
			EXPECT_EQ(sut.asDB(), -0.);
		}

		TEST(DirectConstruction, whenCreatedFromDBValueShouldWork) {
			dsptk::DB sut{-45.67};
			EXPECT_EQ(sut.asDB(), -45.67);
		}

		TEST(FromLinearBuilder, whenCreatedFromLinearGainShouldWork_0dB) {
			auto sut = dsptk::DB::fromLinearGain(1);
			EXPECT_EQ(sut.asDB(), 0.);
		}

		TEST(FromLinearBuilder, whenCreatedFromLinearGainShouldWork_20dBDown) {
			auto sut = dsptk::DB::fromLinearGain(0.1);
			EXPECT_NEAR(sut.asDB(), -20., .001);
		}

		TEST(FromLinearBuilder, whenCreatedFromLinearGainShouldWork_3dBDown) {
			auto sut = dsptk::DB::fromLinearGain(std::sqrt(0.5));
			EXPECT_NEAR(sut.asDB(), -3.01, .001);
		}

		TEST(FromLinearBuilder, whenCreatedFromLinearGainShouldWork_20dBUp) {
			auto sut = dsptk::DB::fromLinearGain(10.);
			EXPECT_NEAR(sut.asDB(), 20., .001);
		}

		TEST(FromLinearBuilder, whenCreatedFromLinearGainShouldWork_3dBUp) {
			auto sut = dsptk::DB::fromLinearGain(std::sqrt(2.));
			EXPECT_NEAR(sut.asDB(), 3.01, .001);
		}

		TEST(FromPowerBuilder, whenCreatedFromPowerGainShouldWork_0dB) {
			auto sut = dsptk::DB::fromPowerGain(1);
			EXPECT_EQ(sut.asDB(), 0.);
		}

		TEST(FromPowerBuilder, whenCreatedFromPowerGainShouldWork_20dBDown) {
			auto sut = dsptk::DB::fromPowerGain(0.01);
			EXPECT_NEAR(sut.asDB(), -20., .001);
		}

		TEST(FromPowerBuilder, whenCreatedFromPowerGainShouldWork_3dBDown) {
			auto sut = dsptk::DB::fromPowerGain(0.5);
			EXPECT_NEAR(sut.asDB(), -3.01, .001);
		}

		TEST(FromPowerBuilder, whenCreatedFromPowerGainShouldWork_20dBUp) {
			auto sut = dsptk::DB::fromPowerGain(100.);
			EXPECT_NEAR(sut.asDB(), 20., .001);
		}

		TEST(FromPowerBuilder, whenCreatedFromPowerGainShouldWork_3dBUp) {
			auto sut = dsptk::DB::fromPowerGain(2.);
			EXPECT_NEAR(sut.asDB(), 3.01, .001);
		}
	}

	namespace conversion {
		TEST(ToLinearGain, whenQueriedAsLinearGainShouldWork_0dB) {
			dsptk::DB sut = .0_dB;
			EXPECT_EQ(sut.asLinearGain(), 1.);
		}

		TEST(ToLinearGain, whenQueriedAsLinearGainShouldWork_3dBDown) {
			dsptk::DB sut = -3.01_dB;
			EXPECT_NEAR(sut.asLinearGain(), std::sqrt(.5), .001);
		}

		TEST(ToLinearGain, whenQueriedAsLinearGainShouldWork_20dBDown) {
			dsptk::DB sut = -20._dB;
			EXPECT_NEAR(sut.asLinearGain(), .1, .001);
		}

		TEST(ToLinearGain, whenQueriedAsLinearGainShouldWork_3dBUp) {
			dsptk::DB sut = 3.01_dB;
			EXPECT_NEAR(sut.asLinearGain(), std::sqrt(2.), .001);
		}

		TEST(ToLinearGain, whenQueriedAsLinearGainShouldWork_20dBUp) {
			dsptk::DB sut = 20._dB;
			EXPECT_NEAR(sut.asLinearGain(), 10., .001);
		}





		TEST(ToPowerGain, whenQueriedAsPowerGainShouldWork_0dB) {
			dsptk::DB sut = .0_dB;
			EXPECT_EQ(sut.asPowerGain(), 1.);
		}

		TEST(ToPowerGain, whenQueriedAsPowerGainShouldWork_3dBDown) {
			dsptk::DB sut = -3.01_dB;
			EXPECT_NEAR(sut.asPowerGain(), .5, .001);
		}

		TEST(ToPowerGain, whenQueriedAsPowerGainShouldWork_20dBDown) {
			dsptk::DB sut = -20._dB;
			EXPECT_NEAR(sut.asPowerGain(), .01, .001);
		}

		TEST(ToPowerGain, whenQueriedAsPowerGainShouldWork_3dBUp) {
			dsptk::DB sut = 3.01_dB;
			EXPECT_NEAR(sut.asPowerGain(), 2., .001);
		}

		TEST(ToPowerGain, whenQueriedAsPowerGainShouldWork_20dBUp) {
			dsptk::DB sut = 20._dB;
			EXPECT_NEAR(sut.asPowerGain(), 100., .001);
		}
	}
}