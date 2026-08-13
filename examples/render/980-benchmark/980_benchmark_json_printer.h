#ifndef VCLIB_980_BENCHMARK_JSON_PRINTER
#define VCLIB_980_BENCHMARK_JSON_PRINTER

#include <vclib/render/automation/printers/benchmark_printer.h>

#include <exception>
#include <fstream>
#include <sstream>
#include <vector>

namespace vcl {

struct ____StringPairHasher {
    size_t operator()(const std::pair<std::string, std::string>& p) const noexcept {
        size_t h1 = std::hash<std::string>{}(p.first);
        size_t h2 = std::hash<std::string>{}(p.second);
        return h1 ^ h2;
    }
};

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
            mFileName {other.mFileName}, mStream(),
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

class Benchmark980JsonPrinterVarying : public BenchmarkPrinter
{
    bool mFirstLoop       = true;
    bool mFirstAutomation = true;
    bool mAppendMode      = true;

    // pair shading type,coloring type
    std::unordered_map<std::pair<std::string,std::string>,std::vector<std::string>,____StringPairHasher> mMeasurements;
    std::vector<std::pair<std::string,std::string>> mKeyOrder;

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
    Benchmark980JsonPrinterVarying(
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

    Benchmark980JsonPrinterVarying(const Benchmark980JsonPrinterVarying& other) :
            mFileName {other.mFileName}, mStream(),
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
        static const std::set<std::string> group1 = {
            "col face",
            "col vert",
            "col mesh",
            "col user",
            "tex vert",
            "tex wedge"
        };
        static const std::set<std::string> group2 = {
            "flat",
            "smooth",
            "none"
        };
        static const std::set<std::string> group3 = {
            "visible",
            "unknown"
        };
        std::string possibleShad = metric.getUnitOfMeasure();
        if (group1.contains(possibleShad)) {
            mColoringType = possibleShad;
            return;
        }
        if (group2.contains(possibleShad)) {
            mShadingType = possibleShad;
            return;
        }
        if (group3.contains(possibleShad)) {
            mShadingType = "unknown";
            mColoringType = "unknown";
            return;
        }
        if (possibleShad == "__skip") {
            return;
        }
        std::vector<std::string> resultVec = metric.getMeasureStrings();
        auto p = std::make_pair(mShadingType, mColoringType);
        if (mKeyOrder.empty() || mKeyOrder.back() != p) {
            mKeyOrder.push_back(p);
        }
        std::vector<std::string>& vecRef = mMeasurements[std::make_pair(mShadingType, mColoringType)];
        vecRef.insert(vecRef.end(), resultVec.begin(), resultVec.end());
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
        bool isFirstMapEl = true;
        for (auto& key: mKeyOrder) {
            auto& vec = mMeasurements[key];
            std::string shadingType = key.first;
            std::string coloringType = key.second;
            if (!isFirstMapEl) {
                mStream << ",";
            } else {
                isFirstMapEl = false;
            }
            mStream << "\n\t{"
                    << "\n\t\t\"Device\" : \"" << mDeviceName << "\","
                    << "\n\t\t\"Mesh\" : \"" << mMeshName << "\","
                    << "\n\t\t\"Splitting\" : \"" << mSplittingType << "\","
                    << "\n\t\t\"Shading\" : \"" << shadingType << "\","
                    << "\n\t\t\"Coloring\" : \"" << coloringType << "\","
                    << "\n\t\t\"Resolution\" : \"" << mResolution << "\","
                    << "\n\t\t\"Framerates\" : [";
            
            // MEASUREMENTS
            bool isFirst = true;
            for (auto meas : vec) {
                if(!isFirst) {
                    mStream << ",";
                }else{
                    isFirst = false;
                }
                mStream << "\n\t\t\t" << meas;
            }
            mStream << "\n\t\t]";
            // END MEASUREMENTS
            mStream << "\n\t}";
        }
        mStream << "\n]";
        mStream.close();
    }

    std::shared_ptr<BenchmarkPrinter> clone() const& override
    {
        return std::make_shared<Benchmark980JsonPrinterVarying>(*this);
    }

    std::shared_ptr<BenchmarkPrinter> clone() && override
    {
        return std::make_shared<Benchmark980JsonPrinterVarying>(std::move(*this));
    }
};

std::shared_ptr<BenchmarkPrinter> get980Printer(
    const std::string& fileName,
    const std::string& deviceName,
    const std::string& meshName,
    const std::string& shadingType,
    const std::string& splittingType,
    const std::string& coloringType,
    const std::string& resolution,
    bool isVarying) 
{
    if (isVarying) {
        return Benchmark980JsonPrinterVarying(
            fileName,
            deviceName,
            meshName,
            shadingType,
            splittingType,
            coloringType,
            resolution
        ).clone();
    }
    return Benchmark980JsonPrinter(
        fileName,
        deviceName,
        meshName,
        shadingType,
        splittingType,
        coloringType,
        resolution
    ).clone();
};

} // namespace vcl
#endif
