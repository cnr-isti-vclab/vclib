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

#ifndef VCL_MISC_LOGGER_NULL_LOGGER_H
#define VCL_MISC_LOGGER_NULL_LOGGER_H

namespace vcl {

/**
 * @brief The NullLogger class is an empty class that is used as default type in all the library
 * functions that take as input a logger type.
 *
 * This class satisfies the LoggerConcept.
 *
 * Before using a logger object, you should check if the type of the logger is valid in the
 * following way:
 *
 * @code{.cpp}
 * if constexpr (vcl::isLoggerValid<LoggerType>()) {
 *     // use here the log object
 * }
 * @endcode
 *
 * This allows to compile the log code only when the user gives a proper Logger object that
 * satisfies the LoggerConcept.
 *
 * @ingroup miscellaneous
 */
class NullLogger {
};

/**
 * @brief The static nullLogger object is an object of type NullLogger that is used as default
 * argument in the functions that can take as input a logger.
 *
 * A typical function that could take a logger is defined as follows:
 *
 * @code{.cpp}
 * template<typename T, LoggerConcept LoggerType = NullLogger>
 * void foo(T obj, LoggerType log = nullLogger)
 * {
 *    // code of the function...
 *
 *    if constexpr (vcl::isLoggerValid<LoggerType>()) {
 *       log.log("log!");
 *    }
 *
 * }
 * @endcode
 *
 * In this way, when the user does not give a logger argument, the default will be an object of
 * NullLogger type that allows to check at compile time if the logger object is valid.
 *
 * @ingroup miscellaneous
 */
static inline NullLogger nullLogger;

} // namespace vcl

#endif // VCL_MISC_LOGGER_NULL_LOGGER_H
