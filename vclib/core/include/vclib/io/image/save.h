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

#ifndef VCL_IO_IMAGE_SAVE_H
#define VCL_IO_IMAGE_SAVE_H

#include "bmp/save.h"

#ifdef VCLIB_WITH_QT
// #include "qt/save.h"
#endif

#ifdef VCLIB_WITH_STB
#include "stb/save.h"
#endif

#include <vclib/space/core/image.h>

namespace vcl {

/**
 * @brief Returns the set of image formats supported for saving.
 *
 * The set contains all the image formats that can be saved using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of image formats supported for saving.
 */
inline std::set<FileFormat> saveImageFormats()
{
    std::set<FileFormat> ff;

#ifdef VCLIB_WITH_QT
    // auto fqt = qt::saveImageFormats();
    // ff.insert(fqt.begin(), fqt.end());
#endif

#ifdef VCLIB_WITH_STB
    auto fstb = stb::saveImageFormats();
    ff.insert(fstb.begin(), fstb.end());
#endif
    return ff;
}

inline void saveImageData(
    const std::string&   filename,
    int                  w,
    int                  h,
    const unsigned char* data,
    uint                 quality = 90)
{
    FileFormat ff = FileInfo::fileFormat(filename);

#ifdef VCLIB_WITH_QT
    // if (qt::saveImageFormats().contains(ff)) {
    //     return qt::saveImageData(filename, w, h, data, quality);
    // }
#endif

#ifdef VCLIB_WITH_STB
    if (stb::saveImageFormats().contains(ff)) {
        return stb::saveImageData(filename, w, h, data, quality);
    }
#endif
    if (ff == FileFormat("bmp")) {
        // save rgb image data into bmp file
        return saveImageToBmp(filename, w, h, data);
    }
    throw std::runtime_error(
        "File Format " + ff.extensions().front() +
        " not supported for saving image data");
}

inline void saveImage(const Image& image, const std::string& filename)
{
    saveImageData(filename, image.width(), image.height(), image.data());
}

} // namespace vcl

#endif // VCL_IO_IMAGE_SAVE_H
