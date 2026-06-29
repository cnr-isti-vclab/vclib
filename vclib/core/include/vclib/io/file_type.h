// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
