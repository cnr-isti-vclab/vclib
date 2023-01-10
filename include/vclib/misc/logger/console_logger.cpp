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

#include "console_logger.h"

namespace vcl {

inline ConsoleLogger::ConsoleLogger(
	std::ostream& errStream,
	std::ostream& warnStream,
	std::ostream& progStream,
	std::ostream& debugStream) :
		errStream(errStream),
		warnStream(warnStream),
		progStream(progStream),
		debugStream(debugStream)
{
}

inline std::ostream* ConsoleLogger::levelStream(LogLevel lvl)
{
	switch (lvl) {
	case ERROR:
		return &errStream;
	case WARNING:
		return &warnStream;
	case PROGRESS:
		return &progStream;
	case DEBUG:
		return &debugStream;
	}
	return nullptr;
}

} // namespace vcl
