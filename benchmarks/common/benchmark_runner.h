// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BENCHMARKS_BENCHMARK_RUNNER_H
#define VCL_BENCHMARKS_BENCHMARK_RUNNER_H

#include <vclib/base.h>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <numeric>
#include <print>
#include <string>
#include <vector>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

namespace bench {

template<typename T>
inline void doNotOptimize(const T& val)
{
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "r,m"(val) : "memory");
#else
    const volatile auto* p = &val;
    (void) p;
#endif
}

template<typename F>
inline void invokeBenchmarkFunc(F&& f)
{
    if constexpr (!std::is_void_v<std::invoke_result_t<F>>) {
        auto res = f();
        doNotOptimize(res);
    }
    else {
        f();
    }
}

inline size_t getPeakRSS()
{
#if defined(__APPLE__) && defined(__MACH__)
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
    return static_cast<size_t>(rusage.ru_maxrss / 1024); // KB
#elif defined(__unix__)
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
    return static_cast<size_t>(rusage.ru_maxrss); // KB
#else
    return 0;
#endif
}

struct BenchmarkResult
{
    std::string         name;
    int                 iterations = 0;
    std::vector<double> runsMs;
    double              minMs     = 0.0;
    double              maxMs     = 0.0;
    double              avgMs     = 0.0;
    double              medianMs  = 0.0;
    double              peakRssMb = 0.0;
};

class BenchmarkRunner
{
public:
    BenchmarkRunner(
        std::string    version,
        std::string    datasetName,
        nlohmann::json metadata          = nlohmann::json::object(),
        int            defaultIterations = 5,
        bool           warmup            = true) :
            mVersion(std::move(version)), mDatasetName(std::move(datasetName)),
            mMetadata(std::move(metadata)),
            mDefaultIterations(defaultIterations), mWarmup(warmup)
    {
    }

    template<typename Func>
    void run(const std::string& name, Func&& func, int iterations = -1)
    {
        if (iterations <= 0) {
            iterations = mDefaultIterations;
        }

        std::println("Running {:<35} ({} iters)...", name, iterations);

        if (mWarmup) {
            invokeBenchmarkFunc(func);
        }

        BenchmarkResult result;
        result.name       = name;
        result.iterations = iterations;
        result.runsMs.reserve(iterations);

        vcl::Timer timer("", false);
        for (int i = 0; i < iterations; ++i) {
            timer.start();
            invokeBenchmarkFunc(func);
            timer.stop();
            result.runsMs.push_back(timer.delay() * 1000.0); // ms
        }

        // Stats
        result.minMs =
            *std::min_element(result.runsMs.begin(), result.runsMs.end());
        result.maxMs =
            *std::max_element(result.runsMs.begin(), result.runsMs.end());
        result.avgMs =
            std::accumulate(result.runsMs.begin(), result.runsMs.end(), 0.0) /
            static_cast<double>(iterations);

        std::vector<double> sorted = result.runsMs;
        std::sort(sorted.begin(), sorted.end());
        if (iterations % 2 == 1) {
            result.medianMs = sorted[iterations / 2];
        }
        else {
            result.medianMs =
                0.5 * (sorted[iterations / 2 - 1] + sorted[iterations / 2]);
        }

        result.peakRssMb = static_cast<double>(getPeakRSS()) / 1024.0;

        std::println(
            "  -> median: {:8.3f} ms (min: {:8.3f} ms, max: {:8.3f} ms) | Peak "
            "RSS: {:.2f} MB",
            result.medianMs,
            result.minMs,
            result.maxMs,
            result.peakRssMb);

        mResults.push_back(std::move(result));
    }

    bool saveJson(const std::string& filepath) const
    {
        nlohmann::json j;
        j["version"]  = mVersion;
        j["dataset"]  = mDatasetName;
        j["metadata"] = mMetadata;

        nlohmann::json benchList = nlohmann::json::array();
        for (const auto& r : mResults) {
            nlohmann::json item;
            item["name"]           = r.name;
            item["iterations"]     = r.iterations;
            item["time_min_ms"]    = r.minMs;
            item["time_max_ms"]    = r.maxMs;
            item["time_avg_ms"]    = r.avgMs;
            item["time_median_ms"] = r.medianMs;
            item["peak_rss_mb"]    = r.peakRssMb;
            benchList.push_back(item);
        }
        j["benchmarks"] = benchList;

        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::println(
                stderr, "Error opening output JSON file: {}", filepath);
            return false;
        }
        file << j.dump(2) << std::endl;
        std::println("Saved benchmark results to JSON: {}", filepath);
        return true;
    }

private:
    std::string                  mVersion;
    std::string                  mDatasetName;
    nlohmann::json               mMetadata;
    int                          mDefaultIterations = 5;
    bool                         mWarmup            = true;
    std::vector<BenchmarkResult> mResults;
};

} // namespace bench

#endif // VCL_BENCHMARKS_BENCHMARK_RUNNER_H
