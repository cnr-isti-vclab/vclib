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

#ifndef VCL_CONCEPTS_LOGGER_H
#define VCL_CONCEPTS_LOGGER_H

#include <string>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief The LoggerConcept is satisfied if the type T is a valid Logger type.
 *
 * A valid Logger type must provide a set of member functions to control the
 * logging behavior, that are checked by this concept.
 *
 * @note The NullLogger class does satisfy this concept, by providing all the
 * required member functions as no-op.
 *
 * @ingroup lib_concepts
 * @ingroup miscellaneous
 */
template<typename T>
concept LoggerConcept = requires (
    T&                   obj,
    const T&             cObj,
    std::string          str,
    uint                 n,
    typename T::LogLevel lvl) {
    typename T::LogLevel;

    { obj.enableIndentation() } -> std::same_as<void>;
    { obj.disableIndentation() } -> std::same_as<void>;
    { obj.enablePrintPercentage() } -> std::same_as<void>;
    { obj.disablePrintPercentage() } -> std::same_as<void>;
    { obj.setPrintLevel(lvl) } -> std::same_as<void>;
    { obj.enablePrintMessageDuringProgress() } -> std::same_as<void>;
    { obj.disablePrintMessageDuringProgress() } -> std::same_as<void>;
    { obj.enablePrintTimer() } -> std::same_as<void>;
    { obj.disablePrintTimer() } -> std::same_as<void>;

    { obj.reset() } -> std::same_as<void>;

    { obj.setMaxLineWidth(n) } -> std::same_as<void>;
    { obj.startTimer() } -> std::same_as<void>;
    { obj.stopTimer() } -> std::same_as<void>;
    { obj.getTime() } -> std::same_as<double>;

    { obj.startNewTask(double(), double(), str) } -> std::same_as<void>;
    { obj.endTask(str) } -> std::same_as<void>;

    { cObj.percentage() } -> std::same_as<double>;
    { obj.setPercentage(n) } -> std::same_as<void>;

    { obj.log(str) } -> std::same_as<void>;
    { obj.log(lvl, str) } -> std::same_as<void>;
    { obj.log(n, str) } -> std::same_as<void>;
    { obj.log(n, lvl, str) } -> std::same_as<void>;

    { obj.startProgress(str, n) } -> std::same_as<void>;
    { obj.startProgress(str, n, n) } -> std::same_as<void>;
    { obj.startProgress(str, n, n, n) } -> std::same_as<void>;
    { obj.startProgress(str, n, n, n, n) } -> std::same_as<void>;
    { obj.endProgress() } -> std::same_as<void>;
    { obj.progress(n) } -> std::same_as<void>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_LOGGER_H
