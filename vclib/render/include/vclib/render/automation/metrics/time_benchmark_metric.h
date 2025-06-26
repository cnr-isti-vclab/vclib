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

#ifndef VCL_TIME_BENCHMARK_METRIC_H
#define VCL_TIME_BENCHMARK_METRIC_H

#include <vclib/render/automation/metrics/benchmark_metric.h>

#include <vclib/misc/timer.h>

#include <iomanip>
#include <sstream>

namespace vcl {

/**
 * The TimeBenchmarkMetric class measures the time (in seconds) an automation
 * takes to complete
 */
class TimeBenchmarkMetric : public BenchmarkMetric
{
    bool  mFirstMeasurement = true;
    Timer mTimer;

public:
    void start() override { mFirstMeasurement = true; }

    void measure() override
    {
        if (mFirstMeasurement) {
            mFirstMeasurement = false;
            mTimer.start();
            return;
        }
    }

    std::vector<std::string> getMeasureStrings() const override
    {
        std::ostringstream temp;
        temp << std::fixed << std::setprecision(3) << mTimer.delay();

        return std::vector<std::string> {temp.str()};
    }

    std::string getUnitOfMeasure() const override { return "s"; }

    std::string getFullLengthUnitOfMeasure() const override { return "seconds"; }

    void end() override { mTimer.stop(); }

    std::shared_ptr<BenchmarkMetric> clone() const& override
    {
        return std::make_shared<TimeBenchmarkMetric>(*this);
    }

    std::shared_ptr<BenchmarkMetric> clone() && override
    {
        return std::make_shared<TimeBenchmarkMetric>(std::move(*this));
    };
};

} // namespace vcl

#endif
