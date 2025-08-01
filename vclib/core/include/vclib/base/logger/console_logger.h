/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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
