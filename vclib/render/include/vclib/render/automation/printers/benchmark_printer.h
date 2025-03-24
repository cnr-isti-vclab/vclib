#ifndef VCL_BENCHMARK_PRINTER_H
#define VCL_BENCHMARK_PRINTER_H

#include <string>
#include <memory>
#include <vclib/render/automation/metrics/benchmark_metric.h>

namespace vcl{

/*
    Abstract class whose children define how you should write (display, print, ...) the metrics measured from
    automations
*/
class BenchmarkPrinter
{
    public:

    virtual void onBenchmarkLoop() = 0;
    virtual void print(BenchmarkMetric &metric) = 0;
    virtual void finish(BenchmarkMetric &metric) = 0;

    virtual std::shared_ptr<BenchmarkPrinter> clone() const & = 0;
    virtual std::shared_ptr<BenchmarkPrinter> clone() && = 0;
};

}

#endif