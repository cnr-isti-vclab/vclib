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

#ifndef VCL_BENCHMARK_METRIC_H
#define VCL_BENCHMARK_METRIC_H

#include <string>
#include <vector>

namespace vcl {

/*
    Abstract class that represents a way of measuring the performance of an
   Automation.
*/
class BenchmarkMetric
{
public:
    virtual void                     start()                      = 0;
    virtual void                     measure()                    = 0;
    virtual std::vector<std::string> getMeasureStrings()          = 0;
    virtual std::string              getUnitOfMeasure()           = 0;
    virtual std::string              getFullLengthUnitOfMeasure() = 0;
    virtual void                     end()                        = 0;

    virtual std::shared_ptr<BenchmarkMetric> clone() const& = 0;
    virtual std::shared_ptr<BenchmarkMetric> clone() &&     = 0;
};

} // namespace vcl

#endif
