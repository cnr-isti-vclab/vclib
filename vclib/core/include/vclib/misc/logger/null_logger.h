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

#ifndef VCL_MISC_LOGGER_NULL_LOGGER_H
#define VCL_MISC_LOGGER_NULL_LOGGER_H

#include "abstract_logger.h"

#include <vclib/concepts/logger.h>

#include <string>

namespace vcl {

/**
 * @brief The NullLogger class is used as default type in all the library
 * functions that take as input a logger type.
 *
 * This class satisfies the LoggerConcept, and just does nothing in all its
 * member functions.
 *
 * @ingroup miscellaneous
 */
class NullLogger : public AbstractLogger
{
public:
    NullLogger() = default;

    void enableIndentation() override final {}

    void disableIndentation() override final {}

    void enablePrintPercentage() override final {}

    void disablePrintPercentage() override final {}

    void setPrintLevel(LogLevel) override final {}

    void enablePrintMessageDuringProgress() override final {}

    void disablePrintMessageDuringProgress() override final {}

    void enablePrintTimer() override final {}

    void disablePrintTimer() override final {}

    void reset() override final {}

    void setMaxLineWidth(uint) override final {}

    void startTimer() override final {}

    void stopTimer() override final {}

    double time() const override final { return 0; }

    void startNewTask(double, double, const std::string&) override final {}

    void endTask(const std::string&) override final {}

    double percentage() const override final { return 0; }

    void setPercentage(uint) override final {}

    void log(const std::string&) const override final {}

    void log(const std::string&, LogLevel) const override final {}

    void log(uint, const std::string&) override final {}

    void log(uint, const std::string&, LogLevel) override final {}

    void startProgress(const std::string&, uint, uint = 0, uint = 0, uint = 0)
        override final
    {
    }

    void endProgress() override final {}

    void progress(uint) override final {}
};

/**
 * @brief The nullLogger object is an object of type NullLogger that is
 * used as default argument in the functions that can take as input a logger.
 *
 * A typical function that could take a logger is defined as follows:
 *
 * @code{.cpp}
 * template<typename T, LoggerConcept LoggerType = NullLogger>
 * void foo(T obj, LoggerType log = nullLogger)
 * {
 *    // code of the function...
 *
 *    // empty function will be called if the user does not give a logger
 *    log.log("log!");
 *
 * }
 * @endcode
 *
 * In this way, when the user does not give a logger argument, the default will
 * be an object of NullLogger.
 *
 * @ingroup miscellaneous
 */
inline NullLogger nullLogger;

} // namespace vcl

#endif // VCL_MISC_LOGGER_NULL_LOGGER_H
