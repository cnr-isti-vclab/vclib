#ifndef FPS_BENCHMARK_METRIC_H
#define FPS_BENCHMARK_METRIC_H

#include <vclib/misc/timer.h>
#include <string>
#include <format>
#include <vclib/render/automation/metrics/benchmark_metric.h>

namespace vcl{

class FpsBenchmarkMetric : public BenchmarkMetric
{
    Timer timer;
    double frames = 0;

    public:

    void start() override
    {
        timer.start();
        frames = 0;
    };

    void measure() override
    {
        frames++;
    };

    std::string getMeasureString()
    {
        return std::format("{:.3f}", frames / timer.delay());
    };

    std::string getUnitOfMeasure()
    {
        return "fps";
    }

    void end() override
    {
        timer.stop();
    };

    std::shared_ptr<BenchmarkMetric> clone() const &
    {
        return std::make_shared<FpsBenchmarkMetric>(*this);
    };
    std::shared_ptr<BenchmarkMetric> clone()  &&
    {
        return std::make_shared<FpsBenchmarkMetric>(std::move(*this));
    };
};

}

#endif