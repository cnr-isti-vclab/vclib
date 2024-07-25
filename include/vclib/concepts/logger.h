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
 * @brief The LoggerConcept is evaluated to true when the input type is a valid
 * Logger type.
 *
 * @note The @ref NullLogger class does satisfy this concept, by providing
 * all the required member functions as no-op.
 *
 * @ingroup lib_concepts
 * @ingroup miscellaneous
 */
template<typename T>
concept LoggerConcept =
    requires (T& o, const T& co, std::string msg, typename T::LogLevel lvl) {
        // clang-format off
        typename T::LogLevel;

        { o.enableIndentation() } -> std::same_as<void>;
        { o.disableIndentation() } -> std::same_as<void>;
        { o.enablePrintPercentage() } -> std::same_as<void>;
        { o.disablePrintPercentage() } -> std::same_as<void>;
        { o.setPrintLevel(lvl) } -> std::same_as<void>;
        { o.enablePrintMessageDuringProgress() } -> std::same_as<void>;
        { o.disablePrintMessageDuringProgress() } -> std::same_as<void>;
        { o.enablePrintTimer() } -> std::same_as<void>;
        { o.disablePrintTimer() } -> std::same_as<void>;

        { o.reset() } -> std::same_as<void>;

        { o.setMaxLineWidth(uint()) } -> std::same_as<void>;
        { o.startTimer() } -> std::same_as<void>;
        { o.stopTimer() } -> std::same_as<void>;
        { o.getTime() } -> std::same_as<double>;

        { o.startNewTask(double(), double(), msg) } -> std::same_as<void>;
        { o.endTask(msg) } -> std::same_as<void>;

        { co.percentage() } -> std::same_as<double>;
        { o.setPercentage(uint()) } -> std::same_as<void>;

        { o.log(msg) } -> std::same_as<void>;
        { o.log(lvl, msg) } -> std::same_as<void>;
        { o.log(uint(), msg) } -> std::same_as<void>;
        { o.log(uint(), lvl, msg) } -> std::same_as<void>;


        { o.startProgress(msg, uint()) } -> std::same_as<void>;
        { o.startProgress(msg, uint(), uint()) } -> std::same_as<void>;
        { o.startProgress(msg, uint(), uint(), uint()) } -> std::same_as<void>;
        { o.startProgress(msg, uint(), uint(), uint(), uint()) } ->
            std::same_as<void>;
        { o.endProgress() }  -> std::same_as<void>;
        { o.progress(uint()) }  -> std::same_as<void>;
        // clang-format off
};

} // nameaspace vcl

#endif // VCL_CONCEPTS_LOGGER_H
