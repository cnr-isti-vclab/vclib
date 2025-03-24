#ifndef CSV_BENCHMARK_PRINTER_H
#define CSV_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>
#include <fstream>

namespace vcl{

class CsvBenchmarkPrinter : public BenchmarkPrinter
{
    std::string fileName;
    std::ofstream stream;
    bool firstInRow = true;

    public:

    CsvBenchmarkPrinter(const std::string &fileName)
    : fileName{fileName}
    {
        stream.open(fileName);
        if(stream.fail()){
            throw "CsvBenchmarkPrinter : invalid file name\n";
        }
    };

    CsvBenchmarkPrinter(const CsvBenchmarkPrinter &other)
    : fileName{other.fileName},
    firstInRow{other.firstInRow},
    stream()
    {
        stream.open(fileName);
    };

    void onBenchmarkLoop()
    {
        stream << "\n";
        firstInRow = true;
    };

    void print(BenchmarkMetric &metric) override
    {
        if(!firstInRow){
            stream << ",";
        }else{
            firstInRow = false;
        }
        stream << metric.getMeasureString()+metric.getUnitOfMeasure();
    };

    void finish(BenchmarkMetric &metric) override
    {
        stream.close();
    };

    std::shared_ptr<BenchmarkPrinter> clone() const & override
    {
        return std::make_shared<CsvBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<CsvBenchmarkPrinter>(std::move(*this));
    };

    ~CsvBenchmarkPrinter()
    {
        if(stream.is_open())
        {
            stream.close();
        }
    };
};

}

#endif