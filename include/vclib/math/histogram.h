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

#include <vector>

#include <vclib/misc/types.h>

namespace vcl {

template<typename ScalarType>
class Histogram
{
public:
	Histogram();
	Histogram(ScalarType minValue, ScalarType maxValue, uint nIntervals, ScalarType gamma = 1.0);

	void clear();

	void addValue(ScalarType value, ScalarType increment = 1.0);

	ScalarType binCountInd(uint ind) const;
	ScalarType binCount(ScalarType value) const;
	ScalarType binCount(ScalarType value, ScalarType width) const;

	ScalarType rangeCount(ScalarType rangeMin, ScalarType rangeMax) const;

protected:
	uint binIndex(ScalarType val) const;

	std::vector<ScalarType> H; // Counters for bins
	std::vector<ScalarType> R; // Range for bins

	ScalarType minVal = 0; // Minimum value
	ScalarType maxVal = 1; // Maximum value

	ScalarType minElem = std::numeric_limits<ScalarType>::max();
	ScalarType maxElem = std::numeric_limits<ScalarType>::lowest();

	uint nIntervals = 0; // Number of valid intervals stored between minValue and maxValue

	ScalarType cnt = 0; // Number of accumulated samples
	ScalarType sum = 0; // Sum of values
	ScalarType rms = 0; // Root mean square
};

} // namespace vcl

#include "histogram.cpp"

#endif // VCL_MATH_HISTOGRAM_H
