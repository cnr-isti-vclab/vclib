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

#ifndef VCL_NULL_BENCHMARK_PRINTER_H
#define VCL_NULL_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>

namespace vcl {

/**
 * The NullBenchmarkPrinter class is a BenchmarkPrinter that writes nothing
 * nowhere
 */
class NullBenchmarkPrinter : public BenchmarkPrinter
{
public:
    void print(BenchmarkMetric& metric, std::string description) override {};

    void onBenchmarkLoop() override {};

    void finish() override {};

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<NullBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<NullBenchmarkPrinter>(std::move(*this));
    };
};

} // namespace vcl

#endif
