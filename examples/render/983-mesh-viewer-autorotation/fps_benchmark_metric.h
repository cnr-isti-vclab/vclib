#ifndef FPS_BENCHMARK_METRIC_H
#define FPS_BENCHMARK_METRIC_H

#include <vclib/misc/timer.h>
#include <string>
#include <format>
#include "benchmark_metric.h"

class FpsBenchmarkMetric : public BenchmarkMetric<double>
{
    vcl::Timer timer;
    double frames = 0;
    public:

    void start() override
    {
        timer.start();
        frames = 0;
    };

    void update() override
    {
        frames++;
    };

    double getMeasure() override
    {
        return frames / timer.delay();
    };

    std::string getMeasureString()
    {
        double measure = getMeasure();
        std::string ret = std::format("{:.3f}", measure);
        return ret;
    };

    void end() override
    {
        timer.stop();
    };
};


#endif