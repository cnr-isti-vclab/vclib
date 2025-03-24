#ifndef NULL_BENCHMARK_PRINTER_H
#define NULL_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>

namespace vcl{

/*
    Class which doesn't write anything anywhere
*/
class NullBenchmarkPrinter : public BenchmarkPrinter
{
    public:

    void print(BenchmarkMetric &metric) override {};

    void onBenchmarkLoop() override {};

    void finish(BenchmarkMetric &metric) override {};

    std::shared_ptr<BenchmarkPrinter> clone() const & override
    {
        return std::make_shared<NullBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<NullBenchmarkPrinter>(std::move(*this));
    };
};

}

#endif