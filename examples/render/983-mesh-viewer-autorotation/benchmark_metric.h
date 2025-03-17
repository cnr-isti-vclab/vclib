#ifndef BENCHMARK_METRIC_H
#define BENCHMARK_METRIC_H

template<typename MeasureType>
class BenchmarkMetric
{
    public:
    
    virtual void start() = 0;
    virtual void update() = 0;
    virtual MeasureType getMeasure() = 0;
    virtual void end() = 0;
};

#endif