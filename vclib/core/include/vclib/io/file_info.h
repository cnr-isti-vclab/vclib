/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_IO_FILE_INFO_H
#define VCL_IO_FILE_INFO_H

#include "file_format.h"

#include <vclib/exceptions.h>
#include <vclib/base.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace vcl {

class FileInfo
{
    std::string mFilename;

public:
    FileInfo() = default;

    FileInfo(const std::string& filename) : mFilename(filename) {}

    bool exists() const { return exists(mFilename); }

    std::size_t fileSize() const { return fileSize(mFilename); }

    bool isFileBinary() const { return isFileBinary(mFilename); }

    /* Static member functions */

    // file stat

    /**
     * @brief Check if a file exists.
     *
     * @param[in] filename: string containing the filename
     * @return true if the file exists, false otherwise
     */
    static bool exists(const std::string& filename)
    {
        return std::filesystem::exists(filename);
    }

    /**
     * @brief Get the size of a file.
     *
     * @param[in] filename: string containing the filename
     * @return the size of the file in bytes
     */
    static std::size_t fileSize(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        return file.tellg();
    }

    /**
     * @brief Check if a file is binary.
     *
     * The function checks the first 1000 bytes of the file. If a single byte is
     * not an ASCII character (i.e., its value is greater than 127), the file is
     * considered binary.
     *
     * @param[in] filename: string containing the filename
     * @return true if the file is binary, false otherwise
     * @throws vcl::MalformedFileException if the file cannot be read
     */
    static bool isFileBinary(const std::string& filename)
    {
        const std::size_t CHECK_BUFF_SIZE = 1000;

        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        std::size_t   fsize = file.tellg();
        std::size_t   size  = std::min(fsize, CHECK_BUFF_SIZE);
        file.seekg(0, std::ios::beg);

        std::vector<unsigned char> buffer(size);
        if (file.read((char*) buffer.data(), size)) {
            for (uint i = 0; i < buffer.size(); ++i) {
                if (buffer[i] > 127)
                    return true;
            }
        }
        else {
            throw MalformedFileException("Cannot read data from file.");
        }
        return false;
    }

    // name and extension management

    /**
     * @brief Extracts the extension of a string that contains a filename.
     *
     * Example of usage:
     * @code{.cpp}
     * std::string fullname = "/usr/bin/foo.sh";
     * std::string filename, extension;
     * FileInfo::separateExtensionFromFileName(fullname, filename, extension);
     * //filename = "/usr/bin/foo"
     * //extension = ".sh"
     * @endcode
     *
     * To separate the path from the filename, see
     * FileInfo::separateFilenameFromPath
     *
     * @param[in] fullname: string containing the filename
     * @param[out] rawName: output string containing the filename without the
     * extension
     * @param[out] extension: output string containing the extension of the
     * filename
     */
    static void separateExtensionFromFileName(
        const std::string& fullname,
        std::string&       rawName,
        std::string&       extension)
    {
        // https://stackoverflow.com/questions/6417817

        size_t lastIndex = fullname.find_last_of(".");
        if (lastIndex != std::string::npos) {
            rawName   = fullname.substr(0, lastIndex);
            extension = fullname.substr(lastIndex, fullname.size());
        }
        else {
            rawName = fullname;
        }
    }

    /**
     * @brief Extracts the filename (extension included) of a string that
     * contains a fullpath.
     *
     * Example of usage:
     * @code{.cpp}
     * std::string fullname = "/usr/bin/foo.sh";
     * std::string path, filename;
     * FileInfo::separateExtensionFromFileName(fullname, path, filename);
     * //path = "/usr/bin/"
     * //filename = "foo.sh"
     * @endcode
     *
     * @param[in] fullpath: string containing the fullpath
     * @param[out] path: output string containing the path of the file
     * @param[out] filename: output string containing the filename (extension
     * included)
     */
    static void separateFileNameFromPath(
        const std::string& fullpath,
        std::string&       path,
        std::string&       filename)
    {
        size_t lastIndex = fullpath.find_last_of("/");
        if (lastIndex != std::string::npos) {
            path     = fullpath.substr(0, lastIndex + 1);
            filename = fullpath.substr(lastIndex + 1, fullpath.size());
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
     * std::string path = FileInfo::pathWithoutFileName(fullname);
     * //path = "/usr/bin/"
     * @endcode
     *
     * @param[in] fullpath: string containing the full path of the file
     * @return the path of the file
     */
    static std::string pathWithoutFileName(const std::string& fullpath)
    {
        std::string path, filename;
        separateFileNameFromPath(fullpath, path, filename);
        return path;
    }

    /**
     * @brief Get the file name without extension of a file.
     *
     * Example of usage:
     * @code{.cpp}
     * std::string fullname = "/usr/bin/foo.sh";
     * std::string filename = FileInfo::fileNameWithoutExtension(fullname);
     * //filename = "foo"
     * @endcode
     *
     * @param[in] fullpath string containing the full path of the file
     * @return the filename without extension of the file
     */
    static std::string fileNameWithoutExtension(const std::string& fullpath)
    {
        std::string filename = fileNameWithExtension(fullpath), ext;
        separateExtensionFromFileName(filename, filename, ext);
        return filename;
    }

    /**
     * @brief Get the filename with extension of a file.
     *
     * Example of usage:
     * @code{.cpp}
     * std::string fullname = "/usr/bin/foo.sh";
     * std::string filename = FileInfo::fileNameWithExtension(fullname);
     * //filename = "foo.sh"
     * @endcode
     *
     * @param[in] fullpath: string containing the full path of the file
     * @return the filename with extension of the file
     */
    static std::string fileNameWithExtension(const std::string& fullpath)
    {
        std::string filename, path, ext;
        separateFileNameFromPath(fullpath, path, filename);
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
    static std::string extension(const std::string& filename)
    {
        std::string fn, ext;
        separateExtensionFromFileName(filename, fn, ext);
        return ext;
    }

    /**
     * @brief Get the file format of a file from its filename.
     *
     * Example of usage:
     * @code{.cpp}
     * std::string fullname = "/usr/bin/foo.sh";
     * FileFormat ff = FileInfo::fileFormat(fullname);
     * //ff = FileFormat("sh", "")
     * @endcode
     *
     * @param[in] filename: string containing the filename
     * @return the file format of the file
     */
    static FileFormat fileFormat(const std::string& filename)
    {
        return FileFormat(extension(filename));
    }

    /**
     * @brief Adds an extension to a file name if it doesn't already have it.
     *
     * @param[in] filename: The file name to add the extension to.
     * @param[in] ext: The extension to add to the file name.
     * @return The file name with the extension added.
     */
    static std::string addExtensionIfNeeded(
        const std::string& filename,
        const std::string& ext)
    {
        std::string actualFileName;
        size_t      lastIndex = filename.find_last_of(".");
        if (lastIndex != std::string::npos) {
            std::string e = filename.substr(lastIndex + 1, filename.size());
            if (e == ext)
                actualFileName = filename;
            else
                actualFileName = filename + "." + ext;
        }
        else
            actualFileName = filename + "." + ext;
        return actualFileName;
    }
};

} // namespace vcl

#endif // VCL_IO_FILE_INFO_H
