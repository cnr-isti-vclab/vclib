/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_JSON_BENCHMARK_PRINTER_H
#define VCL_JSON_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>
#include <fstream>
#include <format>

namespace vcl{

/*
    Class which writes the metric's results to a json file
*/
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