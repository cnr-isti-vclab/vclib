#ifndef TIME_BENCHMARK_METRIC_H
#define TIME_BENCHMARK_METRIC_H

#include "benchmark_metric.h"
#include <format>
#include <vclib/misc/timer.h>

class TimeBenchmarkMetric : public BenchmarkMetric
{
    vcl::Timer timer;

    public:

    void start() override
    {
        timer.start();
    }

    void update() override {}

    std::string getMeasureString() override
    {
        return std::format("{:.3f}", timer.delay());
    }

    std::string getUnitOfMeasure() override
    {
        return "s";
    }

    void end() override
    {
        timer.stop();
    }

    std::shared_ptr<BenchmarkMetric> clone() const & override
    {
        return std::make_shared<TimeBenchmarkMetric>(*this);
    }
    std::shared_ptr<BenchmarkMetric> clone() && override
    {
        return std::make_shared<TimeBenchmarkMetric>(std::move(*this));
    };
};

#endif