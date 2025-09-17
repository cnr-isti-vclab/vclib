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

#ifndef VCL_CSV_BENCHMARK_PRINTER_NO_DESC_H
#define VCL_CSV_BENCHMARK_PRINTER_NO_DESC_H

#include <vclib/render/automation/printers/benchmark_printer.h>

#include <fstream>
#include <iterator>
#include <sstream>
#include <utility>

namespace vcl {

class CsvBenchmarkPrinterShaderChange : public BenchmarkPrinter
{
    uint                                        mLoopCounter       = 0;
    uint                                        mAutomationCounter = 0;
    uint                                        maxMeasurementSize = 0;
    uint                                        newlineEvery       = 0;
    std::string                                 mFileNameUber;
    std::string                                 mFileNameSplit;
    std::string                                 mFileNameUberIf;
    std::ofstream                               mUberStream;
    std::ofstream                               mSplitStream;
    std::ofstream                               mUberIfStream;
    std::vector<std::pair<std::string, size_t>> mMeasurementStrings;

    void openStreams()
    {
        mUberStream.open(mFileNameUber, std::ios_base::app);
        if (mUberStream.fail()) {
            throw "CsvBenchmarkPrinter : invalid file name\n";
        }
        mSplitStream.open(mFileNameSplit, std::ios_base::app);
        if (mSplitStream.fail()) {
            throw "CsvBenchmarkPrinter : invalid file name\n";
        }
        mUberIfStream.open(mFileNameUberIf, std::ios_base::app);
        if (mUberIfStream.fail()) {
            throw "CsvBenchmarkPrinter : invalid file name\n";
        }
    }

    void closeStreams()
    {
        if (mUberStream.is_open()) {
            mUberStream.close();
        }
        if (mSplitStream.is_open()) {
            mSplitStream.close();
        }
        if (mUberIfStream.is_open()) {
            mUberIfStream.close();
        }
    }

public:
    CsvBenchmarkPrinterShaderChange(
        const std::string& fileNameUber,
        const std::string& fileNameSplit,
        const std::string& fileNameUberIf,
        uint               newlineEvery) :
            mFileNameUber {fileNameUber}, mFileNameSplit {fileNameSplit},
            mFileNameUberIf {fileNameUberIf}, newlineEvery {newlineEvery}
    {
        openStreams();
    };

    CsvBenchmarkPrinterShaderChange(
        const CsvBenchmarkPrinterShaderChange& other) :
            mFileNameUber {other.mFileNameUber},
            mFileNameSplit {other.mFileNameSplit},
            mFileNameUberIf {other.mFileNameUberIf},
            newlineEvery {other.newlineEvery}
    {
        openStreams();
    };

    void onBenchmarkLoop() override
    {
        mLoopCounter++;
        mAutomationCounter = 0;
    };

    void print(const BenchmarkMetric& metric, const std::string& description)
        override
    {
        std::ostringstream       temp;
        std::vector<std::string> measureStrings = metric.getMeasureStrings();

        maxMeasurementSize =
            std::max(maxMeasurementSize, (uint32_t) measureStrings.size());

        if (metric.getMeasureStrings().size() > 0) {
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
        uint counter = 0;
        for (const auto& meas : mMeasurementStrings) {
            std::ofstream* usedStream;
            switch (counter / newlineEvery) {
            case 0: usedStream = &mUberStream; break;
            case 1: usedStream = &mSplitStream; break;
            case 2: usedStream = &mUberIfStream; break;
            }
            *usedStream << meas.first
                        << std::string(maxMeasurementSize - meas.second, ';')
                        << "\n";
            counter++;
        }
        closeStreams();
    };

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<CsvBenchmarkPrinterShaderChange>(*this);
    };

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<CsvBenchmarkPrinterShaderChange>(
            std::move(*this));
    };

    ~CsvBenchmarkPrinterShaderChange() { closeStreams(); };
};

} // namespace vcl

#endif
