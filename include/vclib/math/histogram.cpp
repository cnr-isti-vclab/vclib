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
	ScalarType minRangeValue,
	ScalarType maxRangeValue,
	uint       nBins,
	ScalarType gamma) :
		minRangeVal(minRangeValue), maxRangeVal(maxRangeValue), nBins(nBins)
{
	H.resize(nBins + 2, 0);

	R.resize(nBins + 3);

	R[0]              = -std::numeric_limits<ScalarType>::max();
	R[nBins + 2] = std::numeric_limits<ScalarType>::max();

	double delta = (maxRangeVal - minRangeVal);
	if (gamma == 1) {
		for (uint i = 0; i <= nBins; ++i)
			R[i + 1] = minRangeVal + delta * ScalarType(i) / nBins;
	}
	else {
		for (uint i = 0; i <= nBins; ++i)
			R[i + 1] = minRangeVal + delta * std::pow(ScalarType(i) / nBins, gamma);
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
	cnt         = 0;
	sum         = 0;
	rms         = 0;
	nBins       = 0;
	minRangeVal = 0;
	maxRangeVal = 1;
	minVal      = std::numeric_limits<ScalarType>::max();
	maxVal      = std::numeric_limits<ScalarType>::lowest();
}

/**
 * @brief Add a new value to the histogram.
 *
 * The statistics related to the histogram data (sum, RMS, etc.) are
 * also updated.
 */
template<typename ScalarType>
void Histogram<ScalarType>::addValue(ScalarType val, ScalarType increment)
{
	uint pos = binIndex(val);
	if (val < minVal)
		minVal = val;
	if (val > maxVal)
		maxVal = val;
	assert((pos >= 0) && (pos <= nBins + 1));
	H[pos] += increment;
	cnt += increment;
	sum += val * increment;
	rms += (val * val) * increment;
}


/**
 * @brief Minimum value of the range where the histogram is defined.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::minRangeValue() const
{
	return minRangeVal;
}

/**
 * @brief Maximum value of the range where the histogram is defined.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::maxRangeValue() const
{
	return maxRangeVal;
}

/**
 * @brief Total sum of inserted values.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::sumValues() const
{
	return sum;
}

/**
 * @brief Number of values inserted in the histogram.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::numberValues() const
{
	return cnt;
}

/**
 * @brief Minimum value that has been added to the histogram.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::minValue() const
{
	return minVal;
}

/**
 * @brief Maximum value that has been added to the histogram.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::maxValue() const
{
	return maxVal;
}

/**
 * @brief Max number of values among all bins (including the two infinity bounded bins)
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::maxBinCount() const
{
	return *(std::max_element(H.begin(),H.end()));
}

/**
 * @brief Max number of values among all bins between the minRangeValue and maxRangeValue.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::maxBinCountInRange() const
{
	return *(std::max_element(H.begin()+1,H.end()-1));
}

/**
 * @brief Number of intervals in the histogram
 * @return
 */
template<typename ScalarType>
uint Histogram<ScalarType>::binsNumber() const
{
	return nBins;
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binCount(uint ind) const
{
	return H[ind];
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binLowerBound(uint ind) const
{
	return R[ind];
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binUpperBound(uint ind) const
{
	return R[ind+1];
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binOfValueCount(ScalarType value) const
{
	return H[binIndex(value)];
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binOfValueCount(ScalarType value, ScalarType width) const
{
	return rangeCount(value - width / 2.0, value + width / 2.0);
}

template<typename ScalarType>
ScalarType Histogram<ScalarType>::binOfValueWidth(ScalarType value)
{
	uint pos=BinIndex(value);
	return R[pos+1]-R[pos];
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
 * @brief Returns the value corresponding to a given percentile of the data.
 *
 * The percentile range between 0 and 1.
 * @param frac
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::percentile(ScalarType frac) const
{
	if (H.size() == 0)
		return 0;

	// check percentile range
	assert(frac >= 0 && frac <= 1);

	ScalarType sum     = cnt * frac;
	ScalarType partsum = 0;

	uint i = 0;
	while (partsum < sum) {
		partsum += H[i];
		++i;
	}

	return R[i + 1];
}

/**
 * @brief Returns the average of the data.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::average() const
{
	return sum / cnt;
}

/**
 * @brief Returns the Root Mean Square of the data.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::rootMeanSquare() const
{
	return std::sqrt(rms / cnt);
}

/**
 * @brief Returns the variance of the data.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::variance() const
{
	return std::abs(rms / cnt - average() * average());
}

/**
 * @brief Returns the standard deviation of the data.
 * @return
 */
template<typename ScalarType>
ScalarType Histogram<ScalarType>::standardDeviation() const
{
	return std::sqrt(variance());
}

/**
 * @brief Returns the index of the bin which contains a given element.
 */
template<typename ScalarType>
uint Histogram<ScalarType>::binIndex(ScalarType elem) const
{
	// lower_bound returns the furthermost iterator i in [first, last) such that, for every iterator
	// j in [first, i), *j < value. E.g. An iterator pointing to the first element "not less than"
	// val, or end() if every element is less than val.
	typename std::vector<ScalarType>::const_iterator it = std::lower_bound(R.begin(),R.end(), elem);

	assert(it != R.begin());
	assert(it != R.end());

	uint pos = it - R.begin();
	pos -= 1;

	assert (R[pos] < elem);
	assert (elem <= R[pos+1] );
	return pos;
}

} // namespace vcl
