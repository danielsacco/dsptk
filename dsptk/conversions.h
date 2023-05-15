/*
 ==============================================================================

 Contains part of the iPlug 2 library.

 ==============================================================================
*/

#pragma once

#include "cmath"

/** @brief Magic number for gain to dB conversion.
 * Approximates \f$ 20*log_{10}(x) \f$
 * @see AmpToDB
*/
static const double AMP_DB = 8.685889638065036553;
/** @brief Magic number for dB to gain conversion.
 * Approximates \f$ 10^{\frac{x}{20}} \f$
 * @see DBToAmp
*/
static const double IAMP_DB = 0.11512925464970;

/** @brief Calculates gain from a given dB value
 * @param dB Value in dB
 * @return Gain calculated as an approximation of
 * \f$ 10^{\frac{x}{20}} \f$
 * @see #IAMP_DB
 */
static inline double DBToAmp(double dB)
{
	return std::exp(IAMP_DB * dB);
}

/** @return dB calculated as an approximation of
 * \f$ 20*log_{10}(x) \f$
 * @see #AMP_DB */
static inline double AmpToDB(double amp)
{
	return AMP_DB * std::log(std::fabs(amp));
}

