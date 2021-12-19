/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "io_utils.h"

#include <vclib/exception/io_exception.h>

namespace vcl::io::internal {

inline std::ofstream saveFileStream(const std::string& filename, const std::string& ext)
{
	std::string   actualfilename;
	std::ofstream fp;
	fp.imbue(std::locale().classic());
	size_t lastindex = filename.find_last_of(".");
	if (lastindex != filename.size())
		actualfilename = filename;
	else
		actualfilename = filename + "." + ext;

	fp.open(actualfilename, std::ofstream::binary); // need to set binary or windows will fail
	if (!fp) {
		throw vcl::CannotOpenFileException(actualfilename);
	}

	return fp;
}

} // namespace vcl::io::internal
