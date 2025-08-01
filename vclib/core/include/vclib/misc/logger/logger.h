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

#ifndef VCL_MISC_LOGGER_LOGGER_H
#define VCL_MISC_LOGGER_LOGGER_H

#include "abstract_logger.h"

#include <vclib/base.h>

#include <cassert>
#include <cmath>
#include <mutex>
#include <stack>

namespace vcl {

template<typename Stream>
class Logger : public AbstractLogger
{
    struct ProgressStatus
    {
        std::string message;
        uint        step;
        uint        perc;
        uint        percStep;
        uint        size;
        uint        lastProgress;
        bool        isActive = false;
    };

    enum InternalLogLevel { START = DEBUG_LOG + 1, END };

    static const uint TIMER_MAX_CHAR_NUMBER = 12;

    uint mPercPrecision = 0;

    // on top of the stack, we save the interval percentages of the current task
    // values are between 0 and 100
    std::stack<std::pair<double, double>> mIntervals;

    // actual percentage (0 - 100), that is in the interval in top of the stack
    double mGlobalPercProgress = 0;

    double mStep = 1; // the value that corresponds to 1% on the current task

    uint mLineWidth = 80;

    Timer mTimer;

    LogLevel mPrintLevel = PROGRESS_LOG;

    // progress status members
    ProgressStatus mProgress;

    // settings
    bool mPrintPerc              = true;
    bool mPrintMsgDuringProgress = true;
    bool mIndent                 = true;
    bool mPrintTimer             = false;

    std::mutex mMutex;

public:
    Logger()
    {
        mIntervals.push({0, 100});
        updateStep();
    }

    void enableIndentation() override final { mIndent = true; }

    void disableIndentation() override final { mIndent = false; }

    void enablePrintPercentage() override final { mPrintPerc = true; }

    void disablePrintPercentage() override final { mPrintPerc = false; }

    void setPrintLevel(LogLevel level) override final { mPrintLevel = level; }

    void enablePrintMessageDuringProgress() override final
    {
        mPrintMsgDuringProgress = true;
    }

    void disablePrintMessageDuringProgress() override final
    {
        mPrintMsgDuringProgress = false;
    }

    void enablePrintTimer() override final { mPrintTimer = true; }

    void disablePrintTimer() override final { mPrintTimer = false; }

    void reset() override final
    {
        while (!mIntervals.empty())
            mIntervals.pop();
        mIntervals.push({0, 100});
        updateStep();
    }

    void setMaxLineWidth(uint w) override final { mLineWidth = w; }

    void startTimer() override final { mTimer.start(); }

    void stopTimer() override final { mTimer.stop(); }

    double time() const override final { return mTimer.delay(); }

    void startNewTask(double fromPerc, double toPerc, const std::string& action)
        override final
    {
        printLine(action, START);

        assert(fromPerc >= 0);
        assert(toPerc <= 100);
        std::pair<double, double> actualP = mIntervals.top();
        std::pair<double, double> newP;
        newP.first =
            actualP.first + (actualP.second - actualP.first) * (fromPerc / 100);
        newP.second         = (actualP.second - actualP.first) * (toPerc / 100);
        mGlobalPercProgress = newP.first;
        mIntervals.push(newP);
        updateStep();
    }

    void endTask(const std::string& action) override final
    {
        mGlobalPercProgress = mIntervals.top().second;
        if (mIntervals.size() > 1) {
            mIntervals.pop();
            updateStep();

            printLine(action, END);
        }
    }

    double percentage() const override final
    {
        double k = std::pow(10, mPercPrecision);
        uint   c = mGlobalPercProgress * k;
        return c / k;
    }

    virtual void setPercentage(uint newPerc) override
    {
        if (newPerc >= 0 && newPerc <= 100) {
            mGlobalPercProgress = (mIntervals.top().first) + mStep * newPerc;
        }
    }

    void log(const std::string& msg) const override final
    {
        printLine(msg, PROGRESS_LOG);
    }

    void log(const std::string& msg, LogLevel lvl) const override final
    {
        printLine(msg, lvl);
    }

    void log(uint perc, const std::string& msg) override final
    {
        log(perc, msg, PROGRESS_LOG);
    }

    void log(uint perc, const std::string& msg, LogLevel lvl) override final
    {
        if (perc >= 0 && perc <= 100)
            setPercentage(perc);

        printLine(msg, lvl);
    }

    void startProgress(
        const std::string& msg,
        uint               progressSize,
        uint               percPrintProgress = 10,
        uint               startPerc         = 0,
        uint               endPerc           = 100) override final
    {
        assert(percPrintProgress > 0);
        assert((endPerc - startPerc) > 0);
        mProgress.isActive = true;
        mProgress.message  = msg;
        mProgress.size     = progressSize;
        mProgress.perc     = startPerc;
        mProgress.percStep = percPrintProgress;
        mProgress.step =
            (progressSize + 1) / ((endPerc - startPerc) / percPrintProgress);
        if (mProgress.step == 0)
            mProgress.step = progressSize;
        mProgress.lastProgress = 0;
    }

    void endProgress() override final
    {
        progress(mProgress.size);
        mProgress.isActive = false;
    }

    void progress(uint n) override final
    {
        mMutex.lock();
        assert(mProgress.isActive);
        uint progress = n / mProgress.step;
        if (mProgress.lastProgress < progress) {
            mProgress.perc = progress * mProgress.percStep;
            if (mPrintMsgDuringProgress)
                log(mProgress.perc, mProgress.message, PROGRESS_LOG);
            else
                setPercentage(mProgress.perc);
            mProgress.lastProgress = progress;
        }
        mMutex.unlock();
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

    virtual void alignLeft(Stream& o) const {}

    virtual void alignRight(Stream& o) const {}

    virtual void setWidth(Stream& o, uint w) const {}

    virtual void flush(Stream& o) const {}

private:
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
            uint s = 0;

            if (mPrintPerc) {
                s = printPercentage(*stream);
            }
            s += printIndentation(*stream);
            printMessage(*stream, msg, lvl, s);
            printElapsedTime(*stream);
            *stream << "\n";
        }
        flush(*stream);
    }

    uint printPercentage(Stream& o) const
    {
        uint size = 3;
        if (mPercPrecision > 0)
            size += 1 + mPercPrecision;

        o << "[";
        alignRight(o);
        setWidth(o, size);
        o << percentage() << "%]";
        return size + 3;
    }

    uint printIndentation(Stream& o) const
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

    void printMessage(Stream& o, const std::string& msg, uint lvl, uint n) const
    {
        uint maxMsgSize = mLineWidth - n;
        if (mPrintTimer)
            maxMsgSize -= TIMER_MAX_CHAR_NUMBER;
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
        setWidth(o, maxMsgSize);
        alignLeft(o);
        o << msg.c_str();
    }

    void printElapsedTime(Stream& o) const
    {
        if (mPrintTimer) {
            o << "[";
            setWidth(o, TIMER_MAX_CHAR_NUMBER - 3);
            alignRight(o);
            o << mTimer.delay() << "s]";
        }
    }
};

} // namespace vcl

#endif // VCL_MISC_LOGGER_LOGGER_H
