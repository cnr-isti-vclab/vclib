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

#ifndef VCL_MISC_LOGGER_LOGGER_CONCEPT_H
#define VCL_MISC_LOGGER_LOGGER_CONCEPT_H

#include "../types.h"

namespace vcl {

class NullLogger {
};

static NullLogger nullLogger;

template <typename T>
concept LoggerConcept =
	std::is_same<T, NullLogger>::value || requires(T o, std::string msg)
{
	typename T::LogLevel;
	{ o.reset() } -> std::same_as<void>;
	{ o.startNewTask(double(), double(), msg) } -> std::same_as<void>;
	{ o.endTask(msg) } -> std::same_as<void>;
	{ o.percentage() } -> std::same_as<double>;
	{ o.log(msg) } -> std::same_as<void>;

};

template <typename T>
constexpr bool isLoggerValid()
{
	return !std::is_same_v<T, NullLogger> && LoggerConcept<T>;
}

} // nameaspace vcl

#endif // VCL_MISC_LOGGER_LOGGER_CONCEPT_H
