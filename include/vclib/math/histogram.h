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

#ifndef VCL_MATH_HISTOGRAM_H
#define VCL_MATH_HISTOGRAM_H

#include <algorithm>
#include <vector>

#include <vclib/misc/types.h>

namespace vcl {

template<typename ScalarType>
class Histogram
{
public:
	Histogram();
	Histogram(ScalarType minRangeValue, ScalarType maxRangeValue, uint nBins, ScalarType gamma = 1.0);

	void clear();
	void addValue(ScalarType value, ScalarType increment = 1.0);

	ScalarType minRangeValue() const;
	ScalarType maxRangeValue() const;

	ScalarType sumValues() const;
	ScalarType numberValues() const;
	ScalarType minValue() const;
	ScalarType maxValue() const;

	ScalarType maxBinCount() const;
	ScalarType maxBinCountInRange() const;
	uint binsNumber() const;

	ScalarType binCount(uint ind) const;
	ScalarType binLowerBound(uint ind) const;
	ScalarType binUpperBound(uint ind) const;

	ScalarType binOfValueCount(ScalarType value) const;
	ScalarType binOfValueCount(ScalarType value, ScalarType width) const;
	ScalarType binOfValueWidth(ScalarType value);

	ScalarType rangeCount(ScalarType rangeMin, ScalarType rangeMax) const;

	ScalarType percentile(ScalarType frac) const;
	ScalarType average() const;
	ScalarType rootMeanSquare() const;
	ScalarType variance() const;
	ScalarType standardDeviation() const;

protected:
	uint binIndex(ScalarType val) const;

	std::vector<ScalarType> H; // Counters for bins
	std::vector<ScalarType> R; // Range for bins

	ScalarType minRangeVal = 0; // Minimum range value
	ScalarType maxRangeVal = 1; // Maximum range value

	ScalarType minVal = std::numeric_limits<ScalarType>::max();
	ScalarType maxVal = std::numeric_limits<ScalarType>::lowest();

	uint nBins = 0; // Number of valid bins stored between minRangeValue and maxRangeValue

	ScalarType cnt = 0; // Number of accumulated values
	ScalarType sum = 0; // Sum of values
	ScalarType rms = 0; // Root mean square
};

using Histogramf = Histogram<float>;
using Histogramd = Histogram<double>;

} // namespace vcl

#include "histogram.cpp"

#endif // VCL_MATH_HISTOGRAM_H
