#pragma once

namespace dsptk {

	// From C++20 use std::numbers::pi_v instead of this. 
	template <typename T>
	constexpr T PI = T(3.1415926535897932384626433L);

	// From C++20 use std::numbers::pi_v * 2 instead of this.
	template <typename T>
	constexpr T DOUBLE_PI = T(2. * PI<T>);

}