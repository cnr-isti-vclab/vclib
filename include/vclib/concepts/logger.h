/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_LOGGER_H
#define VCL_CONCEPTS_LOGGER_H

#include <string>

#include <vclib/types.h>

namespace vcl {

class NullLogger;

/**
 * @brief The LoggerConcept is evaluated to true when the input type is a valid Logger type.
 *
 * A valid logger type may be the NullLogger type, or a class that implement all the member
 * functions of a Logger class.
 *
 * @ingroup lconcepts
 */
template <typename T>
concept LoggerConcept =
    std::is_same<T, NullLogger>::value || requires(
        T o,
        const T& co,
        std::string msg,
        typename T::LogLevel lvl)
{
    typename T::LogLevel;

    { o.enableIndentation() } -> std::same_as<void>;
    { o.disableIndentation() } -> std::same_as<void>;

    { o.reset() } -> std::same_as<void>;

    { o.setMaxLineWidth(uint()) } -> std::same_as<void>;
    { o.setPrintTimer(bool()) } -> std::same_as<void>;
    { o.startTimer() } -> std::same_as<void>;

    { o.startNewTask(double(), double(), msg) } -> std::same_as<void>;
    { o.endTask(msg) } -> std::same_as<void>;

    { co.percentage() } -> std::same_as<double>;
    { o.setPercentage(uint()) } -> std::same_as<void>;

    { o.log(msg) } -> std::same_as<void>;
    { o.log(lvl, msg) } -> std::same_as<void>;
    { o.log(uint(), msg) } -> std::same_as<void>;
    { o.log(uint(), lvl, msg) } -> std::same_as<void>;

    { o.startProgress(msg, uint(), uint(), uint(), uint())} -> std::same_as<void>;
    { o.endProgress() }  -> std::same_as<void>;
    { o.progress(uint()) }  -> std::same_as<void>;
};

} // nameaspace vcl

#endif // VCL_CONCEPTS_LOGGER_H
