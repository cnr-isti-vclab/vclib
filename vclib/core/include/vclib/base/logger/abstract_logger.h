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
     * printed. For example, if the level is set to `LogLevel::MESSAGE_LOG`, only
     * messages with level `LogLevel::ERROR_LOG`, `LogLevel::WARNING_LOG` and
     * `LogLevel::MESSAGE_LOG` will be printed.
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

    void log(std::string_view msg, LogLevel lvl = PROGRESS_LOG) const
    {
        print(msg, lvl);
    }

    void log(LogLevel lvl, std::string_view msg) const
    {
        print(msg, lvl);
    }

    void log(uint perc, std::string_view msg, LogLevel lvl = PROGRESS_LOG)
    {
        print(perc, msg, lvl);
    }

    void log(uint perc, LogLevel lvl, std::string_view msg)
    {
        print(perc, msg, lvl);
    }

    template<typename... Args>
        requires(sizeof...(Args) > 0)
    void log(std::format_string<Args...> fmt, Args&&... args) const
    {
        print(std::format(fmt, std::forward<Args>(args)...), PROGRESS_LOG);
    }

    template<typename... Args>
        requires(sizeof...(Args) > 0)
    void log(LogLevel lvl, std::format_string<Args...> fmt, Args&&... args) const
    {
        print(std::format(fmt, std::forward<Args>(args)...), lvl);
    }

    template<typename... Args>
        requires(sizeof...(Args) > 0)
    void log(uint perc, std::format_string<Args...> fmt, Args&&... args)
    {
        print(perc, std::format(fmt, std::forward<Args>(args)...), PROGRESS_LOG);
    }

    template<typename... Args>
        requires(sizeof...(Args) > 0)
    void log(
        uint                        perc,
        LogLevel                    lvl,
        std::format_string<Args...> fmt,
        Args&&... args)
    {
        print(perc, std::format(fmt, std::forward<Args>(args)...), lvl);
    }

    /**
     * @brief Starts a new progress tracking block.
     *
     * @param[in] msg: the message that will be printed during the progress
     * @param[in] progressSize: the number of iterations made during the progress
     * @param[in] mode: the mode of the progress (TIME or PERCENTAGE)
     * @param[in] printInterval: interval of time or percentage to print
     * @param[in] startPerc: start percentage of the progress
     * @param[in] endPerc: end percentage of the progress
     */
    virtual void startProgress(
        std::string_view msg,
        uint             progressSize,
        ProgressMode     mode          = ProgressMode::TIME,
        double           printInterval = 1.0,
        uint             startPerc     = 0,
        uint             endPerc       = 100) = 0;

    /**
     * @brief Ends the current progress.
     */
    virtual void endProgress() = 0;

    /**
     * @brief Increments the current progress.
     *
     * @param[in] n: iteration number of the current progress.
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
    virtual void print(
        std::string_view msg,
        LogLevel         lvl = PROGRESS_LOG) const = 0;

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
