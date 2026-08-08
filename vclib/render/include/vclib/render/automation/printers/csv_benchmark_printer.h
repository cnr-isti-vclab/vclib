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

#include <exception>
#include <fstream>
#include <iterator>
#include <sstream>
#include <utility>

namespace vcl {

/**
 * The CsvBenchmarkPrinter class is a BenchmarkPrinter that writes the results
 * of a BenchmarkMetric to a CSV file that uses ; as a separator.
 *
 * The CSV file will have the following columns:
 * - Description: A description of the automation
 * - Measurement 0: first measurement for the automation
 * - ...
 * - Measurement n: the last measurement for the automation
 */
class CsvBenchmarkPrinter : public BenchmarkPrinter
{
    uint                                        mLoopCounter       = 0;
    uint                                        mAutomationCounter = 0;
    uint                                        maxMeasurementSize = 0;
    std::string                                 mFileName;
    std::ofstream                               mStream;
    std::vector<std::pair<std::string, size_t>> mMeasurementStrings;
    bool                                        mPrintHeader        = true;
    bool                                        mAppend             = false;
    bool                                        mPrintDescription   = true;
    bool                                        mPrintUnitOfMeasure = true;

public:
    CsvBenchmarkPrinter(const std::string& fileName, bool append = false) :
            mFileName {fileName}, mAppend {append}
    {
        if (append) {
            mStream.open(fileName, std::ios_base::app);
        }
        else {
            mStream.open(fileName);
        }
        if (mStream.fail()) {
            throw std::invalid_argument(
                "CsvBenchmarkPrinter: invalid filename");
        }
    };

    CsvBenchmarkPrinter(const CsvBenchmarkPrinter& other) :
            BenchmarkPrinter(other), mFileName {other.mFileName}, mStream(),
            mPrintHeader(other.mPrintHeader), mAppend(other.mAppend),
            mPrintDescription(other.mPrintDescription),
            mPrintUnitOfMeasure(other.mPrintUnitOfMeasure)
    {
        if (mAppend) {
            mStream.open(mFileName, std::ios_base::app);
        }
        else {
            mStream.open(mFileName);
        }
    };

    void onBenchmarkLoop() override
    {
        mLoopCounter++;
        mAutomationCounter = 0;
    };

    void print(const BenchmarkMetric& metric, const std::string& description)
        override
    {
        std::ostringstream temp;
        if (mPrintDescription) {
            temp << description;
        }

        std::vector<std::string> measureStrings = metric.getMeasureStrings();

        maxMeasurementSize =
            std::max(maxMeasurementSize, (uint32_t) measureStrings.size());

        if (mPrintUnitOfMeasure) {
            for (size_t i = 0; i < measureStrings.size(); i++) {
                measureStrings[i] =
                    measureStrings[i] + metric.getUnitOfMeasure();
            }
        }

        if (metric.getMeasureStrings().size() > 0) {
            if (mPrintDescription) {
                temp << ";";
            }
            std::copy(
                measureStrings.begin(),
                measureStrings.end() - 1,
                std::ostream_iterator<std::string>(temp, ";"));
            temp << measureStrings.back();
        }

        mMeasurementStrings.push_back(
            std::make_pair(temp.str(), measureStrings.size()));

        mAutomationCounter++;
    };

    void finish() override
    {
        if (mPrintHeader) {
            if (mPrintDescription) {
                mStream << "Description";
            }
            for (uint32_t i = 0; i < maxMeasurementSize; i++) {
                if (i != 0 || mPrintDescription) {
                    mStream << ";";
                }
                mStream << "Measurement " << i;
            }
        }
        bool firstLoop = true;
        for (const auto& meas : mMeasurementStrings) {
            if (!firstLoop || mPrintHeader) {
                mStream << "\n";
            }
            mStream << meas.first
                    << std::string(maxMeasurementSize - meas.second, ';');
            firstLoop = false;
        }
        mStream << std::endl;
        mStream.close();
    };

    void useHeader(bool b = true) { mPrintHeader = b; }

    void useDescription(bool b = true) { mPrintDescription = b; }

    void useUnitOfMeasure(bool b = true) { mPrintUnitOfMeasure = b; }

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<CsvBenchmarkPrinter>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<CsvBenchmarkPrinter>(std::move(*this));
    };

    ~CsvBenchmarkPrinter()
    {
        if (mStream.is_open()) {
            mStream.close();
        }
    };
};

} // namespace vcl

#endif
