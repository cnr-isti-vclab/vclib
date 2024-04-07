/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <stack>

#include <vclib/misc/timer.h>
#include <vclib/types.h>

namespace vcl {

template<typename Stream>
class Logger
{
public:
    enum LogLevel { ERROR = 0, WARNING, PROGRESS, DEBUG };

private:
    enum InternalLogLevel { START = DEBUG + 1, END };

    static const uint TIMER_MAX_CHAR_NUMBER = 12;

    uint mPercPrecision = 0;

    // on top of the stack, we save the interval percentages of the current task
    // values are between 0 and 100
    std::stack<std::pair<double, double>> mIntervals;

    // actual percentage (0 - 100), that is in the interval in top of the stack
    double mGlobalPercProgress = 0;

    double mStep = 1; // the value that corresponds to 1% on the current task

    bool mIndent    = true;
    uint mLineWidth = 80;

    vcl::Timer mTimer;
    bool       mPrintTimer = false;

    // progress status members
    bool        mIsProgressActive = false;
    std::string mProgressMessage;
    uint        mProgressStep;
    uint        mProgressPerc;
    uint        mProgressPercStep;
    uint        mLastProgress;

    std::mutex mMutex;

public:
    Logger()
    {
        mIntervals.push({0, 100});
        updateStep();
    }

    void enableIndentation() { mIndent = true; }

    void disableIndentation() { mIndent = false; }

    void reset()
    {
        while (!mIntervals.empty())
            mIntervals.pop();
        mIntervals.push({0, 100});
        updateStep();
    }

    void setMaxLineWidth(uint w) { mLineWidth = w; }

    void setPrintTimer(bool b) { mPrintTimer = b; }

    void startTimer() { mTimer.start(); }

    void startNewTask(double fromPerc, double toPerc, const std::string& action)
    {
        printLine(action, START);

        assert(fromPerc >= 0);
        assert(toPerc <= 100);
        std::pair<double, double> newP;
        newP.first = mIntervals.top().first +
                     (mIntervals.top().second - mIntervals.top().first) *
                         (fromPerc / 100);
        newP.second =
            (mIntervals.top().second - mIntervals.top().first) * (toPerc / 100);
        mGlobalPercProgress = newP.first;
        mIntervals.push(newP);
        updateStep();
    }

    void endTask(const std::string& action)
    {
        mGlobalPercProgress = mIntervals.top().second;
        if (mIntervals.size() > 1) {
            mIntervals.pop();
            updateStep();

            printLine(action, END);
        }
    }

    double percentage() const
    {
        double k = std::pow(10, mPercPrecision);
        uint   c = mGlobalPercProgress * k;
        return c / k;
    }

    virtual void setPercentage(uint newPerc)
    {
        if (newPerc >= 0 && newPerc <= 100) {
            mGlobalPercProgress = (mIntervals.top().first) + mStep * newPerc;
        }
    }

    void log(const std::string& msg) { log(101, PROGRESS, msg); }

    void log(LogLevel lvl, const std::string& msg) { log(101, lvl, msg); }

    void log(uint perc, const std::string& msg) { log(perc, PROGRESS, msg); }

    void log(uint perc, LogLevel lvl, const std::string& msg)
    {
        if (perc >= 0 && perc <= 100)
            setPercentage(perc);

        printLine(msg, lvl);
    }

    /**
     * @brief Allows to easily manage progresses with the logger, along with the
     * `progress` and `endProgress` member functions.
     *
     * This logger functionality should be used when processing a loop having a
     * fixed size, with regular prints of the progress.
     *
     * This member function starts a new progress.
     * With the default arguments, will print a message from 0% to 100%, every
     * 10%.
     *
     * The typical usage is the following:
     *
     * @code{.cpp}
     * log.startProgress("Computing...", vec.size());
     *
     * for (uint i = 0; i < vec.size(); ++i) {
     *     // make computations
     *     log.progress(i); // will print only every 10% of progress
     * }
     * log.endProgress();
     *
     * @endcode
     *
     * @param[in] msg: the message that will be printed during the progress
     * @param[in] progressSize: the number of iterations made during the
     * progress
     * @param[in] percPrintProgress: interval of percentage on which print a
     * progress message, default 10%
     * @param[in] startPerc: start percentage of the progress, default 0%
     * @param[in] endPerc: end percentage of the progress, default 100%
     */
    void startProgress(
        const std::string& msg,
        uint               progressSize,
        uint               percPrintProgress = 10,
        uint               startPerc         = 0,
        uint               endPerc           = 100)
    {
        assert(percPrintProgress > 0);
        assert((endPerc - startPerc) > 0);
        mIsProgressActive = true;
        mProgressMessage  = msg;
        mProgressPerc     = startPerc;
        mProgressPercStep = percPrintProgress;
        mProgressStep =
            progressSize / ((endPerc - startPerc) / percPrintProgress - 1);
        if (mProgressStep == 0)
            mProgressStep = progressSize;
        mLastProgress = 0;
    }

    /**
     * @brief Allows to easily manage progresses with the logger, along with the
     * `startProgress` and `progress` member functions.
     *
     * This logger functionality should be used when processing a loop having a
     * fixed size, with regular prints of the progress.
     *
     * This member function ends the current progress.
     *
     * The typical usage is the following:
     *
     * @code{.cpp}
     * uint s = vec.size();
     * log.startProgress("Computing...", s);
     *
     * for (uint i = 0; i < vec.size(); ++i) {
     *     // make computations
     *     log.progress(i); // will print only every 10% of progress
     * }
     * log.endProgress();
     *
     * @endcode
     */
    void endProgress() { mIsProgressActive = false; }

    /**
     * @brief Allows to easily manage progresses with the logger, along with the
     * `startProgress` and `endProgress` member functions.
     *
     * This logger functionality should be used when processing a loop having a
     * fixed size, with regular prints of the progress.
     *
     * This member functions increments the current progress. Only if the
     * current percentage reaches a new step percentage (set in the
     * `startProgress` member function), a message will be printed by the
     * logger.
     *
     * The typical usage is the following:
     *
     * @code{.cpp}
     * uint s = vec.size();
     * log.startProgress("Computing...", s);
     *
     * for (uint i = 0; i < vec.size(); ++i) {
     *     // make computations
     *     log.progress(i); // will print only every 10% of progress
     * }
     * log.endProgress();
     *
     * @endcode
     *
     * @param[in] n: iteration number of the current progress. It must be less
     * than the `progressSize` argument of the `startProgress` member function.
     */
    void progress(uint n)
    {
        mMutex.lock();
        assert(mIsProgressActive);
        uint progress = n / mProgressStep;
        if (mLastProgress < progress) {
            mProgressPerc = progress * mProgressPercStep;
            log(mProgressPerc, mProgressMessage);
            mLastProgress = progress;
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
    virtual Stream* levelStream(LogLevel lvl) = 0;

private:
    void updateStep()
    {
        mStep = (mIntervals.top().second - mIntervals.top().first) / 100;
    }

    void printLine(const std::string& msg, uint lvl)
    {
        LogLevel l = PROGRESS;
        if (lvl < DEBUG) {
            l = (LogLevel) lvl;
        }
        Stream* stream = levelStream(l);

        if (stream) {
            uint s = 0;
            s      = printPercentage(*stream);
            s += printIndentation(*stream);
            printMessage(*stream, msg, lvl, s);
            printElapsedTime(*stream);
            *stream << std::endl;
        }
    }

    uint printPercentage(Stream& o) const
    {
        uint size = 3;
        if (mPercPrecision > 0)
            size += 1 + mPercPrecision;

        o << "[";
        o << std::right << std::setw(size) << percentage() << "%]";
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

    void printMessage(Stream& o, const std::string& msg, uint lvl, uint n)
    {
        uint maxMsgSize = mLineWidth - n;
        if (mPrintTimer)
            maxMsgSize -= TIMER_MAX_CHAR_NUMBER;
        switch (lvl) {
        case ERROR:
            maxMsgSize -= 8;
            o << " ERROR: ";
            break;
        case WARNING:
            maxMsgSize -= 10;
            o << " WARNING: ";
            break;
        case PROGRESS:
            maxMsgSize -= 1;
            o << " ";
            break;
        case DEBUG:
            maxMsgSize -= 9;
            o << " (debug) ";
            break;
        case START:
            maxMsgSize -= 7;
            o << " Start ";
            break;
        case END:
            maxMsgSize -= 5;
            o << " End ";
            break;
        }
        o << std::setw(maxMsgSize) << std::left << msg;
    }

    void printElapsedTime(Stream& o) const
    {
        if (mPrintTimer) {
            o << "[" << std::setw(TIMER_MAX_CHAR_NUMBER - 3) << std::right
              << mTimer.delay() << "s]";
        }
    }
};

} // namespace vcl

#endif // VCL_MISC_LOGGER_LOGGER_H
