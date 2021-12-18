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

#include "distribution.h"

#include <cmath>

namespace vcl {

/**
 * @brief Creates an empty distribuiton.
 */
template<typename Scalar>
Distribution<Scalar>::Distribution()
{
}

/**
 * @brief Clears the distribution, removing all its values.
 */
template<typename Scalar>
void Distribution<Scalar>::clear()
{
	set.clear();
	minValue   = std::numeric_limits<Scalar>::max();
	maxValue   = std::numeric_limits<Scalar>::lowest();
	valSum     = 0;
	sqrdValSum = 0;
	valAvg     = 0;
	sqrdValAvg = 0;
	valRMS     = 0;
}

/**
 * @brief Adds a value to the distribution.
 * @param[in] v: the value to add.
 */
template<typename Scalar>
void Distribution<Scalar>::add(Scalar v)
{
	set.insert(v);
	if (v < minValue)
		minValue = v;
	if (v > maxValue)
		maxValue = v;
	valSum += v;
	sqrdValSum += v * v;
	valAvg     = valSum / set.size();
	sqrdValAvg = sqrdValSum / set.size();
	valRMS     = std::sqrt(sqrdValAvg);
}

/**
 * @brief Returns the minimum value of the distribution.
 *
 * If the distribution is empty, returns std::numeric_limits::max().
 * @return The minimum of the distribution values.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::min() const
{
	return minValue;
}

/**
 * @brief Returns the maximum value of the distribution.
 *
 * If the distribution is empty, returns std::numeric_limits::lowest().
 * @return The maximum of the distribution values.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::max() const
{
	return maxValue;
}

/**
 * @brief Returns the number of values of the distribution.
 * @return The size of the distribution.
 */
template<typename Scalar>
uint Distribution<Scalar>::size() const
{
	return set.size();
}

/**
 * @brief Returns the sum of the values of the distribution.
 * @return The sum of the distribution values.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::sum() const
{
	return valSum;
}

/**
 * @brief Returns the average of the values of the distribution.
 * @return The average of the distribution values.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::average() const
{
	return valAvg;
}

/**
 * @brief Returns the root mean square of the values of the distribution.
 * @return The root mean square of the distribution values.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::rootMeanSquare() const
{
	return valRMS;
}

/**
 * @brief Returns the variance of the values of the distribution.
 * @return The variance of the distribution values.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::variance() const
{
	return sqrdValAvg - valAvg * valAvg;
}

/**
 * @brief Returns the standard deviation of the values of the distribution.
 * @return The standard deviation of the distribution values.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::standardDeviation() const
{
	return std::sqrt(variance());
}

/**
 * @brief Returns the `perc` percentile of the values of the distribution.
 *
 * @param[in] perc: the desired percentile. Must be a value between 0 and 1.
 * @return The value of the distribution in the given percentile.
 */
template<typename Scalar>
Scalar Distribution<Scalar>::percentile(Scalar perc) const
{
	assert(!set.empty());
	assert(perc >= 0 && perc <= 1);

	int index = set.size() * perc - 1;
	if (index < 0)
		index = 0;
	auto it = set.begin();
	std::advance(it, index);
	return *it;
}

} // namespace vcl
