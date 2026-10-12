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
#include <format>
#include <mutex>
#include <stack>
#include <string>
#include <string_view>

namespace vcl {

class Logger : public AbstractLogger
{
    struct ProgressStatus
    {
        std::string message;
        uint        step = 1;
        uint        perc;
        uint        percStep     = 10;
        uint        size         = 1;
        uint        lastProgress = 0;
        uint        currentN     = 0;
        uint        startPerc    = 0;
        uint        endPerc      = 100;
        bool        isActive     = false;

        ProgressMode mode        = ProgressMode::TIME;
        double       secondsStep = 1.0;
        Timer        progressTimer;
        uint         throttleCounter = 0;
    };

    enum InternalLogLevel {
        START = static_cast<uint>(DEBUG_LOG) + 1,
        END
    };

    static const uint TIMER_MAX_CHAR_COUNT = 12;

    std::recursive_mutex mMutex;

    uint mPercPrecision = 0;

    // on top of the stack, we save the interval percentages of the current task
    // values are between 0 and 100
    std::stack<std::pair<double, double>> mIntervals;

    // actual percentage (0 - 100), that is in the interval in top of the stack
    double mGlobalPercProgress = 0;

    double mStep = 1; // the value that corresponds to 1% on the current task

    Timer mTimer;

    // progress status stack
    std::stack<ProgressStatus> mProgresses;

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

    void startNewTask(
        double           fromPerc,
        double           toPerc,
        std::string_view action) override final
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

    void endTask(std::string_view action) override final
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
        std::lock_guard<std::recursive_mutex> lock(
            const_cast<std::recursive_mutex&>(mMutex));
        double k = std::pow(10, mPercPrecision);
        uint   c = mGlobalPercProgress * k;
        return c / k;
    }

    virtual void setPercentage(uint newPerc) override
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (newPerc <= 100) {
            mGlobalPercProgress = (mIntervals.top().first) + mStep * newPerc;
        }
    }

    void startProgress(
        std::string_view msg,
        uint             progressSize,
        ProgressMode     mode          = ProgressMode::TIME,
        double           printInterval = 1.0,
        uint             startPerc     = 0,
        uint             endPerc       = 100) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        assert(printInterval > 0);
        assert((endPerc - startPerc) > 0);

        ProgressStatus prog;
        prog.isActive  = true;
        prog.message   = std::string(msg);
        prog.size      = progressSize;
        prog.startPerc = startPerc;
        prog.endPerc   = endPerc;
        prog.perc      = startPerc;
        prog.mode      = mode;
        prog.currentN  = 0;

        if (!mProgresses.empty() && mProgresses.top().isActive) {
            auto& parent = mProgresses.top();
            uint  p_start =
                parent.startPerc +
                (parent.currentN * (parent.endPerc - parent.startPerc)) /
                    parent.size;
            uint p_end =
                parent.startPerc +
                ((parent.currentN + 1) * (parent.endPerc - parent.startPerc)) /
                    parent.size;

            prog.startPerc = p_start + (startPerc * (p_end - p_start)) / 100;
            prog.endPerc   = p_start + (endPerc * (p_end - p_start)) / 100;
            prog.perc      = prog.startPerc;
        }

        if (mode == ProgressMode::PERCENTAGE) {
            uint unscaledDiff     = endPerc - startPerc;
            uint unscaledPercStep = static_cast<uint>(printInterval);
            if (unscaledPercStep == 0)
                unscaledPercStep = 1;

            if (unscaledDiff == 0) {
                prog.step = progressSize;
            }
            else {
                uint diff_step = unscaledDiff / unscaledPercStep;
                if (diff_step == 0)
                    diff_step = 1;
                prog.step = (progressSize + 1) / diff_step;
                if (prog.step == 0)
                    prog.step = progressSize;
            }
        }
        else {
            prog.secondsStep = printInterval;
            prog.progressTimer.start();
            prog.throttleCounter = 0;
        }

        prog.lastProgress = 0;
        mProgresses.push(prog);
    }

    void endProgress() override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (!mProgresses.empty()) {
            ProgressStatus& prog = mProgresses.top();
            if (prog.isActive) {
                if (prog.perc < prog.endPerc) {
                    prog.perc = prog.endPerc;
                    if (mPrintMsgDuringProgress)
                        print(prog.perc, prog.message, PROGRESS_LOG);
                    else
                        setPercentage(prog.perc);
                }
                prog.isActive = false;
            }
            mProgresses.pop();
        }
    }

    void progress(uint n) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (mProgresses.empty())
            return;
        ProgressStatus& prog = mProgresses.top();
        if (!prog.isActive)
            return;
        prog.currentN = n;
        if (n > prog.size)
            n = prog.size;

        if (prog.mode == ProgressMode::TIME) {
            if (++prog.throttleCounter >= 64) {
                prog.throttleCounter = 0;
                double elapsed       = prog.progressTimer.delay();
                if (elapsed >= prog.secondsStep) {
                    prog.perc =
                        prog.startPerc +
                        (n * (prog.endPerc - prog.startPerc)) / prog.size;
                    if (prog.perc > prog.endPerc)
                        prog.perc = prog.endPerc;

                    if (mPrintMsgDuringProgress)
                        print(prog.perc, prog.message, PROGRESS_LOG);
                    else
                        setPercentage(prog.perc);

                    prog.progressTimer.start();
                }
            }
        }
        else {
            uint progress = n / prog.step;
            if (prog.lastProgress < progress) {
                prog.perc = prog.startPerc +
                            (n * (prog.endPerc - prog.startPerc)) / prog.size;
                if (prog.perc > prog.endPerc)
                    prog.perc = prog.endPerc;
                if (mPrintMsgDuringProgress)
                    print(prog.perc, prog.message, PROGRESS_LOG);
                else
                    setPercentage(prog.perc);
                prog.lastProgress = progress;
            }
        }
    }

protected:
    /**
     * @brief Writes a formatted log line to the destination sink.
     *
     * Must be implemented by derived classes.
     *
     * @param[in] line: the formatted line to emit.
     * @param[in] lvl: the LogLevel corresponding to the line.
     */
    virtual void write(std::string_view line, LogLevel lvl) const = 0;

    /**
     * @brief Flushes the output sink.
     */
    virtual void flush() const {}

private:
    void print(
        std::string_view msg,
        LogLevel         lvl = PROGRESS_LOG) const override final
    {
        std::lock_guard<std::recursive_mutex> lock(
            const_cast<std::recursive_mutex&>(mMutex));
        printLine(msg, static_cast<uint>(lvl));
    }

    void print(
        uint             perc,
        std::string_view msg,
        LogLevel         lvl = PROGRESS_LOG) override final
    {
        std::lock_guard<std::recursive_mutex> lock(mMutex);
        if (perc <= 100)
            setPercentage(perc);

        printLine(msg, static_cast<uint>(lvl));
    }

    void updateStep()
    {
        mStep = (mIntervals.top().second - mIntervals.top().first) / 100;
    }

    void printLine(std::string_view msg, uint lvl) const
    {
        if (!mPrintPerc && msg.empty())
            return;

        LogLevel l = PROGRESS_LOG;
        if (lvl <= static_cast<uint>(DEBUG_LOG)) {
            l = static_cast<LogLevel>(lvl);
        }
        if (l > mPrintLevel)
            return;

        std::string line;
        uint        s = 0;

        if (mPrintPerc) {
            uint width = 3;
            if (mPercPrecision > 0)
                width += 1 + mPercPrecision;
            if (mPercPrecision == 0) {
                line += std::format(
                    "[{:>{}}%]", static_cast<uint>(percentage()), width);
            }
            else {
                line += std::format(
                    "[{:>{}.{}f}%]", percentage(), width, mPercPrecision);
            }
            s += width + 3;
        }

        if (mIndent && mIntervals.size() > 1) {
            uint indentSpaces = (mIntervals.size() - 1) * 2;
            line.append(indentSpaces, ' ');
            s += indentSpaces;
        }

        std::string_view levelPrefix;
        uint             prefixLen = 0;
        switch (lvl) {
        case static_cast<uint>(ERROR_LOG):
            levelPrefix = " ERROR: ";
            prefixLen   = 8;
            break;
        case static_cast<uint>(WARNING_LOG):
            levelPrefix = " WARNING: ";
            prefixLen   = 10;
            break;
        case static_cast<uint>(MESSAGE_LOG):
        case static_cast<uint>(PROGRESS_LOG):
            levelPrefix = " ";
            prefixLen   = 1;
            break;
        case static_cast<uint>(DEBUG_LOG):
            levelPrefix = " (debug) ";
            prefixLen   = 9;
            break;
        case InternalLogLevel::START:
            levelPrefix = " Start ";
            prefixLen   = 7;
            break;
        case InternalLogLevel::END:
            levelPrefix = " End ";
            prefixLen   = 5;
            break;
        }

        line += levelPrefix;
        s += prefixLen;

        uint maxMsgSize = mLineWidth > s ? mLineWidth - s : 0;
        if (mPrintTimer && maxMsgSize >= TIMER_MAX_CHAR_COUNT) {
            maxMsgSize -= TIMER_MAX_CHAR_COUNT;
        }

        if (msg.size() < maxMsgSize) {
            line += msg;
            line.append(maxMsgSize - msg.size(), ' ');
        }
        else {
            line += msg;
        }

        if (mPrintTimer) {
            line += std::format(
                "[{:>{}.3f}s]", mTimer.delay(), TIMER_MAX_CHAR_COUNT - 3);
        }

        write(line, l);
        flush();
    }
};

} // namespace vcl

#endif // VCL_BASE_LOGGER_LOGGER_H
