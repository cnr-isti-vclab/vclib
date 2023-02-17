/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "histogram.h"

#include <cmath>

namespace vcl {

template<typename ScalarType>
Histogram<ScalarType>::Histogram()
{

}

/**
 * @brief Set the histogram values.
 *
 * This method is used to correctly initialize the bins of the histogram.
 * n is the number of valid intervals between minv and maxv.
 * for a more robust working, the Histogram class stores also the two out of range intervals (-inf,
 * minv] and [maxv, +inf). Each bin is left closed (eg it contains the value). The \a gamma
 * parameter is applied to modify the distribution of the ranges of the bins. Default uniform
 * distibution.
 */
template<typename ScalarType>
Histogram<ScalarType>::Histogram(
	ScalarType minValue,
	ScalarType maxValue,
	uint       nIntervals,
	ScalarType gamma) :
		minVal(minValue), maxVal(maxValue), nIntervals(nIntervals)
{
	H.resize(nIntervals + 2, 0);

	R.resize(nIntervals + 3);

	R[0]              = -std::numeric_limits<ScalarType>::max();
	R[nIntervals + 2] = std::numeric_limits<ScalarType>::max();

	double delta = (maxValue - minVal);
	if (gamma == 1) {
		for (int i = 0; i <= nIntervals; ++i)
			R[i + 1] = minValue + delta * ScalarType(i) / nIntervals;
	}
	else {
		for (int i = 0; i <= nIntervals; ++i)
			R[i + 1] = minValue + delta * std::pow(ScalarType(i) / nIntervals, gamma);
	}
}

/**
 * @brief Clears the histogram.
 */
template<typename ScalarType>
void Histogram<ScalarType>::clear()
{
	H.clear();
	R.clear();
	cnt        = 0;
	sum        = 0;
	rms        = 0;
	nIntervals = 0;
	minVal     = 0;
	maxVal     = 1;
	minElem    = std::numeric_limits<ScalarType>::max();
	maxElem    = std::numeric_limits<ScalarType>::lowest();
}

/**
 * @brief Add a new value to the histogram.
 *
 * The statistics related to the histogram data (sum, RMS, etc.) are
 * also updated.
 */
template<typename ScalarType>
void Histogram<ScalarType>::addValue(ScalarType value, ScalarType increment)
{
	uint pos = BinIndex(value);
	if (value < minElem)
		minElem = value;
	if (value > maxElem)
		maxElem = value;
	assert((pos >= 0) && (pos <= nIntervals + 1));
	H[pos] += increment;
	cnt += increment;
	sum += value * increment;
	rms += (value * value) * increment;
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binCountInd(uint ind) const
{
	return H[ind];
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binCount(ScalarType value) const
{
	return H[binIndex(value)];
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binCount(ScalarType value, ScalarType width) const
{
	return rangeCount(value - width / 2.0, value + width / 2.0);
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::rangeCount(ScalarType rangeMin, ScalarType rangeMax) const
{
	uint firstBin = binIndex(rangeMin);
	uint lastBin  = binIndex(rangeMax);

	ScalarType sum = 0;
	for (uint i = firstBin; i <= lastBin; ++i)
		sum += H[i];
	return sum;
}

/**
 * @brief Returns the index of the bin which contains a given value.
 */
template<typename ScalarType>
uint Histogram<ScalarType>::binIndex(ScalarType val) const
{
	// lower_bound returns the furthermost iterator i in [first, last) such that, for every iterator
	// j in [first, i), *j < value. E.g. An iterator pointing to the first element "not less than"
	// val, or end() if every element is less than val.
	typename std::vector<ScalarType>::iterator it = std::lower_bound(R.begin(),R.end(), val);

	assert(it != R.begin());
	assert(it != R.end());

	uint pos = it - R.begin();
	pos -= 1;

	assert (R[pos] < val);
	assert (val <= R[pos+1] );
	return pos;
}

} // namespace vcl
