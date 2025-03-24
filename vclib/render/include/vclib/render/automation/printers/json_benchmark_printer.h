#ifndef JSON_BENCHMARK_PRINTER_H
#define JSON_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>
#include <fstream>
#include <format>

namespace vcl{

class JsonBenchmarkPrinter : public BenchmarkPrinter
{
    uint32_t loopCounter = 0;
    uint32_t automationIndex = 0;

    std::string fileName;
    std::ofstream stream;

    public:

    JsonBenchmarkPrinter(const std::string &fileName)
    : fileName{fileName}
    {
        stream.open(fileName);
        if(stream.fail()){
            throw "JsonBenchmarkPrinter : invalid file name\n";
        }
    };

    JsonBenchmarkPrinter(const JsonBenchmarkPrinter &other)
    : fileName{other.fileName},
    stream()
    {
        stream.open(fileName);
    };

    void onBenchmarkLoop()
    {
        loopCounter++;
        automationIndex = 0;
        stream << std::format("\n\t}},\n\t\"Loop {}\" : {{", loopCounter);
    };

    void print(BenchmarkMetric &metric)
    {
        if(loopCounter == 0 && automationIndex == 0)
        {
            stream << "{\n\t\"Loop 0\" : {";
        }

        if(automationIndex != 0)
        {
            stream << ",";
        }

        stream 
        << std::format("\n\t\t\"Automation {}\" : {{", automationIndex)
        << std::format("\n\t\t\t\"metric\" : \"{}\"", metric.getMeasureString()+metric.getUnitOfMeasure())
        << "\n\t\t}";

        automationIndex++;
    };

    void finish(BenchmarkMetric &metric)
    {
        stream << "\n\t}\n}";
        stream.close();
    };

    std::shared_ptr<BenchmarkPrinter> clone() const & override
    {
        return std::make_shared<JsonBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<JsonBenchmarkPrinter>(std::move(*this));
    };

    ~JsonBenchmarkPrinter()
    {
        if(stream.is_open())
        {
            stream.close();
        }
    };
};

}

#endif