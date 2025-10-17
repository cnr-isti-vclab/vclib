/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_MULTIPLE_FPS_BENCHMARK_METRIC_H
#define VCL_MULTIPLE_FPS_BENCHMARK_METRIC_H

#include <vclib/render/automation/metrics/benchmark_metric.h>

#include <vclib/base.h>

#include <iomanip>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

namespace vcl {

/**
 * The MultipleFpsBenchmarkMetric class measures the average framerate multiple
 * times for an automation. It keeps track of the frame-local framerate in a
 * vector, then "splits" the vector into multiple sections and calculates the
 * average for each section. If the chosen number of measurements is greater
 * than the measurements taken (i.e. the number of frames the automation took to
 * complete), it returns the entire vector.
 */
class MultipleFpsBenchmarkMetric : public BenchmarkMetric
{
    bool                mFirstMeasurement = true;
    std::vector<double> mMeasurements;
    uint                mMeasurementCount;
    Timer               mTimer;

public:
    MultipleFpsBenchmarkMetric(uint measurementCount) :
            mMeasurementCount {measurementCount}
    {
    }

    void start() override
    {
        mMeasurements     = std::vector<double>();
        mFirstMeasurement = true;
    };

    void measure() override
    {
        if (mFirstMeasurement) {
            mTimer.start();
            mFirstMeasurement = false;
            return;
        }
        mTimer.stop();
        mMeasurements.push_back(1.0 / mTimer.delay());
        mTimer.start();
    };

    std::vector<std::string> getMeasureStrings() const override
    {
        std::vector<std::string> retVec;
        uint                     trueMeasurementCount =
            std::min((uint) mMeasurements.size() - 1, mMeasurementCount);
        for (uint i = 1; i <= trueMeasurementCount; i++) {
            uint from =
                (uint) ((double) (i - 1) / (double) trueMeasurementCount *
                        (double) mMeasurements.size());
            uint   to  = (uint) ((double) i / (double) trueMeasurementCount *
                              (double) mMeasurements.size());
            double sum = 0;
            for (uint j = from; j < to; j++) {
                sum += mMeasurements[j];
            }
            double count = (double) to - (double) from;

            std::ostringstream temp;
            temp << std::fixed << std::setprecision(3) << sum / count;
            retVec.push_back(temp.str());
        }

        return retVec;
    };

    std::string getUnitOfMeasure() const override { return "fps"; }

    std::string getFullLengthUnitOfMeasure() const override
    {
        return "frames per second";
    }

    void end() override { mTimer.stop(); };

    std::shared_ptr<BenchmarkMetric> clone() const& override
    {
        return std::make_shared<MultipleFpsBenchmarkMetric>(*this);
    };

    std::shared_ptr<BenchmarkMetric> clone() && override
    {
        return std::make_shared<MultipleFpsBenchmarkMetric>(std::move(*this));
    };
};

} // namespace vcl

#endif
