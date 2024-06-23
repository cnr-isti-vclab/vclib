/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_IO_FILE_FORMAT_H
#define VCL_IO_FILE_FORMAT_H

#include <bit>

namespace vcl {

/**
 * @brief Class that defines the format of a file.
 *
 * This class is used to define the format of a file, specifying if it is binary
 * or text, and (if binary) the endianness.
 */
struct FileFormat
{
    bool isBinary = true;

    std::endian endian   = std::endian::little;

    /**
     * @brief Default constructor, creates a little endian binary file format.
     */
    FileFormat() = default;

    /**
     * @brief Constructor that creates a file format with the specified binary
     * flag.
     *
     * If the file is binary, the endianness is set to little endian.
     *
     * @param binary: flag that specifies if the file is binary
     */
    FileFormat(bool binary) : isBinary(binary) {}

    /**
     * @brief Constructor that creates a binary file format with the specified
     * endianness.
     *
     * @param end: endianness of the file
     */
    FileFormat(std::endian end) : isBinary(true), endian(end) {}
};

} // namespace vcl

#endif // VCL_IO_FILE_FORMAT_H
