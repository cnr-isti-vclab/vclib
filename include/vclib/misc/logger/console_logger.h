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

#ifndef VCL_MISC_LOGGER_CONSOLE_LOGGER_H
#define VCL_MISC_LOGGER_CONSOLE_LOGGER_H

#include "logger.h"

#include <vclib/concepts/logger.h>

namespace vcl {

class ConsoleLogger : public Logger<std::ostream>
{
    std::ostream& errStream   = std::cerr;
    std::ostream& warnStream  = std::cout;
    std::ostream& progStream  = std::cout;
    std::ostream& debugStream = std::cerr;

public:
    ConsoleLogger() = default;

    ConsoleLogger(
        std::ostream& errStream,
        std::ostream& warnStream,
        std::ostream& progStream,
        std::ostream& debugStream) :
            errStream(errStream),
            warnStream(warnStream), progStream(progStream),
            debugStream(debugStream)
    {
    }

protected:
    std::ostream* levelStream(LogLevel lvl)
    {
        switch (lvl) {
        case ERROR: return &errStream;
        case WARNING: return &warnStream;
        case PROGRESS: return &progStream;
        case DEBUG: return &debugStream;
        }
        return nullptr;
    }
};

} // namespace vcl

#endif // VCL_MISC_LOGGER_CONSOLE_LOGGER_H
