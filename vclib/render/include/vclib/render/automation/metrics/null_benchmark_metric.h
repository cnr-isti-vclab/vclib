#ifndef VCL_NULL_BENCHMARK_METRIC_H
#define VCL_NULL_BENCHMARK_METRIC_H

#include <string>
#include <format>
#include <vclib/render/automation/metrics/benchmark_metric.h>

namespace vcl{

/*
    Measures nothing
*/
class NullBenchmarkMetric : public BenchmarkMetric
{
    public:

    void start() override {};

    void measure() override {};

    std::string getMeasureString() { return ""; };

    std::string getUnitOfMeasure() { return ""; }

    void end() override {};

    std::shared_ptr<BenchmarkMetric> clone() const &
    {
        return std::make_shared<NullBenchmarkMetric>(*this);
    };

    std::shared_ptr<BenchmarkMetric> clone()  &&
    {
        return std::make_shared<NullBenchmarkMetric>(std::move(*this));
    };
};

}

#endif