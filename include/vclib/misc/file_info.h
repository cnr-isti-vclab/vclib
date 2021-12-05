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

#ifndef VCL_FILE_INFO_H
#define VCL_FILE_INFO_H

#include <string>

namespace vcl::file_info {

void separateExtensionFromFilename(
	const std::string& fullname,
	std::string& rawname,
	std::string& extension);

void separateFilenameFromPath(
	const std::string& fullpath,
	std::string& path,
	std::string& filename);

std::string pathWithoutFilename(const std::string& fullpath);

std::string filenameWithoutExtension(const std::string& fullpath);

std::string filenameWithExtension(const std::string& fullpath);

}

#include "file_info.cpp"

#endif // VCL_FILE_INFO_H
