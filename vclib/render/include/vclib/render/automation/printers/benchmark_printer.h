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

#ifndef VCL_BENCHMARK_PRINTER_H
#define VCL_BENCHMARK_PRINTER_H

#include <vclib/render/automation/metrics/benchmark_metric.h>

#include <memory>
#include <string>

namespace vcl {

/**
 * The BenchmarkPrinter class is an abstract class that represent a way of
 * writing (printing, ...) measurements from a BenchmarkMetric
 */
class BenchmarkPrinter
{
public:
    /**
     * @brief Called just before the start of the next BenchmarkDrawer loop
     */
    virtual void onBenchmarkLoop() = 0;

    /**
     * @brief Called every time an automation ends
     * @param[in] metric: The metric to take the measurements from
     */
    virtual void print(BenchmarkMetric& metric) = 0;

    /**
     * @brief Called when the last automation of the last loop has finished
     * @param[in] metric: The metric to take the measurements from
     */
    virtual void finish(BenchmarkMetric& metric) = 0;

    virtual std::shared_ptr<BenchmarkPrinter> clone() const& = 0;
    virtual std::shared_ptr<BenchmarkPrinter> clone() &&     = 0;
};

} // namespace vcl

#endif
