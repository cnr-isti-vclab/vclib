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

#ifndef VCL_MISC_FILE_INFO_H
#define VCL_MISC_FILE_INFO_H

#include <string>

namespace vcl {

class FileInfo
{
public:
	FileInfo();

	FileInfo(const std::string& filename);

	/* Static members */

	// file stat

	bool exists(const std::string& filename);

	static std::size_t fileSize(const std::string& filename);

	static bool isFileBinary(const std::string& filename);

	// name and extension management

	static void separateExtensionFromFilename(
		const std::string& fullname,
		std::string& rawname,
		std::string& extension);

	static void separateFilenameFromPath(
		const std::string& fullpath,
		std::string& path,
		std::string& filename);

	static std::string pathWithoutFilename(const std::string& fullpath);

	static std::string filenameWithoutExtension(const std::string& fullpath);

	static std::string filenameWithExtension(const std::string& fullpath);

	static std::string extension(const std::string& filename);

	static std::string addExtensionToFileName(const std::string& filename, const std::string& ext);

private:
	std::string filename;
};

} // namespace vcl

#include "file_info.cpp"

#endif // VCL_MISC_FILE_INFO_H
