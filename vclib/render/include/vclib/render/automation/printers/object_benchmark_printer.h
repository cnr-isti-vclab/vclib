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
public:
    using VectorElementType =
        std::pair<std::string, std::shared_ptr<BenchmarkMetric>>;

    using VectorType = std::vector<VectorElementType>;

private:
    std::condition_variable mFinishedCond;
    std::condition_variable mElementCountCond;
    std::mutex              mVectorMutex;
    bool                    mIsFinished = false;
    VectorType              mDescriptionAndMeasurementsVector;

public:
    void finish()
    {
        std::unique_lock lV(mVectorMutex);
        mIsFinished = true;
        mFinishedCond.notify_all();
        mElementCountCond.notify_all();
    }

    bool isFinished()
    {
        std::unique_lock lV(mVectorMutex);
        return mIsFinished;
    }

    size_t vectorSize()
    {
        std::unique_lock lV(mVectorMutex);
        return mDescriptionAndMeasurementsVector.size();
    }

    /**
     * Adds a measurment and its description to the vector. Nothing gets added
     * if mIsFinished is false.
     */
    void addEntry(
        std::string                      description,
        std::shared_ptr<BenchmarkMetric> metric)
    {
        std::unique_lock lV(mVectorMutex);
        if (mIsFinished) {
            return;
        }
        mDescriptionAndMeasurementsVector.push_back(
            std::make_pair(description, metric));
        mElementCountCond.notify_all();
    }

    /**
     * Blocks the calling thread until mIsFinished is true (i.e. until all
     * automations are completed).
     *
     * @returns A copy of the vector of descriptions and measured metrics
     */
    VectorType getVectorBlocking()
    {
        std::unique_lock lV(mVectorMutex);
        while (!mIsFinished) {
            mFinishedCond.wait(lV);
        }
        VectorType copy = VectorType(mDescriptionAndMeasurementsVector);
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
        if (!mVectorMutex.try_lock()) {
            return std::nullopt;
        }
        if (!mIsFinished) {
            mVectorMutex.unlock();
            return std::nullopt;
        }
        std::optional<VectorType> copy =
            std::make_optional(mDescriptionAndMeasurementsVector);
        mVectorMutex.unlock();
        return copy;
    }

    /**
     * Blocks the calling thread until the Vector contains the requested index
     *
     * @note Check that the automations aren't finished and the vector size
     * before increasing index and calling again: it may remain blocked forever
     * otherwise
     *
     * @returns An optional copy of the element at the requested index. It is
     * only ever the null optional if the automations are finished and the index
     * ends up being out of range.
     */
    std::optional<VectorElementType> getVectorAtBlocking(size_t index)
    {
        std::unique_lock lV(mVectorMutex);
        while (mDescriptionAndMeasurementsVector.size() <= index &&
               !mIsFinished) {
            mElementCountCond.wait(lV);
        }
        if (mIsFinished && mDescriptionAndMeasurementsVector.size() <= index) {
            return std::nullopt;
        }
        return std::make_optional(mDescriptionAndMeasurementsVector[index]);
    }

    /**
     * Returns the null optional until the Vector contains the requested index
     * or if locking fails
     *
     * @note Check that the automations aren't finished and the vector size
     * before increasing index and calling again: you may otherwise loop forever
     * for no reason
     *
     * @returns An optional copy of the element at the requested index
     */
    std::optional<VectorElementType> getVectorAtNonBlocking(size_t index)
    {
        if (!mVectorMutex.try_lock()) {
            return std::nullopt;
        }
        if (mDescriptionAndMeasurementsVector.size() <= index) {
            mVectorMutex.unlock();
            return std::nullopt;
        }
        std::optional<VectorElementType> copy =
            std::make_optional(mDescriptionAndMeasurementsVector[index]);
        mVectorMutex.unlock();
        return copy;
    }
};

/**
 * Multithread-safe printer class that "prints" to a (or rather, stores in a)
 * vector. It is still not recommended to call the print() function from
 * multiple threads, since the first one to finish will  disable
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
