#ifndef BENCHMARK_PRINTER_H
#define BENCHMARK_PRINTER_H

#include <string>
#include <memory>
#include "benchmark_metric.h"

class BenchmarkPrinter
{
    public:

    virtual void onBenchmarkLoop() = 0;
    virtual void print(BenchmarkMetric &metric) = 0;
    virtual void finish(BenchmarkMetric &metric) = 0;

    virtual std::shared_ptr<BenchmarkPrinter> clone() const & = 0;
    virtual std::shared_ptr<BenchmarkPrinter> clone() && = 0;
};

#endif