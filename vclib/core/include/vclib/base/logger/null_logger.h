// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_LOGGER_NULL_LOGGER_H
#define VCL_BASE_LOGGER_NULL_LOGGER_H

#include "abstract_logger.h"

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

    void startNewTask(double, double, std::string_view) override final {}

    void endTask(std::string_view) override final {}

    double percentage() const override final { return 0; }

    void setPercentage(uint) override final {}

    void log(auto&&...) const noexcept {}

    void startProgress(
        std::string_view,
        uint,
        ProgressMode = ProgressMode::TIME,
        double       = 1.0,
        uint         = 0,
        uint         = 0) override final
    {
    }

    void endProgress() override final {}

    void progress(uint) override final {}

private:
    void print(std::string_view, LogLevel = PROGRESS_LOG) const override final
    {
    }

    void print(uint, std::string_view, LogLevel = PROGRESS_LOG) override final
    {
    }
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
