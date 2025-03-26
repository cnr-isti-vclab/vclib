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

#include <vclib/misc/timer.h>
#include <string>
#include <format>
#include <vclib/render/automation/metrics/benchmark_metric.h>

namespace vcl{

/*
    Measures the average frames per second of the automations
*/
class FpsBenchmarkMetric : public BenchmarkMetric
{
    Timer timer;
    double frames = 0;

    public:

    void start() override
    {
        timer.start();
        frames = 0;
    };

    void measure() override
    {
        frames++;
    };

    std::string getMeasureString() override
    {
        return std::format("{:.3f}", frames / timer.delay());
    };

    std::string getUnitOfMeasure() override
    {
        return "fps";
    }

    std::string getFullLengthUnitOfMeasure() override
    {
        return "frames per second";
    }

    void end() override
    {
        timer.stop();
    };

    std::shared_ptr<BenchmarkMetric> clone() const & override
    {
        return std::make_shared<FpsBenchmarkMetric>(*this);
    };
    std::shared_ptr<BenchmarkMetric> clone()  && override
    {
        return std::make_shared<FpsBenchmarkMetric>(std::move(*this));
    };
};

}

#endif