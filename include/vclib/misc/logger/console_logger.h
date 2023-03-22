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

#ifndef VCL_MISC_LOGGER_CONSOLE_LOGGER_H
#define VCL_MISC_LOGGER_CONSOLE_LOGGER_H

#include "logger.h"

#include <vclib/concept/logger.h>

namespace vcl {

class ConsoleLogger : public Logger<std::ostream>
{
public:
	ConsoleLogger() = default;
	ConsoleLogger(
		std::ostream& errStream,
		std::ostream& warnStream,
		std::ostream& progStream,
		std::ostream& debugStream);

protected:
	std::ostream* levelStream(LogLevel lvl);

private:
	std::ostream& errStream = std::cerr;
	std::ostream& warnStream = std::cout;
	std::ostream& progStream = std::cout;
	std::ostream& debugStream = std::cerr;
};

static_assert(LoggerConcept<ConsoleLogger>, "ConsoleLogger does not satisfy the LoggerConcept");

} // namespace vcl

#include "console_logger.cpp"

#endif // VCL_MISC_LOGGER_CONSOLE_LOGGER_H
