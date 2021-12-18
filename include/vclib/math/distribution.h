/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MATH_DISTRIBUTION_H
#define VCL_MATH_DISTRIBUTION_H

#include <limits>
#include <set>

#include <vclib/misc/vcl_types.h>

namespace vcl {

/**
 * @brief The Distribution class allows to collect a set of values and then compute some statistics
 * like average, variance, standardDeviation, and percentiles.
 */
template<typename Scalar>
class Distribution
{
public:
	Distribution();

	void clear();

	void add(Scalar v);

	Scalar min() const;
	Scalar max() const;
	uint   size() const;

	Scalar sum() const;
	Scalar average() const;
	Scalar rootMeanSquare() const;
	Scalar variance() const;
	Scalar standardDeviation() const;

	Scalar percentile(Scalar perc) const;

private:
	std::set<Scalar> set;

	Scalar minValue = std::numeric_limits<Scalar>::max();
	Scalar maxValue = std::numeric_limits<Scalar>::lowest();

	Scalar valSum     = 0;
	Scalar sqrdValSum = 0;
	Scalar valAvg     = 0;
	Scalar sqrdValAvg = 0;
	Scalar valRMS     = 0;
};

} // namespace vcl

#include "distribution.cpp"

#endif // VCL_MATH_DISTRIBUTION_H
