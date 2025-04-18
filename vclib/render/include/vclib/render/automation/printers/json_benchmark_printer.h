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
#include <sstream>

namespace vcl {

/**
 * The JsonBenchmarkPrinter class is a BenchmarkPrinter that writes the results
 * of a BenchmarkMetric to a json file.
 *
 * The file will have the following format:
 * <PRE>
 * {
 *      "Loop <loop_number>": {
 *          "Automation <automation_number>": {
 *              "measurements": [
 *                  ...
 *              ]
 *          },
 *          ...
 *      },
 *      ...
 * }
 * </PRE>
 */
class JsonBenchmarkPrinter : public BenchmarkPrinter
{
    bool mFirstLoop       = true;
    bool mFirstAutomation = true;

    std::string   mFileName;
    std::ofstream mStream;

public:
    JsonBenchmarkPrinter(const std::string& fileName) : mFileName {fileName}
    {
        mStream.open(fileName);
        if (mStream.fail()) {
            throw "JsonBenchmarkPrinter : invalid file name\n";
        }
    };

    JsonBenchmarkPrinter(const JsonBenchmarkPrinter& other) :
            mFileName {other.mFileName}, mStream()
    {
        mStream.open(mFileName);
    };

    void onBenchmarkLoop() override { mFirstLoop = false; };

    void print(const BenchmarkMetric& metric, const std::string& description)
        override
    {
        if (mFirstLoop && mFirstAutomation) {
            mStream << "[";
        }

        if (!mFirstAutomation) {
            mStream << ",";
        }

        std::ostringstream temp;

        temp << "[";

        bool isFirst = true;
        for (auto meas : metric.getMeasureStrings()) {
            if (!isFirst) {
                temp << ",";
            }
            else {
                isFirst = false;
            }
            temp << "\n\t\t\t\"" << meas << metric.getUnitOfMeasure() << "\"";
        }
        temp << "\n\t\t]";

        mStream << "\n\t{";

        mStream << "\n\t\t\"Description\" : \"" << description << "\""
                << "\n\t\t\"Measurements\" : " << temp.str() << "\n\t}";

        mFirstAutomation = false;
    };

    void finish() override
    {
        mStream << "\n]";
        mStream.close();
    };

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<JsonBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<JsonBenchmarkPrinter>(std::move(*this));
    };

    ~JsonBenchmarkPrinter()
    {
        if (mStream.is_open()) {
            mStream.close();
        }
    };
};

} // namespace vcl

#endif
