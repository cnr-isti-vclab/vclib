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

#ifndef LOGGERS_H
#define LOGGERS_H

#include <vclib/base.h>

void loggersStaticAsserts()
{
    using namespace vcl;

    static_assert(
        LoggerConcept<AbstractLogger>,
        "AbstractLogger does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<const AbstractLogger>,
        "const AbstractLogger does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<AbstractLogger&>,
        "AbstractLogger& does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<const AbstractLogger&>,
        "const AbstractLogger& does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<AbstractLogger&&>,
        "AbstractLogger&& does not satisfy the LoggerConcept");

    static_assert(
        LoggerConcept<NullLogger>,
        "NullLogger does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<const NullLogger>,
        "const NullLogger does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<NullLogger&>,
        "NullLogger& does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<const NullLogger&>,
        "const NullLogger& does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<NullLogger&&>,
        "NullLogger&& does not satisfy the LoggerConcept");

    static_assert(
        LoggerConcept<ConsoleLogger>,
        "ConsoleLogger does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<const ConsoleLogger>,
        "const ConsoleLogger does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<ConsoleLogger&>,
        "ConsoleLogger& does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<const ConsoleLogger&>,
        "const ConsoleLogger& does not satisfy the LoggerConcept");
    static_assert(
        LoggerConcept<ConsoleLogger&&>,
        "ConsoleLogger&& does not satisfy the LoggerConcept");
}

#endif // LOGGERS_H
