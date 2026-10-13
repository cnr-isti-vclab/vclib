// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_LOGGER_ABSTRACT_LOGGER_H
#define VCL_BASE_LOGGER_ABSTRACT_LOGGER_H

#include <vclib/base/base.h>
#include <vclib/base/pointers.h>

#include <format>
#include <string_view>

namespace vcl {

/**
 * @brief The AbstractLogger class is used as common ancestor class for all the
 * logger types in the library.
 *
 * @ingroup base
 */
class AbstractLogger
{
public:
    enum class LogLevel {
        ERROR_LOG = 0,
        WARNING_LOG,
        MESSAGE_LOG,
        PROGRESS_LOG,
        DEBUG_LOG
    };

    using enum LogLevel;

    enum class ProgressMode { TIME, PERCENTAGE };

protected:
    bool     mIndent                 = true;
    bool     mPrintPerc              = true;
    bool     mPrintMsgDuringProgress = true;
    bool     mPrintTimer             = false;
    uint     mLineWidth              = 80;
    LogLevel mPrintLevel             = PROGRESS_LOG;

public:
    AbstractLogger()          = default;
    virtual ~AbstractLogger() = default;

    void enableIndentation() { mIndent = true; }

    void disableIndentation() { mIndent = false; }

    void enablePrintPercentage() { mPrintPerc = true; }

    void disablePrintPercentage() { mPrintPerc = false; }

    /**
     * @brief Sets the maximum print level of the logger.
     *
     * Only messages with a level less or equal to the given level will be
     * printed. For example, if the level is set to `LogLevel::MESSAGE_LOG`,
     * only messages with level `LogLevel::ERROR_LOG`, `LogLevel::WARNING_LOG`
     * and `LogLevel::MESSAGE_LOG` will be printed.
     *
     * @note The default level is `LogLevel::PROGRESS_LOG`.
     *
     * @param[in] level: The maximum print level of the logger.
     */
    void setPrintLevel(LogLevel level) { mPrintLevel = level; }

    void enablePrintMessageDuringProgress() { mPrintMsgDuringProgress = true; }

    void disablePrintMessageDuringProgress()
    {
        mPrintMsgDuringProgress = false;
    }

    void enablePrintTimer() { mPrintTimer = true; }

    void disablePrintTimer() { mPrintTimer = false; }

    virtual void reset() = 0;

    void setMaxLineWidth(uint width) { mLineWidth = width; }

    virtual void startTimer() = 0;

    virtual void stopTimer() = 0;

    /**
     * @brief Returns the time passed since the last call to `startTimer` member
     * function, or the time passed between the call to `startTimer` and the
     * call to `stopTimer` member functions. The time is expressed in seconds.
     *
     * @return The time passed, expressed in seconds.
     */
    virtual double time() const = 0;

    virtual void startNewTask(
        double           fromPerc,
        double           toPerc,
        std::string_view action) = 0;

    virtual void endTask(std::string_view action) = 0;

    virtual double percentage() const = 0;

    virtual void setPercentage(uint newPerc) = 0;

    /**
     * @brief Prints a message to the logger, with level `PROGRESS_LOG` and
     * without modifying the current percentage.
     *
     * @param[in] msg: The message to print.
     */
    void log(std::string_view msg) const { print(msg, PROGRESS_LOG); }

    /**
     * @brief Prints a message to the logger, with the given level and without
     * modifying the current percentage.
     *
     * @param[in] lvl: The level of the message.
     * @param[in] msg: The message to print.
     */
    void log(LogLevel lvl, std::string_view msg) const { print(msg, lvl); }

    /**
     * @brief Prints a message to the logger, with the given percentage and with
     * level `PROGRESS_LOG`.
     *
     * @param[in] perc: The percentage of the progress.
     * @param[in] msg: The message to print.
     */
    void log(uint perc, std::string_view msg)
    {
        print(perc, msg, PROGRESS_LOG);
    }

    /**
     * @brief Prints a message to the logger, with the given percentage and with
     * the given level.
     *
     * @param[in] perc: The percentage of the progress.
     * @param[in] lvl: The level of the message.
     * @param[in] msg: The message to print.
     */
    void log(uint perc, LogLevel lvl, std::string_view msg)
    {
        print(perc, msg, lvl);
    }

    /**
     * @brief Prints a formatted message to the logger, with level
     * `PROGRESS_LOG` and without modifying the current percentage.
     *
     * @param[in] fmt: The format string.
     * @param[in] args: The arguments to format.
     */
    template<typename... Args>
    requires (sizeof...(Args) > 0)
    void log(std::format_string<Args...> fmt, Args&&... args) const
    {
        print(std::format(fmt, std::forward<Args>(args)...), PROGRESS_LOG);
    }

    /**
     * @brief Prints a formatted message to the logger, with the given level
     * and without modifying the current percentage.
     *
     * @param[in] lvl: The level of the message.
     * @param[in] fmt: The format string.
     * @param[in] args: The arguments to format.
     */
    template<typename... Args>
    requires (sizeof...(Args) > 0)
    void log(LogLevel lvl, std::format_string<Args...> fmt, Args&&... args)
        const
    {
        print(std::format(fmt, std::forward<Args>(args)...), lvl);
    }

    /**
     * @brief Prints a formatted message to the logger, with level
     * `PROGRESS_LOG` and with the given percentage.
     *
     * @param[in] perc: The percentage of the progress.
     * @param[in] fmt: The format string.
     * @param[in] args: The arguments to format.
     */
    template<typename... Args>
    requires (sizeof...(Args) > 0)
    void log(uint perc, std::format_string<Args...> fmt, Args&&... args)
    {
        print(
            perc, std::format(fmt, std::forward<Args>(args)...), PROGRESS_LOG);
    }

    /**
     * @brief Prints a formatted message to the logger, with the given level
     * and with the given percentage.
     *
     * @param[in] perc: The percentage of the progress.
     * @param[in] lvl: The level of the message.
     * @param[in] fmt: The format string.
     * @param[in] args: The arguments to format.
     */
    template<typename... Args>
    requires (sizeof...(Args) > 0)
    void log(
        uint                        perc,
        LogLevel                    lvl,
        std::format_string<Args...> fmt,
        Args&&... args)
    {
        print(perc, std::format(fmt, std::forward<Args>(args)...), lvl);
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
     * @param[in] mode: the mode of the progress, either `ProgressMode::TIME` or
     * `ProgressMode::PERCENTAGE`, default `ProgressMode::TIME`
     * @param[in] printInterval: interval of time (in seconds) or percentage on
     * which print a progress message, depending on the `mode` argument, default
     * 1.0 seconds
     * @param[in] startPerc: start percentage of the progress, default 0%
     * @param[in] endPerc: end percentage of the progress, default 100%
     */
    virtual void startProgress(
        std::string_view msg,
        uint             progressSize,
        ProgressMode     mode          = ProgressMode::TIME,
        double           printInterval = 1.0,
        uint             startPerc     = 0,
        uint             endPerc       = 100) = 0;

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
    virtual void endProgress() = 0;

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
    virtual void progress(uint n) = 0;

protected:
    /**
     * @brief Prints a message to the logger with the given level and without
     * modifying the current percentage.
     *
     * @param[in] msg: The message to print.
     * @param[in] lvl: The level of the message.
     */
    virtual void print(std::string_view msg, LogLevel lvl = PROGRESS_LOG)
        const = 0;

    /**
     * @brief Prints a message to the logger with the given level and with the
     * given percentage.
     *
     * @param[in] perc: The percentage of the progress.
     * @param[in] msg: The message to print.
     * @param[in] lvl: The level of the message.
     */
    virtual void print(
        uint             perc,
        std::string_view msg,
        LogLevel         lvl = PROGRESS_LOG) = 0;
};

/* Concepts */

/**
 * @brief The LoggerConcept is satisfied if the type T is a valid Logger type.
 *
 * A valid Logger type must inherit from AbstractLogger, and therefore it must
 * provide all the pure virtual member functions to control the logging
 * behavior.
 *
 * @note The NullLogger class does satisfy this concept, by providing all the
 * required member functions as no-op.
 *
 * @ingroup base
 */
template<typename T>
concept LoggerConcept = std::derived_from<RemoveRef<T>, AbstractLogger>;

} // namespace vcl

#endif // VCL_BASE_LOGGER_ABSTRACT_LOGGER_H
