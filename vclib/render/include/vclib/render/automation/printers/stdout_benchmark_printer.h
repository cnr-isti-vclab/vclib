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

#ifndef VCL_STDOUT_BENCHMARK_PRINTER_H
#define VCL_STDOUT_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>

#include <sstream>

namespace vcl {

/**
 * The StdoutBenchmarkPrinter class is a BenchmarkPrinter that writes the
 * results of a BenchmarkMetric to standard output
 */
class StdoutBenchmarkPrinter : public BenchmarkPrinter
{
    uint32_t mAutomationIndex = 0;
    uint32_t mLoopCounter     = 0;

public:
    void print(BenchmarkMetric& metric) override
    {
        std::ostringstream temp;
        temp << "[";
        bool isFirst = true;
        for (const auto& meas : metric.getMeasureStrings()) {
            if (!isFirst) {
                temp << ", ";
            }
            else {
                isFirst = false;
            }
            temp << meas + metric.getUnitOfMeasure();
        }
        temp << "]";
        std::cout << "Loop " << mLoopCounter << ", automation "
                  << mAutomationIndex << ": " << temp.str() << std::endl;

        mAutomationIndex++;
    };

    void onBenchmarkLoop() override { mLoopCounter++; }

    void finish(BenchmarkMetric& metric) override {};

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<StdoutBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<StdoutBenchmarkPrinter>(std::move(*this));
    };
};

} // namespace vcl

#endif
