// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_LOGGER_NULL_LOGGER_H
#define VCL_BASE_LOGGER_NULL_LOGGER_H

#include "abstract_logger.h"

#include <string>

namespace vcl {

/**
 * @brief The NullLogger class is used as default type in all the library
 * functions that take as input a logger type.
 *
 * This class satisfies the LoggerConcept, and just does nothing in all its
 * member functions.
 *
 * @ingroup base
 */
class NullLogger : public AbstractLogger
{
public:
    NullLogger() = default;

    void reset() override final {}

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

    void startProgress(
        const std::string&,
        uint,
        ProgressMode = ProgressMode::TIME,
        double = 1.0,
        uint = 0,
        uint = 0) override final
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
 * @ingroup base
 */
inline NullLogger nullLogger;

} // namespace vcl

#endif // VCL_BASE_LOGGER_NULL_LOGGER_H
