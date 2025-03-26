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

#ifndef VCL_CSV_BENCHMARK_PRINTER_H
#define VCL_CSV_BENCHMARK_PRINTER_H

#include <vclib/render/automation/printers/benchmark_printer.h>
#include <fstream>

namespace vcl{

/*
    Class which writes the metric's results to a csv file
*/
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