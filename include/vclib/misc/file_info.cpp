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

#include "file_info.h"

namespace vcl::fileInfo {

/**
 * @brief Extracts the extension of a string that contains a filename.
 *
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string filename, extension;
 * vcl::separateExtensionFromFilename(fullname, filename, extension);
 * //filename = "/usr/bin/foo"
 * //extension = ".sh"
 * @endcode
 *
 * To separate the path from the filename, see vcl::separateFilenameFromPath
 *
 * @param fullname
 * @param rawname
 * @param extension
 */
inline void separateExtensionFromFilename(
	const std::string& fullname,
	std::string& rawname,
	std::string& extension)
{
	// https://stackoverflow.com/questions/6417817/easy-way-to-remove-extension-from-a-filename

	size_t lastindex = fullname.find_last_of(".");
	if (lastindex != std::string::npos){
		rawname = fullname.substr(0, lastindex);
		extension = fullname.substr(lastindex, fullname.size());
	}
	else {
		rawname = "";
		extension = "";
	}
}

/**
 * @brief Extracts the filename (extension included) of a string that contains a fullpath.
 *
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string path, filename;
 * vcl::separateExtensionFromFilename(fullname, path, filename);
 * //path = "/usr/bin"
 * //filename = "foo.sh"
 * @endcode
 *
 * @param fullpath
 * @param path
 * @param filename
 */
inline void separateFilenameFromPath(
	const std::string &fullpath,
	std::string &path,
	std::string &filename)
{
	size_t lastindex = fullpath.find_last_of("/");
	if (lastindex != std::string::npos){
		path = fullpath.substr(0, lastindex);
		filename = fullpath.substr(lastindex+1, fullpath.size());
	}
	else {
		path = "";
		filename = "";
	}
}

/**
 * @brief pathWithoutFilename
 *
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string path = vcl::pathWithoutFilename(fullname);
 * //path = "/usr/bin/"
 * @endcode
 *
 * @param fullpath
 * @return
 */
inline std::string pathWithoutFilename(const std::string& fullpath)
{
	std::string path, filename;
	separateFilenameFromPath(fullpath, path, filename);
	return path + "/";
}

/**
 * @brief filenameWithExtension
 *
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string filename = vcl::filenameWithExtension(fullname);
 * //filename = "foo.sh"
 * @endcode
 *
 * @param fullpath
 * @return
 */
inline std::string filenameWithExtension(const std::string& fullpath)
{
	std::string filename, path, ext;
	separateFilenameFromPath(fullpath, path, filename);
	return filename;
}

/**
 * @brief filenameWithoutExtension
 *
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string filename = vcl::filenameWithoutExtension(fullname);
 * //filename = "foo"
 * @endcode
 *
 * @param fullpath
 * @return
 */
inline std::string filenameWithoutExtension(const std::string& fullpath)
{
	std::string filename = filenameWithExtension(fullpath), ext;
	separateExtensionFromFilename(filename, filename, ext);
	return filename;
}

}
