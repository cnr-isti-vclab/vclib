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
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MATH_DISTRIBUTION_H
#define VCL_MATH_DISTRIBUTION_H

#include <vclib/types.h>

#include <cmath>
#include <limits>
#include <set>

namespace vcl {

/**
 * @brief The Distribution class allows to collect a set of values and then
 * compute some statistics like average, variance, standardDeviation, and
 * percentiles.
 *
 * @ingroup math
 */
template<typename Scalar>
class Distribution
{
    std::set<Scalar> mSet;

    Scalar mMin = std::numeric_limits<Scalar>::max();
    Scalar mMax = std::numeric_limits<Scalar>::lowest();

    Scalar mSum     = 0;
    Scalar mSqrdSum = 0;
    Scalar mAvg     = 0;
    Scalar mSqrdAvg = 0;
    Scalar mRMS     = 0;

public:
    /**
     * @brief Creates an empty distribution.
     */
    Distribution() = default;

    /**
     * @brief Clears the distribution, removing all its values.
     */
    void clear()
    {
        mSet.clear();
        mMin     = std::numeric_limits<Scalar>::max();
        mMax     = std::numeric_limits<Scalar>::lowest();
        mSum     = 0;
        mSqrdSum = 0;
        mAvg     = 0;
        mSqrdAvg = 0;
        mRMS     = 0;
    }

    /**
     * @brief Adds a value to the distribution.
     * @param[in] v: the value to add.
     */
    void add(Scalar v)
    {
        mSet.insert(v);
        if (v < mMin)
            mMin = v;
        if (v > mMax)
            mMax = v;
        mSum += v;
        mSqrdSum += v * v;
        mAvg     = mSum / mSet.size();
        mSqrdAvg = mSqrdSum / mSet.size();
        mRMS     = std::sqrt(mSqrdAvg);
    }

    /**
     * @brief Returns the minimum value of the distribution.
     *
     * If the distribution is empty, returns std::numeric_limits::max().
     * @return The minimum of the distribution values.
     */
    Scalar min() const { return mMin; }

    /**
     * @brief Returns the maximum value of the distribution.
     *
     * If the distribution is empty, returns std::numeric_limits::lowest().
     * @return The maximum of the distribution values.
     */
    Scalar max() const { return mMax; }

    /**
     * @brief Returns the number of values of the distribution.
     * @return The size of the distribution.
     */
    uint size() const { return mSet.size(); }

    /**
     * @brief Returns the sum of the values of the distribution.
     * @return The sum of the distribution values.
     */
    Scalar sum() const { return mSum; }

    /**
     * @brief Returns the average of the values of the distribution.
     * @return The average of the distribution values.
     */
    Scalar average() const { return mAvg; }

    /**
     * @brief Returns the root mean square of the values of the distribution.
     * @return The root mean square of the distribution values.
     */
    Scalar rootMeanSquare() const { return mRMS; }

    /**
     * @brief Returns the variance of the values of the distribution.
     * @return The variance of the distribution values.
     */
    Scalar variance() const { return mSqrdAvg - mAvg * mAvg; }

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
        assert(!mSet.empty());
        assert(perc >= 0 && perc <= 1);

        int index = mSet.size() * perc - 1;
        if (index < 0)
            index = 0;
        auto it = mSet.begin();
        std::advance(it, index);
        return *it;
    }
};

} // namespace vcl

#endif // VCL_MATH_DISTRIBUTION_H
