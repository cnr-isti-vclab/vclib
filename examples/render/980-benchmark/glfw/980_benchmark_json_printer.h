#ifndef VCLIB_980_BENCHMARK_JSON_PRINTER
#define VCLIB_980_BENCHMARK_JSON_PRINTER

#include <vclib/render/automation/printers/benchmark_printer.h>

#include <exception>
#include <fstream>
#include <sstream>
#include <vector>

namespace vcl {

class Benchmark980JsonPrinter : public BenchmarkPrinter
{
    bool mFirstLoop       = true;
    bool mFirstAutomation = true;
    bool mAppendMode      = true;

    std::vector<std::string> mMeasurements;

    std::string   mPrevFile;
    std::string   mFileName;
    std::string   mDeviceName;
    std::string   mMeshName;
    std::string   mShadingType;
    std::string   mSplittingType;
    std::string   mColoringType;
    std::string   mResolution;
    std::ofstream mStream;

public:
    Benchmark980JsonPrinter(
        const std::string& fileName,
        const std::string& deviceName,
        const std::string& meshName,
        const std::string& shadingType,
        const std::string& splittingType,
        const std::string& coloringType,
        const std::string& resolution) :
            mFileName {fileName}, mDeviceName {deviceName},
            mShadingType {shadingType}, mSplittingType {splittingType},
            mColoringType {coloringType}, mResolution {resolution},
            mMeshName {meshName}
    {
        std::string   entireFile = "";
        std::ifstream tempStream(fileName);
        if (tempStream.fail()) {
            mAppendMode = false;
        }
        else {
            for (std::string lineStr; std::getline(tempStream, lineStr);) {
                entireFile = entireFile + lineStr + "\n";
            }
            if (entireFile.size() == 0) {
                mAppendMode = false;
            }
            while (entireFile.size() > 0) {
                std::string popped;
                popped = entireFile.back();
                entireFile.pop_back();
                if (popped == "]") {
                    break;
                }
            }
        }
        tempStream.close();
        mStream.open(fileName);
        if (mStream.fail()) {
            throw std::invalid_argument(
                "Benchmark980JsonPrinter: invalid filename");
        }
        mPrevFile = entireFile;
    }

    Benchmark980JsonPrinter(const Benchmark980JsonPrinter& other) :
            BenchmarkPrinter(other), mFileName {other.mFileName}, mStream(),
            mMeasurements {other.mMeasurements},
            mAppendMode {other.mAppendMode}, mDeviceName {other.mDeviceName},
            mMeshName {other.mMeshName}, mShadingType {other.mShadingType},
            mSplittingType {other.mSplittingType},
            mColoringType {other.mColoringType}, mResolution {other.mResolution}, mPrevFile{other.mPrevFile}
    {
        mStream.open(mFileName);
    };

    void onBenchmarkLoop() override { mFirstLoop = false; };

    void print(const BenchmarkMetric& metric, const std::string& description)
        override
    {
        std::vector<std::string> resultVec = metric.getMeasureStrings();
        mMeasurements.insert(
            mMeasurements.end(), resultVec.begin(), resultVec.end());
    }

    void finish() override
    {
        mStream << mPrevFile;
        if (mAppendMode) {
            mStream << "\t,";
        }
        if (!mAppendMode) {
            mStream << "[";
        }
        mStream << "\n\t{"
                << "\n\t\t\"Device\" : \"" << mDeviceName << "\","
                << "\n\t\t\"Mesh\" : \"" << mMeshName << "\","
                << "\n\t\t\"Splitting\" : \"" << mSplittingType << "\","
                << "\n\t\t\"Shading\" : \"" << mShadingType << "\","
                << "\n\t\t\"Coloring\" : \"" << mColoringType << "\","
                << "\n\t\t\"Resolution\" : \"" << mResolution << "\","
                << "\n\t\t\"Framerates\" : [";
        bool isFirst = true;
        for (auto meas : mMeasurements) {
            if(!isFirst) {
                mStream << ",";
            }else{
                isFirst = false;
            }
            mStream << "\n\t\t\t" << meas;
        }
        mStream << "\n\t\t]\n\t}\n]";
        mStream.close();
    }

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<Benchmark980JsonPrinter>(*this);
    }

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<Benchmark980JsonPrinter>(std::move(*this));
    }
};

} // namespace vcl
#endif
