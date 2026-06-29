// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_LOGGER_CONSOLE_LOGGER_H
#define VCL_BASE_LOGGER_CONSOLE_LOGGER_H

#include "logger.h"

#include <iomanip>

namespace vcl {

class ConsoleLogger : public Logger<std::ostream>
{
    std::ostream& mErrStream   = std::cerr;
    std::ostream& mWarnStream  = std::cout;
    std::ostream& mMsgStream   = std::cout;
    std::ostream& mProgStream  = std::cout;
    std::ostream& mDebugStream = std::cerr;

public:
    ConsoleLogger() = default;

    ConsoleLogger(
        std::ostream& errStream,
        std::ostream& warnStream,
        std::ostream& msgStream,
        std::ostream& progStream,
        std::ostream& debugStream) :
            mErrStream(errStream), mWarnStream(warnStream),
            mMsgStream(msgStream), mProgStream(progStream),
            mDebugStream(debugStream)
    {
    }

protected:
    std::ostream* levelStream(LogLevel lvl) const override
    {
        switch (lvl) {
        case ERROR_LOG: return &mErrStream;
        case WARNING_LOG: return &mWarnStream;
        case MESSAGE_LOG: return &mMsgStream;
        case PROGRESS_LOG: return &mProgStream;
        case DEBUG_LOG: return &mDebugStream;
        }
        return nullptr;
    }

    void alignLeft(std::ostream& o) const override { o << std::left; }

    void alignRight(std::ostream& o) const override { o << std::right; }

    void setWidth(std::ostream& o, uint w) const override { o << std::setw(w); }

    void flush(std::ostream& o) const override { o.flush(); }
};

} // namespace vcl

#endif // VCL_BASE_LOGGER_CONSOLE_LOGGER_H
