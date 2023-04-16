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

#include "file_info.h"

#include <fstream>
#include <filesystem>
#include <vector>

#include <vclib/exceptions/io_exceptions.h>

namespace vcl {

inline FileInfo::FileInfo()
{
}

inline FileInfo::FileInfo(const std::string& filename) : filename(filename)
{
}

/**
 * @brief Check if a file exists.
 *
 * @param[in] filename: string containing the filename
 * @return true if the file exists, false otherwise
 */
inline bool FileInfo::exists(const std::string& filename)
{
	return std::filesystem::exists(filename);
}

/**
 * @brief Get the size of a file.
 *
 * @param[in] filename: string containing the filename
 * @return the size of the file in bytes
 */
inline std::size_t FileInfo::fileSize(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	return file.tellg();
}

/**
 * @brief Check if a file is binary.
 *
 * The function checks the first 1000 bytes of the file. If a single byte is not an ASCII character
 * (i.e., its value is greater than 127), the file is considered binary.
 *
 * @param[in] filename: string containing the filename
 * @return true if the file is binary, false otherwise
 * @throws vcl::MalformedFileException if the file cannot be read
 */
inline bool FileInfo::isFileBinary(const std::string& filename)
{
	const std::size_t CHECK_BUFF_SIZE = 1000;

	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::size_t fsize = file.tellg();
	std::size_t size = std::min(fsize, CHECK_BUFF_SIZE);
	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> buffer(size);
	if (file.read((char*)buffer.data(), size)) {
		for (uint i = 0; i < buffer.size(); ++i) {
			if (buffer[i] > 127)
				return true;
		}
	}
	else {
		throw vcl::MalformedFileException("Cannot read data from file.");
	}
	return false;
}

/**
 * @brief Extracts the extension of a string that contains a filename.
 *
 * Example of usage:
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string filename, extension;
 * FileInfo::separateExtensionFromFilename(fullname, filename, extension);
 * //filename = "/usr/bin/foo"
 * //extension = ".sh"
 * @endcode
 *
 * To separate the path from the filename, see FileInfo::separateFilenameFromPath
 *
 * @param[in] fullname: string containing the filename
 * @param[out] rawname: output string containing the filename without the extension
 * @param[out] extension: output string containing the extension of the filename
 */
inline void FileInfo::separateExtensionFromFilename(
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
		rawname = fullname;
	}
}

/**
 * @brief Extracts the filename (extension included) of a string that contains a fullpath.
 *
 * Example of usage:
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string path, filename;
 * FileInfo::separateExtensionFromFilename(fullname, path, filename);
 * //path = "/usr/bin"
 * //filename = "foo.sh"
 * @endcode
 *
 * @param[in] fullpath: string containing the fullpath
 * @param[out] path: output string containing the path of the file
 * @param[out] filename: output string containing the filename (extension included)
 */
inline void FileInfo::separateFilenameFromPath(
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
		filename = fullpath;
	}
}

/**
 * @brief Get the path of a file.
 *
 * Example of usage:
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string path = FileInfo::pathWithoutFilename(fullname);
 * //path = "/usr/bin/"
 * @endcode
 *
 * @param[in] fullpath: string containing the full path of the file
 * @return the path of the file
 */
inline std::string FileInfo::pathWithoutFilename(const std::string& fullpath)
{
	std::string path, filename;
	separateFilenameFromPath(fullpath, path, filename);
	return path + "/";
}

/**
 * @brief Get the filename with extension of a file.
 *
 * Example of usage:
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string filename = FileInfo::filenameWithExtension(fullname);
 * //filename = "foo.sh"
 * @endcode
 *
 * @param[in] fullpath: string containing the full path of the file
 * @return the filename with extension of the file
 */
inline std::string FileInfo::filenameWithExtension(const std::string& fullpath)
{
	std::string filename, path, ext;
	separateFilenameFromPath(fullpath, path, filename);
	return filename;
}

/**
 * @brief Get the filename without extension of a file.
 *
 * Example of usage:
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string filename = FileInfo::filenameWithoutExtension(fullname);
 * //filename = "foo"
 * @endcode
 *
 * @param[in] fullpath string containing the full path of the file
 * @return the filename without extension of the file
 */
inline std::string FileInfo::filenameWithoutExtension(const std::string& fullpath)
{
	std::string filename = filenameWithExtension(fullpath), ext;
	separateExtensionFromFilename(filename, filename, ext);
	return filename;
}

/**
 * @brief Get the extension of a file.
 *
 * Example of usage:
 * @code{.cpp}
 * std::string fullname = "/usr/bin/foo.sh";
 * std::string ext = FileInfo::extension(fullname);
 * //ext = ".sh"
 * @endcode
 *
 * @param[in] filename: string containing the filename
 * @return the extension of the file
 */
inline std::string FileInfo::extension(const std::string& filename)
{
	std::string fn, ext;
	separateExtensionFromFilename(filename, fn, ext);
	return ext;
}

/**
 * @brief Adds an extension to a file name if it doesn't already have it.
 *
 * @param[in] filename: The file name to add the extension to.
 * @param[in] ext: The extension to add to the file name.
 * @return The file name with the extension added.
 */
inline std::string FileInfo::addExtensionToFileName(
	const std::string& filename,
	const std::string& ext)
{
	std::string actualfilename;
	size_t lastindex = filename.find_last_of(".");
	if (lastindex != std::string::npos) {
		std::string e = filename.substr(lastindex+1, filename.size());
		if (e == ext)
			actualfilename = filename;
		else
			actualfilename = filename + "." + ext;
	}
	else
		actualfilename = filename + "." + ext;
	return actualfilename;
}

} // namespace vcl
