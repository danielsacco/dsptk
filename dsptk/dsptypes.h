#pragma once

#include "cmath"
#include "conversions.h"

namespace dsptk {

	class DB {
	public:
		DB() {}
		DB(double dBValue) : value(dBValue) {}

		static DB fromLinearGain(const double linearGain) {
			return DB(20. * std::log10(std::fabs(linearGain)));
		}

		static DB fromPowerGain(const double powerGain) {
			return DB(10. * std::log10(std::fabs(powerGain)));
		}

		double asDB() {
			return value;
		}

		double asLinearGain() {
			return std::pow(10., value / 20.);
		}

		double asPowerGain() {
			return std::pow(10., value / 10.);
		}

		// Required for the _dB literal to work with negative values
		DB operator-() const {
			return DB(-value);
		}

		bool operator==(const DB& rhs) const { return value == rhs.value; }

		bool operator!=(const DB& rhs) const { return value != rhs.value; }

	private:
		double value = 0.;
	};


}



