#ifndef BENCHMARK_METRIC_H
#define BENCHMARK_METRIC_H

#include <string>

class BenchmarkMetric
{
    public:
    
    virtual void start() = 0;
    virtual void measure() = 0;
    virtual std::string getMeasureString() = 0;
    virtual std::string getUnitOfMeasure() = 0;
    virtual void end() = 0;

    virtual std::shared_ptr<BenchmarkMetric> clone() const & = 0;
    virtual std::shared_ptr<BenchmarkMetric> clone()  && = 0;
};

#endif