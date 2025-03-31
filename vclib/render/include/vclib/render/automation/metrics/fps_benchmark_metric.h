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

#ifndef VCL_FPS_BENCHMARK_METRIC_H
#define VCL_FPS_BENCHMARK_METRIC_H

#include <vclib/render/automation/metrics/benchmark_metric.h>

#include <vclib/misc/timer.h>

#include <iomanip>
#include <sstream>
#include <string>

namespace vcl {

/**
 * The FpsBenchmarkMetric class measures the average framerate
 * for an automation by keeping track of the total duration of the automation
 * and the total frames it took to complete
 */
class FpsBenchmarkMetric : public BenchmarkMetric
{
    Timer  mTimer;
    double mFrames = 0;

public:
    void start() override
    {
        mTimer.start();
        mFrames = 0;
    };

    void measure() override { mFrames++; };

    std::vector<std::string> getMeasureStrings() override
    {
        std::ostringstream temp;
        temp << std::setprecision(3) << mFrames / mTimer.delay();

        return std::vector<std::string> {temp.str()};
    };

    std::string getUnitOfMeasure() override { return "fps"; }

    std::string getFullLengthUnitOfMeasure() override
    {
        return "mFrames per second";
    }

    void end() override { mTimer.stop(); };

    std::shared_ptr<BenchmarkMetric> clone() const& override
    {
        return std::make_shared<FpsBenchmarkMetric>(*this);
    };

    std::shared_ptr<BenchmarkMetric> clone() && override
    {
        return std::make_shared<FpsBenchmarkMetric>(std::move(*this));
    };
};

} // namespace vcl

#endif
