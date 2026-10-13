// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_LOGGER_CONSOLE_LOGGER_H
#define VCL_BASE_LOGGER_CONSOLE_LOGGER_H

#include "logger.h"

#include <cstdio>
#include <print>

namespace vcl {

/**
 * @brief The ConsoleLogger class writes formatted log messages to standard
 * C streams (FILE*).
 *
 * By default, error and debug messages are emitted to stderr, while warning,
 * message, and progress lines are emitted to stdout. Custom target streams
 * can be provided through the constructor.
 *
 * @ingroup base
 */
class ConsoleLogger : public Logger
{
    std::FILE* mErrFile   = stderr;
    std::FILE* mWarnFile  = stdout;
    std::FILE* mMsgFile   = stdout;
    std::FILE* mProgFile  = stdout;
    std::FILE* mDebugFile = stderr;

public:
    /**
     * @brief Constructs a default ConsoleLogger.
     *
     * Errors and debug messages default to stderr; warnings, messages,
     * and progress logs default to stdout.
     */
    ConsoleLogger() = default;

    /**
     * @brief Constructs a ConsoleLogger with custom target streams.
     *
     * @param[in] errFile: Stream for ERROR_LOG messages (defaults to stderr).
     * @param[in] warnFile: Stream for WARNING_LOG messages (defaults to
     * stdout).
     * @param[in] msgFile: Stream for MESSAGE_LOG messages (defaults to stdout).
     * @param[in] progFile: Stream for PROGRESS_LOG messages (defaults to
     * stdout).
     * @param[in] debugFile: Stream for DEBUG_LOG messages (defaults to stderr).
     */
    ConsoleLogger(
        std::FILE* errFile,
        std::FILE* warnFile,
        std::FILE* msgFile,
        std::FILE* progFile,
        std::FILE* debugFile) :
            mErrFile(errFile), mWarnFile(warnFile), mMsgFile(msgFile),
            mProgFile(progFile), mDebugFile(debugFile)
    {
    }

protected:
    /**
     * @brief Writes a formatted log line to the destination stream based on log
     * level.
     *
     * @param[in] line: The formatted log line to emit.
     * @param[in] lvl: The LogLevel associated with the line.
     */
    void write(std::string_view line, LogLevel lvl) const override
    {
        std::FILE* out = targetFile(lvl);
        if (out) {
            std::println(out, "{}", line);
        }
    }

    /**
     * @brief Flushes all configured output streams.
     */
    void flush() const override
    {
        if (mErrFile)
            std::fflush(mErrFile);
        if (mWarnFile && mWarnFile != mErrFile)
            std::fflush(mWarnFile);
        if (mMsgFile && mMsgFile != mWarnFile && mMsgFile != mErrFile)
            std::fflush(mMsgFile);
        if (mProgFile && mProgFile != mMsgFile && mProgFile != mWarnFile &&
            mProgFile != mErrFile)
            std::fflush(mProgFile);
        if (mDebugFile && mDebugFile != mProgFile && mDebugFile != mMsgFile &&
            mDebugFile != mWarnFile && mDebugFile != mErrFile)
            std::fflush(mDebugFile);
    }

private:
    std::FILE* targetFile(LogLevel lvl) const
    {
        switch (lvl) {
        case ERROR_LOG: return mErrFile;
        case WARNING_LOG: return mWarnFile;
        case MESSAGE_LOG: return mMsgFile;
        case PROGRESS_LOG: return mProgFile;
        case DEBUG_LOG: return mDebugFile;
        }
        return nullptr;
    }
};

} // namespace vcl

#endif // VCL_BASE_LOGGER_CONSOLE_LOGGER_H
