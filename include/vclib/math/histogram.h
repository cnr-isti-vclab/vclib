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

template<typename ScalarType>
class Histogram
{
    std::vector<ScalarType> H; // Counters for bins
    std::vector<ScalarType> R; // Range for bins

    ScalarType minRangeVal = 0; // Minimum range value
    ScalarType maxRangeVal = 1; // Maximum range value

    ScalarType minVal = std::numeric_limits<ScalarType>::max();
    ScalarType maxVal = std::numeric_limits<ScalarType>::lowest();

    // Number of valid bins stored between minRangeValue and maxRangeValue
    uint nBins = 0;

    ScalarType cnt = 0; // Number of accumulated values
    ScalarType sum = 0; // Sum of values
    ScalarType rms = 0; // Root mean square

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
     * the distribution of the ranges of the bins. Default uniform distibution.
     */
    Histogram(
        ScalarType minRangeValue,
        ScalarType maxRangeValue,
        uint       nBins,
        ScalarType gamma = 1.0) :
            minRangeVal(minRangeValue),
            maxRangeVal(maxRangeValue), nBins(nBins)
    {
        H.resize(nBins + 2, 0);

        R.resize(nBins + 3);

        R[0]         = -std::numeric_limits<ScalarType>::max();
        R[nBins + 2] = std::numeric_limits<ScalarType>::max();

        double delta = (maxRangeVal - minRangeVal);
        if (gamma == 1) {
            for (uint i = 0; i <= nBins; ++i)
                R[i + 1] = minRangeVal + delta * ScalarType(i) / nBins;
        }
        else {
            for (uint i = 0; i <= nBins; ++i)
                R[i + 1] = minRangeVal +
                           delta * std::pow(ScalarType(i) / nBins, gamma);
        }
    }

    /**
     * @brief Clears the histogram.
     */
    void clear()
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
    void addValue(ScalarType value, ScalarType increment = 1.0)
    {
        uint pos = binIndex(value);
        if (value < minVal)
            minVal = value;
        if (value > maxVal)
            maxVal = value;
        assert((pos >= 0) && (pos <= nBins + 1));
        H[pos] += increment;
        cnt += increment;
        sum += value * increment;
        rms += (value * value) * increment;
    }

    /**
     * @brief Minimum value of the range where the histogram is defined.
     * @return
     */
    ScalarType minRangeValue() const { return minRangeVal; }

    /**
     * @brief Maximum value of the range where the histogram is defined.
     * @return
     */
    ScalarType maxRangeValue() const { return maxRangeVal; }

    /**
     * @brief Total sum of inserted values.
     * @return
     */
    ScalarType sumValues() const { return sum; }

    /**
     * @brief Number of values inserted in the histogram.
     * @return
     */
    ScalarType numberValues() const { return cnt; }

    /**
     * @brief Minimum value that has been added to the histogram.
     * @return
     */
    ScalarType minValue() const { return minVal; }

    /**
     * @brief Maximum value that has been added to the histogram.
     * @return
     */
    ScalarType maxValue() const { return maxVal; }

    /**
     * @brief Max number of values among all bins (including the two infinity
     * bounded bins)
     * @return
     */
    ScalarType maxBinCount() const
    {
        return *(std::max_element(H.begin(), H.end()));
    }

    /**
     * @brief Max number of values among all bins between the minRangeValue and
     * maxRangeValue.
     * @return
     */
    ScalarType maxBinCountInRange() const
    {
        return *(std::max_element(H.begin() + 1, H.end() - 1));
    }

    /**
     * @brief Number of intervals in the histogram
     * @return
     */
    uint binsNumber() const { return nBins; }

    ScalarType binCount(uint ind) const { return H[ind]; }

    ScalarType binLowerBound(uint ind) const { return R[ind]; }

    ScalarType binUpperBound(uint ind) const { return R[ind + 1]; }

    ScalarType binOfValueCount(ScalarType value) const
    {
        return H[binIndex(value)];
    }

    ScalarType binOfValueCount(ScalarType value, ScalarType width) const
    {
        return rangeCount(value - width / 2.0, value + width / 2.0);
    }

    ScalarType binOfValueWidth(ScalarType value)
    {
        uint pos = BinIndex(value);
        return R[pos + 1] - R[pos];
    }

    ScalarType rangeCount(ScalarType rangeMin, ScalarType rangeMax) const
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
    ScalarType percentile(ScalarType frac) const
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
    ScalarType average() const { return sum / cnt; }

    /**
     * @brief Returns the Root Mean Square of the data.
     * @return
     */
    ScalarType rootMeanSquare() const { return std::sqrt(rms / cnt); }

    /**
     * @brief Returns the variance of the data.
     * @return
     */
    ScalarType variance() const
    {
        return std::abs(rms / cnt - average() * average());
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
        auto it = std::lower_bound(R.begin(), R.end(), elem);

        assert(it != R.begin());
        assert(it != R.end());

        uint pos = it - R.begin();
        pos -= 1;

        assert(R[pos] < elem);
        assert(elem <= R[pos + 1]);
        return pos;
    }
};

/* Specialization Aliases */

using Histogramf = Histogram<float>;
using Histogramd = Histogram<double>;

} // namespace vcl

#endif // VCL_MATH_HISTOGRAM_H
