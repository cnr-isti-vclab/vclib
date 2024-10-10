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

#ifndef VCL_MISC_LOGGER_ABSTRACT_LOGGER_H
#define VCL_MISC_LOGGER_ABSTRACT_LOGGER_H

#include <string>

#include <vclib/concepts/logger.h>

namespace vcl {

/**
 * @brief The AbstractLogger class is used as common ancestor class for all the
 * logger types in the library.
 *
 * @ingroup miscellaneous
 */
class AbstractLogger
{
public:
    enum LogLevel { ERROR = 0, WARNING, MESSAGE, PROGRESS, DEBUG };

    AbstractLogger() = default;

    virtual void enableIndentation() = 0;

    virtual void disableIndentation() = 0;

    virtual void enablePrintPercentage() = 0;

    virtual void disablePrintPercentage() = 0;

    /**
     * @brief Sets the maximum print level of the logger.
     *
     * Only messages with a level less or equal to the given level will be
     * printed. For example, if the level is set to `LogLevel::MESSAGE`, only
     * messages with level `LogLevel::ERROR`, `LogLevel::WARNING` and
     * `LogLevel::MESSAGE` will be printed.
     *
     * @note The default level is `LogLevel::PROGRESS`.
     *
     * @param[in] level: The maximum print level of the logger.
     */
    virtual void setPrintLevel(LogLevel level) = 0;

    virtual void enablePrintMessageDuringProgress() = 0;

    virtual void disablePrintMessageDuringProgress() = 0;

    virtual void enablePrintTimer() = 0;

    virtual void disablePrintTimer() = 0;

    virtual void reset() = 0;

    virtual void setMaxLineWidth(uint width) = 0;

    virtual void startTimer() = 0;

    virtual void stopTimer() = 0;

    /**
     * @brief Returns the time passed since the last call to `startTimer` member
     * function, or the time passed between the call to `startTimer` and the
     * call to `stopTimer` member functions. The time is expressed in seconds.
     *
     * @return The time passed, expressed in seconds.
     */
    virtual double getTime() = 0;

    virtual void startNewTask(
        double             fromPerc,
        double             toPerc,
        const std::string& action) = 0;

    virtual void endTask(const std::string& action) = 0;

    virtual double percentage() const = 0;

    virtual void setPercentage(uint newPerc) = 0;

    /**
     * @brief Prints a message to the logger, with level `LogLevel::PROGRESS`
     * and without modifying the current percentage.
     *
     * @param[in] msg: The message to print.
     */
    virtual void log(const std::string& msg) = 0;

    /**
     * @brief Prints a message to the logger, with the given level and without
     * modifying the current percentage.
     *
     * @param[in] lvl: The level of the message.
     * @param[in] msg: The message to print.
     */
    virtual void log(LogLevel lvl, const std::string& msg) = 0;

    /**
     * @brief Prints a message to the logger, with the level
     * `LogLevel::PROGRESS` and with the given percentage.
     *
     * @param[in] perc: The percentage of the progress.
     * @param[in] msg: The message to print.
     */
    virtual void log(uint perc, const std::string& msg) = 0;

    /**
     * @brief Prints a message to the logger, with the given level and with the
     * given percentage.
     *
     * @param[in] perc: The percentage of the progress.
     * @param[in] lvl: The level of the message.
     * @param[in] msg: The message to print.
     */
    virtual void log(uint perc, LogLevel lvl, const std::string& msg) = 0;

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
    virtual void startProgress(
        const std::string& msg,
        uint               progressSize,
        uint               percPrintProgress = 10,
        uint               startPerc         = 0,
        uint               endPerc           = 100) = 0;

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
};

} // namespace vcl

#endif // VCL_MISC_LOGGER_ABSTRACT_LOGGER_H
