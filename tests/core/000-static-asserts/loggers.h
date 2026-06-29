// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
