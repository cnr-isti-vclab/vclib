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

#ifndef VCL_MATH_HISTOGRAM_H
#define VCL_MATH_HISTOGRAM_H

#include <algorithm>
#include <cmath>
#include <vector>

#include <vclib/types.h>

namespace vcl {

// TODO: improve documentation for this class

/**
 * @brief The Histogram class allows to collect a set of values and then compute
 * some statistics like average, variance, standardDeviation, and percentiles.
 *
 * @tparam ScalarType
 *
 * @ingroup math
 */
template<typename ScalarType>
class Histogram
{
    std::vector<ScalarType> mHist;   // Counters for bins
    std::vector<ScalarType> mRanges; // Range for bins

    ScalarType mMinRange = 0; // Minimum range value
    ScalarType mMaxRange = 1; // Maximum range value

    ScalarType mMin = std::numeric_limits<ScalarType>::max();
    ScalarType mMax = std::numeric_limits<ScalarType>::lowest();

    // Number of valid bins stored between mMinRange and mMaxRange
    uint mBinNumber = 0;

    ScalarType mCnt = 0; // Number of accumulated values
    ScalarType mSum = 0; // Sum of values
    ScalarType mRMS = 0; // Root mean square

public:
    Histogram() = default;

    /**
     * @brief Set the histogram values.
     *
     * This method is used to correctly initialize the bins of the histogram.
     * n is the number of valid intervals between minv and maxv.
     * for a more robust working, the Histogram class stores also the two out of
     * range intervals (-inf, minv] and [maxv, +inf). Each bin is left closed
     * (eg it contains the value). The \a gamma parameter is applied to modify
     * the distribution of the ranges of the bins. Default uniform distribution.
     */
    Histogram(
        ScalarType minRangeValue,
        ScalarType maxRangeValue,
        uint       nBins,
        ScalarType gamma = 1.0) :
            mMinRange(minRangeValue), mMaxRange(maxRangeValue),
            mBinNumber(nBins)
    {
        mHist.resize(nBins + 2, 0);

        mRanges.resize(nBins + 3);

        mRanges[0]         = -std::numeric_limits<ScalarType>::max();
        mRanges[nBins + 2] = std::numeric_limits<ScalarType>::max();

        double delta = (mMaxRange - mMinRange);
        if (gamma == 1) {
            for (uint i = 0; i <= nBins; ++i)
                mRanges[i + 1] = mMinRange + delta * ScalarType(i) / nBins;
        }
        else {
            for (uint i = 0; i <= nBins; ++i)
                mRanges[i + 1] =
                    mMinRange + delta * std::pow(ScalarType(i) / nBins, gamma);
        }
    }

    /**
     * @brief Clears the histogram.
     */
    void clear()
    {
        mHist.clear();
        mRanges.clear();
        mCnt       = 0;
        mSum       = 0;
        mRMS       = 0;
        mBinNumber = 0;
        mMinRange  = 0;
        mMaxRange  = 1;
        mMin       = std::numeric_limits<ScalarType>::max();
        mMax       = std::numeric_limits<ScalarType>::lowest();
    }

    /**
     * @brief Add a new value to the histogram.
     *
     * The statistics related to the histogram data (sum, RMS, etc.) are
     * also updated.
     */
    void addValue(ScalarType value, ScalarType increment = 1.0)
    {
        uint pos = binIndex(value);
        if (value < mMin)
            mMin = value;
        if (value > mMax)
            mMax = value;
        assert((pos >= 0) && (pos <= mBinNumber + 1));
        mHist[pos] += increment;
        mCnt += increment;
        mSum += value * increment;
        mRMS += (value * value) * increment;
    }

    /**
     * @brief Minimum value of the range where the histogram is defined.
     * @return
     */
    ScalarType minRangeValue() const { return mMinRange; }

    /**
     * @brief Maximum value of the range where the histogram is defined.
     * @return
     */
    ScalarType maxRangeValue() const { return mMaxRange; }

    /**
     * @brief Total sum of inserted values.
     * @return
     */
    ScalarType sumValues() const { return mSum; }

    /**
     * @brief Number of values inserted in the histogram.
     * @return
     */
    ScalarType numberValues() const { return mCnt; }

    /**
     * @brief Minimum value that has been added to the histogram.
     * @return
     */
    ScalarType minValue() const { return mMin; }

    /**
     * @brief Maximum value that has been added to the histogram.
     * @return
     */
    ScalarType maxValue() const { return mMax; }

    /**
     * @brief Max number of values among all bins (including the two infinity
     * bounded bins)
     * @return
     */
    ScalarType maxBinCount() const
    {
        return *(std::max_element(mHist.begin(), mHist.end()));
    }

    /**
     * @brief Max number of values among all bins between the minRangeValue and
     * maxRangeValue.
     * @return
     */
    ScalarType maxBinCountInRange() const
    {
        return *(std::max_element(mHist.begin() + 1, mHist.end() - 1));
    }

    /**
     * @brief Number of intervals in the histogram
     * @return
     */
    uint binsNumber() const { return mBinNumber; }

    ScalarType binCount(uint ind) const { return mHist[ind]; }

    ScalarType binLowerBound(uint ind) const { return mRanges[ind]; }

    ScalarType binUpperBound(uint ind) const { return mRanges[ind + 1]; }

    ScalarType binOfValueCount(ScalarType value) const
    {
        return mHist[binIndex(value)];
    }

    ScalarType binOfValueCount(ScalarType value, ScalarType width) const
    {
        return rangeCount(value - width / 2.0, value + width / 2.0);
    }

    ScalarType binOfValueWidth(ScalarType value)
    {
        uint pos = BinIndex(value);
        return mRanges[pos + 1] - mRanges[pos];
    }

    ScalarType rangeCount(ScalarType rangeMin, ScalarType rangeMax) const
    {
        uint firstBin = binIndex(rangeMin);
        uint lastBin  = binIndex(rangeMax);

        ScalarType sum = 0;
        for (uint i = firstBin; i <= lastBin; ++i)
            sum += mHist[i];
        return sum;
    }

    /**
     * @brief Returns the value corresponding to a given percentile of the data.
     *
     * The percentile range between 0 and 1.
     * @param frac
     * @return
     */
    ScalarType percentile(ScalarType frac) const
    {
        if (mHist.size() == 0)
            return 0;

        // check percentile range
        assert(frac >= 0 && frac <= 1);

        ScalarType sum     = mCnt * frac;
        ScalarType partsum = 0;

        uint i = 0;
        while (partsum < sum) {
            partsum += mHist[i];
            ++i;
        }

        return mRanges[i + 1];
    }

    /**
     * @brief Returns the average of the data.
     * @return
     */
    ScalarType average() const { return mSum / mCnt; }

    /**
     * @brief Returns the Root Mean Square of the data.
     * @return
     */
    ScalarType rootMeanSquare() const { return std::sqrt(mRMS / mCnt); }

    /**
     * @brief Returns the variance of the data.
     * @return
     */
    ScalarType variance() const
    {
        return std::abs(mRMS / mCnt - average() * average());
    }

    /**
     * @brief Returns the standard deviation of the data.
     * @return
     */
    ScalarType standardDeviation() const { return std::sqrt(variance()); }

private:
    /**
     * @brief Returns the index of the bin which contains a given element.
     */
    uint binIndex(ScalarType elem) const
    {
        // lower_bound returns the furthermost iterator i in [first, last) such
        // that, for every iterator j in [first, i), *j < value. E.g. An
        // iterator pointing to the first element "not less than" val, or end()
        // if every element is less than val.
        auto it = std::lower_bound(mRanges.begin(), mRanges.end(), elem);

        assert(it != mRanges.begin());
        assert(it != mRanges.end());

        uint pos = it - mRanges.begin();
        pos -= 1;

        assert(mRanges[pos] < elem);
        assert(elem <= mRanges[pos + 1]);
        return pos;
    }
};

/* Specialization Aliases */

using Histogramf = Histogram<float>;
using Histogramd = Histogram<double>;

} // namespace vcl

#endif // VCL_MATH_HISTOGRAM_H
