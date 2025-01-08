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

#ifndef VCL_IO_FILE_TYPE_H
#define VCL_IO_FILE_TYPE_H

#include <bit>

namespace vcl {

/**
 * @brief Class that defines whether a file is binary or text, and (if binary)
 * the endianness of the file.
 */
struct FileType
{
    bool isBinary = true;

    std::endian endian = std::endian::little;

    /**
     * @brief Default constructor, the file type is set to little endian binary.
     */
    FileType() = default;

    /**
     * @brief Constructor that creates a FileType object with the specified
     * binary flag.
     *
     * If the file is binary, the endianness is set to little endian.
     *
     * @param binary: flag that specifies if the file is binary
     */
    FileType(bool binary) : isBinary(binary) {}

    /**
     * @brief Constructor that creates a FileType object with the specified
     * endianness.
     *
     * The binary flag is set to true.
     *
     * @param end: endianness of the file
     */
    FileType(std::endian end) : isBinary(true), endian(end) {}
};

} // namespace vcl

#endif // VCL_IO_FILE_TYPE_H
