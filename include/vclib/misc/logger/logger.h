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

    uint percPrecision = 0;

    // on top of the stack, we save the interval percentages of the current task
    // values are between 0 and 100
    std::stack<std::pair<double, double>> stack;

    // actual percentage (0 - 100), that is in the interval in top of the stack
    double globalPercProgress;

    double step; // the value that corresponds to 1% on the current task

    bool       indent     = true;
    uint       lineW      = 80;
    const uint TIMER_SIZE = 12;

    vcl::Timer timer;
    bool       printTimer = false;

    // progress status members
    bool        isProgressActive = false;
    std::string progressMessage;
    uint        progressStep;
    uint        progressPerc;
    uint        progressPercStep;
    uint        lastProgress;

    std::mutex mutex;

public:
    Logger()
    {
        stack.push({0, 100});
        updateStep();
    }

    void enableIndentation() { indent = true; }

    void disableIndentation() { indent = false; }

    void reset()
    {
        while (!stack.empty())
            stack.pop();
        stack.push({0, 100});
        updateStep();
    }

    void setMaxLineWidth(uint w) { lineW = w; }

    void setPrintTimer(bool b) { printTimer = b; }

    void startTimer() { timer.start(); }

    void startNewTask(double fromPerc, double toPerc, const std::string& action)
    {
        printLine(action, START);

        assert(fromPerc >= 0);
        assert(toPerc <= 100);
        std::pair<double, double> newP;
        newP.first =
            stack.top().first +
            (stack.top().second - stack.top().first) * (fromPerc / 100);
        newP.second = (stack.top().second - stack.top().first) * (toPerc / 100);
        globalPercProgress = newP.first;
        stack.push(newP);
        updateStep();
    }

    void endTask(const std::string& action)
    {
        globalPercProgress = stack.top().second;
        if (stack.size() > 1) {
            stack.pop();
            updateStep();

            printLine(action, END);
        }
    }

    double percentage() const
    {
        double k = std::pow(10, percPrecision);
        uint   c = globalPercProgress * k;
        return c / k;
    }

    virtual void setPercentage(uint newPerc)
    {
        if (newPerc >= 0 && newPerc <= 100) {
            globalPercProgress = (stack.top().first) + step * newPerc;
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
        isProgressActive = true;
        progressMessage  = msg;
        progressPerc     = startPerc;
        progressPercStep = percPrintProgress;
        progressStep =
            progressSize / ((endPerc - startPerc) / percPrintProgress - 1);
        if (progressStep == 0)
            progressStep = progressSize;
        lastProgress = 0;
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
    void endProgress() { isProgressActive = false; }

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
        mutex.lock();
        assert(isProgressActive);
        uint progress = n / progressStep;
        if (lastProgress < progress) {
            progressPerc = progress * progressPercStep;
            log(progressPerc, progressMessage);
            lastProgress = progress;
        }
        mutex.unlock();
    }

protected:
    // you should override this member function if you want to use a different
    // stream that are not std::cout and std::cerr
    virtual Stream* levelStream(LogLevel lvl) = 0;

private:
    void updateStep() { step = (stack.top().second - stack.top().first) / 100; }

    void printLine(const std::string& msg, uint lvl)
    {
        LogLevel l = PROGRESS;
        if (lvl < DEBUG) {
            l = (LogLevel) lvl;
        }
        std::ostream* stream = levelStream(l);

        if (stream) {
            uint s = 0;
            s      = printPercentage(*stream);
            s += printIndentation(*stream);
            printMessage(*stream, msg, lvl, s);
            printElapsedTime(*stream);
            *stream << std::endl;
        }
    }

    uint printPercentage(std::ostream& o) const
    {
        uint size = 3;
        if (percPrecision > 0)
            size += 1 + percPrecision;

        o << "[";
        o << std::right << std::setw(size) << percentage() << "%]";
        return size + 3;
    }

    uint printIndentation(std::ostream& o) const
    {
        uint s = 0;
        if (indent) {
            uint n = stack.size() - 1;
            for (uint i = 0; i < n; i++) {
                o << "  ";
                s += 2;
            }
        }
        return s;
    }

    void printMessage(std::ostream& o, const std::string& msg, uint lvl, uint n)
    {
        uint maxMsgSize = lineW - n;
        if (printTimer)
            maxMsgSize -= TIMER_SIZE;
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

    void printElapsedTime(std::ostream& o) const
    {
        if (printTimer) {
            o << "[" << std::setw(TIMER_SIZE - 3) << std::right << timer.delay()
              << "s]";
        }
    }
};

} // namespace vcl

#endif // VCL_MISC_LOGGER_LOGGER_H
