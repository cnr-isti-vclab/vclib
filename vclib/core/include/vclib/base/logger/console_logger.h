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

class ConsoleLogger : public Logger
{
    std::FILE* mErrFile   = stderr;
    std::FILE* mWarnFile  = stdout;
    std::FILE* mMsgFile   = stdout;
    std::FILE* mProgFile  = stdout;
    std::FILE* mDebugFile = stderr;

public:
    ConsoleLogger() = default;

    ConsoleLogger(
        std::FILE* errFile,
        std::FILE* warnFile,
        std::FILE* msgFile,
        std::FILE* progFile,
        std::FILE* debugFile) :
            mErrFile(errFile), mWarnFile(warnFile),
            mMsgFile(msgFile), mProgFile(progFile),
            mDebugFile(debugFile)
    {
    }

protected:
    void write(std::string_view line, LogLevel lvl) const override
    {
        std::FILE* out = targetFile(lvl);
        if (out) {
            std::println(out, "{}", line);
        }
    }

    void flush() const override
    {
        std::fflush(stdout);
        std::fflush(stderr);
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
