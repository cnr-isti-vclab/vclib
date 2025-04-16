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
#include <vclib/render/drawers/benchmark_drawer.h>

#include <condition_variable>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

namespace vcl {

/**
 * Stores the vector that is "printed to" by the ObjectBenchmarkPrinter class
 * and manages synchronization.
 */
class ObjectBenchmarkPrinterResult
{
    friend class ObjectBenchmarkPrinter;

    using VectorType =
        std::vector<std::pair<std::string, std::shared_ptr<BenchmarkMetric>>>;
    std::condition_variable mFinishedCond;
    std::mutex              mFinishBoolMutex;
    std::mutex              mVectorMutex;
    bool                    mIsFinished = false;
    VectorType              mDescriptionAndMeasurementsVector;

public:
    void finish()
    {
        std::unique_lock lF(mFinishBoolMutex);
        mIsFinished = true;
        mFinishedCond.notify_all();
    }

    /**
     * Adds a measurment and its description to the vector. Nothing gets added
     * if mIsFinished is false.
     */
    void addEntry(
        std::string                      description,
        std::shared_ptr<BenchmarkMetric> metric)
    {
        std::unique_lock lF(mFinishBoolMutex);
        if (mIsFinished) {
            return;
        }
        std::unique_lock lV(mVectorMutex);
        mDescriptionAndMeasurementsVector.push_back(
            std::make_pair(description, metric));
    }

    /**
     * Blocks the calling thread until mIsFinished is true (i.e. until all
     * automations are completed).
     *
     * @returns A copy of the vector of descriptions and measured metrics
     */
    VectorType getVectorBlocking()
    {
        std::unique_lock lF(mFinishBoolMutex);
        while (!mIsFinished) {
            mFinishedCond.wait(lF);
        }
        std::unique_lock lV(mVectorMutex);
        VectorType       copy = VectorType(mDescriptionAndMeasurementsVector);
        return copy;
    }

    /**
     * Returns the null optional if locking failed or if mIsFinished was false
     * (i.e. not all automations were completed).
     *
     * @returns An optional copy of the vector of descriptions and measured
     * metrics
     */
    std::optional<VectorType> getVectorNonBlocking()
    {
        if (!mFinishBoolMutex.try_lock()) {
            return std::nullopt;
        }
        if (!mIsFinished) {
            return std::nullopt;
        }
        if (!mVectorMutex.try_lock()) {
            return std::nullopt;
        }
        std::optional<VectorType> copy =
            std::make_optional(VectorType(mDescriptionAndMeasurementsVector));
        mVectorMutex.unlock();
        mFinishBoolMutex.unlock();
        return copy;
    }
};

/**
 * Multithread-safe printer class that "prints" to a (or rather, stores in a)
 * vector. It is still not recommended to call the print() function from
 * multiple threads, since the first one to finish will call finish and disable
 * writing
 *
 * To use this class all that is needed is to create an object of this class and
 * then pass the result pointer to the separate thread
 */
class ObjectBenchmarkPrinter : public BenchmarkPrinter
{
    std::shared_ptr<ObjectBenchmarkPrinterResult> result =
        std::make_shared<ObjectBenchmarkPrinterResult>();

public:
    void onBenchmarkLoop() override {};

    std::shared_ptr<ObjectBenchmarkPrinterResult> getResultPtr()
    {
        return result;
    }

    void print(const BenchmarkMetric& metric, std::string description) override
    {
        result->addEntry(description, metric.clone());
    }

    void finish() override { result->finish(); };

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<ObjectBenchmarkPrinter>(*this);
    }

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<ObjectBenchmarkPrinter>(std::move(*this));
    }
};

} // namespace vcl

#endif
