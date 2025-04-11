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

#ifndef VCL_OBJECT_BENCHMARK_PRINTER
#define VCL_OBJECT_BENCHMARK_PRINTER

#include "../metrics/benchmark_metric.h"
#include "benchmark_printer.h"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

namespace vcl {

class ObjectBenchmarkPrinterResultMTUnsafe
{
    template<typename ResultObjectType>
    friend class ObjectBenchmarkPrinter;

    using VectorType =
        std::vector<std::pair<std::string, std::shared_ptr<BenchmarkMetric>>>;
    bool       mIsFinished = false;
    VectorType mDescriptionAndMeasurementsVector;

    void finish() { mIsFinished = true; }

    void addEntry(
        std::string                      description,
        std::shared_ptr<BenchmarkMetric> metric)
    {
        mDescriptionAndMeasurementsVector.push_back(
            std::make_pair(description, metric));
    }

public:
    /**
     * @returns An optional copy of the vector of descriptions and measured
     * metrics. Returns nullopt when the mIsFinished variable was false
     */
    std::optional<VectorType> getVector()
    {
        if (!mIsFinished) {
            return std::nullopt;
        }
        return std::make_optional(
            VectorType(mDescriptionAndMeasurementsVector));
    }
};

class ObjectBenchmarkPrinterResultMTSafe
{
    template<typename ResultObjectType>
    friend class ObjectBenchmarkPrinter;

    using VectorType =
        std::vector<std::pair<std::string, std::shared_ptr<BenchmarkMetric>>>;
    std::condition_variable      mFinishedCond;
    std::unique_lock<std::mutex> mFinishBoolMutex;
    bool                         mIsFinished = false;
    VectorType                   mDescriptionAndMeasurementsVector;

    void finish()
    {
        mFinishBoolMutex.lock();
        mIsFinished = true;
        mFinishBoolMutex.unlock();
    }

    void addEntry(
        std::string                      description,
        std::shared_ptr<BenchmarkMetric> metric)
    {
        mDescriptionAndMeasurementsVector.push_back(
            std::make_pair(description, metric));
    }

public:
    /**
     * Blocks the calling thread until mIsFinished is true (i.e. until all
     * automations are completed).
     *
     * @returns A copy of the vector of descriptions and measured metrics
     */
    VectorType getVector()
    {
        mFinishBoolMutex.lock();
        while (!mIsFinished) {
            mFinishedCond.wait(mFinishBoolMutex);
        }
        mFinishBoolMutex.unlock();
        return VectorType(mDescriptionAndMeasurementsVector);
    }
};

template<typename ResultObjectType>
class ObjectBenchmarkPrinter : public BenchmarkPrinter
{
    std::shared_ptr<ResultObjectType> result;

public:
    void print(BenchmarkMetric& metric, std::string description) override
    {
        result->addEntry(description, metric.clone());
    }

    void onBenchmarkLoop() override {};

    void finish() override { result->finish(); };

    std::shared_ptr<ResultObjectType> getResultPtr() { return result; }

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<ObjectBenchmarkPrinter<ResultObjectType>>(
            *this);
    }

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<ObjectBenchmarkPrinter<ResultObjectType>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
