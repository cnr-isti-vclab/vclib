/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_MATH_DISTRIBUTION_H
#define VCL_MATH_DISTRIBUTION_H

#include <cmath>
#include <limits>
#include <set>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief The Distribution class allows to collect a set of values and then
 * compute some statistics like average, variance, standardDeviation, and
 * percentiles.
 */
template<typename Scalar>
class Distribution
{
    std::set<Scalar> set;

    Scalar minValue = std::numeric_limits<Scalar>::max();
    Scalar maxValue = std::numeric_limits<Scalar>::lowest();

    Scalar valSum     = 0;
    Scalar sqrdValSum = 0;
    Scalar valAvg     = 0;
    Scalar sqrdValAvg = 0;
    Scalar valRMS     = 0;

public:
    /**
     * @brief Creates an empty distribuiton.
     */
    Distribution() = default;

    /**
     * @brief Clears the distribution, removing all its values.
     */
    void clear()
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
    void add(Scalar v)
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
    Scalar min() const { return minValue; }

    /**
     * @brief Returns the maximum value of the distribution.
     *
     * If the distribution is empty, returns std::numeric_limits::lowest().
     * @return The maximum of the distribution values.
     */
    Scalar max() const { return maxValue; }

    /**
     * @brief Returns the number of values of the distribution.
     * @return The size of the distribution.
     */
    uint size() const { return set.size(); }

    /**
     * @brief Returns the sum of the values of the distribution.
     * @return The sum of the distribution values.
     */
    Scalar sum() const { return valSum; }

    /**
     * @brief Returns the average of the values of the distribution.
     * @return The average of the distribution values.
     */
    Scalar average() const { return valAvg; }

    /**
     * @brief Returns the root mean square of the values of the distribution.
     * @return The root mean square of the distribution values.
     */
    Scalar rootMeanSquare() const { return valRMS; }

    /**
     * @brief Returns the variance of the values of the distribution.
     * @return The variance of the distribution values.
     */
    Scalar variance() const { return sqrdValAvg - valAvg * valAvg; }

    /**
     * @brief Returns the standard deviation of the values of the distribution.
     * @return The standard deviation of the distribution values.
     */
    Scalar standardDeviation() const { return std::sqrt(variance()); }

    /**
     * @brief Returns the `perc` percentile of the values of the distribution.
     *
     * @param[in] perc: the desired percentile. Must be a value between 0 and 1.
     * @return The value of the distribution in the given percentile.
     */
    Scalar percentile(Scalar perc) const
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
};

} // namespace vcl

#endif // VCL_MATH_DISTRIBUTION_H
