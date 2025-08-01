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

#ifndef VCL_IO_IMAGE_BMP_SAVE_H
#define VCL_IO_IMAGE_BMP_SAVE_H

#include <vclib/io/file_info.h>

namespace vcl {

/**
 * @brief Save the image data to a BMP file.
 *
 * The data is assumed to be in ABGR format (pixels of 32 bits, 8 bits per
 * channel).
 *
 * @param[in] filename: the name of the file to save. If the extension is not
 * '.bmp', it is added.
 * @param[in] w: the width of the image.
 * @param[in] h: the height of the image.
 * @param[in] data: the pixel data in ABGR format.
 */
inline void saveImageToBmp(
    const std::string&   filename,
    int                  w,
    int                  h,
    const unsigned char* data)
{
    std::string fn  = filename;
    std::string ext = FileInfo::extension(filename);
    ext             = toLower(ext);
    if (ext != ".bmp") {
        fn += ".bmp";
    }

    std::ofstream file(fn, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + fn);
    }

    // BMP file header
    unsigned char fileHeader[14] = {
        'B',
        'M', // Signature
        0,
        0,
        0,
        0, // File size in bytes
        0,
        0, // Reserved
        0,
        0, // Reserved
        54,
        0,
        0,
        0 // Offset to pixel data
    };

    // BMP info header
    unsigned char infoHeader[40] = {
        40, 0, 0, 0, // Header size
        0,  0, 0, 0, // Image width
        0,  0, 0, 0, // Image height
        1,  0,       // Planes
        32, 0,       // Bits per pixel
        0,  0, 0, 0, // Compression (none)
        0,  0, 0, 0, // Image size (can be 0 for uncompressed)
        0,  0, 0, 0, // X pixels per meter
        0,  0, 0, 0, // Y pixels per meter
        0,  0, 0, 0, // Total colors (0 = default)
        0,  0, 0, 0  // Important colors (0 = all)
    };

    // Fill in file size
    int fileSize  = 54 + w * h * 4; // Header size + pixel data size
    fileHeader[2] = (unsigned char) (fileSize);
    fileHeader[3] = (unsigned char) (fileSize >> 8);
    fileHeader[4] = (unsigned char) (fileSize >> 16);
    fileHeader[5] = (unsigned char) (fileSize >> 24);

    // Fill in image width and height
    infoHeader[4]  = (unsigned char) (w);
    infoHeader[5]  = (unsigned char) (w >> 8);
    infoHeader[6]  = (unsigned char) (w >> 16);
    infoHeader[7]  = (unsigned char) (w >> 24);
    infoHeader[8]  = (unsigned char) (h);
    infoHeader[9]  = (unsigned char) (h >> 8);
    infoHeader[10] = (unsigned char) (h >> 16);
    infoHeader[11] = (unsigned char) (h >> 24);

    // Write headers
    file.write(reinterpret_cast<const char*>(fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(infoHeader), sizeof(infoHeader));

    // Write pixel data (BMP stores pixels bottom-up)
    for (int y = h - 1; y >= 0; --y) {
        file.write(reinterpret_cast<const char*>(data + y * w * 4), w * 4);
    }

    file.close();
}

} // namespace vcl

#endif // VCL_IO_IMAGE_BMP_SAVE_H
