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



protected:
	std::vector<ScalarType> H; // Counters for bins
	std::vector<ScalarType> R; // Range for bins

	ScalarType minValue;
	ScalarType maxValue;

	ScalarType minElem;
	ScalarType maxElem;
	uint nIntervals; // Number of valid intervals stored between minValue and maxValue

	ScalarType cnt; // Number of accumulated samples
	ScalarType avg; // Average
	ScalarType rms; // Root mean square
};

} // namespace vcl

#include "histogram.cpp"

#endif // VCL_MATH_HISTOGRAM_H
