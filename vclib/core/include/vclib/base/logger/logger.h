// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_LOGGER_LOGGER_H
#define VCL_BASE_LOGGER_LOGGER_H

#include "abstract_logger.h"

#include <vclib/base/timer.h>

#include <cassert>
#include <cmath>
#include <iomanip>
#include <mutex>
#include <stack>
#include <sstream>

namespace vcl {

template<typename Stream>
class Logger : public AbstractLogger
{
    struct ProgressStatus
    {
        std::string message;
        uint        step = 1;
        uint        perc;
        uint        percStep = 10;
        uint        size = 1;
        uint        lastProgress = 0;
        uint        startPerc = 0;
        uint        endPerc = 100;
        bool        isActive = false;

        ProgressMode mode = ProgressMode::TIME;
        double       secondsStep = 1.0;
        Timer        progressTimer;
        uint         throttleCounter = 0;
    };

    enum InternalLogLevel { START = DEBUG_LOG + 1, END };

    static const uint TIMER_MAX_CHAR_COUNT = 12;

    std::recursive_mutex mMutex;

public:
    Logger()
    {
        mIntervals.push({0, 100});
        updateStep();
    }

    void reset() override final
    {
        while (!mIntervals.empty())
            mIntervals.pop();
        mIntervals.push({0, 100});
        updateStep();
    }

    void startTimer() override final { mTimer.start(); }

    void stopTimer() override final { mTimer.stop(); }

    double time() const override final { return mTimer.delay(); }

    void startNewTask(double fromPerc, double toPerc, const std::string& action)
        override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        printLine(action, START);

        assert(fromPerc >= 0);
        assert(toPerc <= 100);
        std::pair<double, double> actualP = mIntervals.top();
        std::pair<double, double> newP;
        newP.first =
            actualP.first + (actualP.second - actualP.first) * (fromPerc / 100);
        newP.second =
            actualP.first + (actualP.second - actualP.first) * (toPerc / 100);
        mGlobalPercProgress = newP.first;
        mIntervals.push(newP);
        updateStep();
    }

    void endTask(const std::string& action) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        mGlobalPercProgress = mIntervals.top().second;
        if (mIntervals.size() > 1) {
            mIntervals.pop();
            updateStep();

            printLine(action, END);
        }
    }

    double percentage() const override final
    {
        std::lock_guard<std::recursive_mutex> lock(const_cast<std::recursive_mutex&>(mMutex));
        double k = std::pow(10, mPercPrecision);
        uint   c = mGlobalPercProgress * k;
        return c / k;
    }

    virtual void setPercentage(uint newPerc) override
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (newPerc >= 0 && newPerc <= 100) {
            mGlobalPercProgress = (mIntervals.top().first) + mStep * newPerc;
        }
    }

    void log(const std::string& msg) const override final
    {
        std::lock_guard<std::recursive_mutex> lock(const_cast<std::recursive_mutex&>(mMutex));
        printLine(msg, PROGRESS_LOG);
    }

    void log(const std::string& msg, LogLevel lvl) const override final
    {
        std::lock_guard<std::recursive_mutex> lock(const_cast<std::recursive_mutex&>(mMutex));
        printLine(msg, lvl);
    }

    void log(uint perc, const std::string& msg) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        log(perc, msg, PROGRESS_LOG);
    }

    void log(uint perc, const std::string& msg, LogLevel lvl) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (perc >= 0 && perc <= 100)
            setPercentage(perc);

        printLine(msg, lvl);
    }

    void startProgress(
        const std::string& msg,
        uint               progressSize,
        ProgressMode       mode              = ProgressMode::TIME,
        double             printInterval     = 1.0,
        uint               startPerc         = 0,
        uint               endPerc           = 100) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        assert(printInterval > 0);
        assert((endPerc - startPerc) > 0);
        mProgress.isActive = true;
        mProgress.message  = msg;
        mProgress.size     = progressSize;
        mProgress.perc     = startPerc;
        mProgress.startPerc = startPerc;
        mProgress.endPerc   = endPerc;
        mProgress.mode      = mode;

        if (mode == ProgressMode::PERCENTAGE) {
            mProgress.percStep = static_cast<uint>(printInterval);
            if (mProgress.percStep == 0) mProgress.percStep = 1;
            mProgress.step =
                (progressSize + 1) / ((endPerc - startPerc) / mProgress.percStep);
            if (mProgress.step == 0)
                mProgress.step = progressSize;
        } else {
            mProgress.secondsStep = printInterval;
            mProgress.progressTimer.start();
            mProgress.throttleCounter = 0;
        }

        mProgress.lastProgress = 0;
    }

    void endProgress() override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (mProgress.isActive) {
            if (mProgress.perc < mProgress.endPerc) {
                mProgress.perc = mProgress.endPerc;
                if (mPrintMsgDuringProgress)
                    log(mProgress.perc, mProgress.message, PROGRESS_LOG);
                else
                    setPercentage(mProgress.perc);
            }
            mProgress.isActive = false;
        }
    }

    void progress(uint n) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (!mProgress.isActive)
            return;
        if (n > mProgress.size)
            n = mProgress.size;

        if (mProgress.mode == ProgressMode::TIME) {
            if (++mProgress.throttleCounter >= 64) {
                mProgress.throttleCounter = 0;
                double elapsed = mProgress.progressTimer.delay();
                if (elapsed >= mProgress.secondsStep) {
                    mProgress.perc = mProgress.startPerc + (n * (mProgress.endPerc - mProgress.startPerc)) / mProgress.size;
                    if (mProgress.perc > mProgress.endPerc)
                        mProgress.perc = mProgress.endPerc;

                    if (mPrintMsgDuringProgress)
                        log(mProgress.perc, mProgress.message, PROGRESS_LOG);
                    else
                        setPercentage(mProgress.perc);

                    mProgress.progressTimer.start();
                }
            }
        } else {
            uint progress = n / mProgress.step;
            if (mProgress.lastProgress < progress) {
                mProgress.perc = mProgress.startPerc + progress * mProgress.percStep;
                if (mProgress.perc > mProgress.endPerc)
                    mProgress.perc = mProgress.endPerc;
                if (mPrintMsgDuringProgress)
                    log(mProgress.perc, mProgress.message, PROGRESS_LOG);
                else
                    setPercentage(mProgress.perc);
                mProgress.lastProgress = progress;
            }
        }
    }

protected:
    /**
     * @brief Returns the stream corresponding to the given LogLevel.
     *
     * It should be implemented by the derived classes.
     *
     * @param[in] lvl: the LogLevel for which the stream is requested.
     * @return the stream corresponding to the given LogLevel.
     */
    virtual Stream* levelStream(LogLevel lvl) const = 0;

    virtual void flush(Stream& o) const {}

private:
    uint mPercPrecision = 0;

    // on top of the stack, we save the interval percentages of the current task
    // values are between 0 and 100
    std::stack<std::pair<double, double>> mIntervals;

    // actual percentage (0 - 100), that is in the interval in top of the stack
    double mGlobalPercProgress = 0;

    double mStep = 1; // the value that corresponds to 1% on the current task

    Timer mTimer;

    // progress status members
    ProgressStatus mProgress;

    void updateStep()
    {
        mStep = (mIntervals.top().second - mIntervals.top().first) / 100;
    }

    void printLine(const std::string& msg, uint lvl) const
    {
        if (!mPrintPerc && msg.empty())
            return;

        // lvl could be also InternalLogLevel BEGIN or END
        LogLevel l = PROGRESS_LOG;
        if (lvl <= DEBUG_LOG) {
            l = (LogLevel) lvl;
        }
        if (l > mPrintLevel)
            return;
        Stream* stream = levelStream(l);

        if (stream) {
            std::ostringstream ss;
            uint s = 0;

            if (mPrintPerc) {
                s = printPercentage(ss);
            }
            s += printIndentation(ss);
            printMessage(ss, msg, lvl, s);
            printElapsedTime(ss);
            ss << "\n";
            *stream << ss.str().c_str();
            flush(*stream);
        }
    }

    uint printPercentage(std::ostream& o) const
    {
        uint size = 3;
        if (mPercPrecision > 0)
            size += 1 + mPercPrecision;

        o << "[";
        o << std::right;
        o << std::setw(size);
        o << percentage() << "%]";
        return size + 3;
    }

    uint printIndentation(std::ostream& o) const
    {
        uint s = 0;
        if (mIndent) {
            uint n = mIntervals.size() - 1;
            for (uint i = 0; i < n; i++) {
                o << "  ";
                s += 2;
            }
        }
        return s;
    }

    void printMessage(std::ostream& o, const std::string& msg, uint lvl, uint n) const
    {
        uint maxMsgSize = mLineWidth - n;
        if (mPrintTimer)
            maxMsgSize -= TIMER_MAX_CHAR_COUNT;
        switch (lvl) {
        case LogLevel::ERROR_LOG:
            maxMsgSize -= 8;
            o << " ERROR: ";
            break;
        case LogLevel::WARNING_LOG:
            maxMsgSize -= 10;
            o << " WARNING: ";
            break;
        case LogLevel::PROGRESS_LOG:
            maxMsgSize -= 1;
            o << " ";
            break;
        case LogLevel::DEBUG_LOG:
            maxMsgSize -= 9;
            o << " (debug) ";
            break;
        case InternalLogLevel::START:
            maxMsgSize -= 7;
            o << " Start ";
            break;
        case InternalLogLevel::END:
            maxMsgSize -= 5;
            o << " End ";
            break;
        }
        o << std::setw(maxMsgSize);
        o << std::left;
        o << msg.c_str();
    }

    void printElapsedTime(std::ostream& o) const
    {
        if (mPrintTimer) {
            o << "[";
            o << std::setw(TIMER_MAX_CHAR_COUNT - 3);
            o << std::right;
            o << mTimer.delay() << "s]";
        }
    }
};

} // namespace vcl

#endif // VCL_BASE_LOGGER_LOGGER_H
