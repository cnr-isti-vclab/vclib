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

#ifndef VCL_IO_IMAGE_LOAD_H
#define VCL_IO_IMAGE_LOAD_H

#ifdef VCLIB_WITH_QT
#include "qt/load.h"
#endif

#ifdef VCLIB_WITH_STB
#include "stb/load.h"
#endif

#include <vclib/io/file_format.h>
#include <vclib/io/file_info.h>
#include <vclib/space/core/image.h>

#include <memory>
#include <string>

namespace vcl {

/**
 * @brief Returns the set of image formats supported for loading.
 *
 * The set contains all the image formats that can be loaded using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of image formats supported for loading.
 */
inline std::set<FileFormat> loadImageFormats()
{
    std::set<FileFormat> ff;

#ifdef VCLIB_WITH_QT
    auto fqt = qt::loadImageFormats();
    ff.insert(fqt.begin(), fqt.end());
#endif

#ifdef VCLIB_WITH_STB
    auto fstb = stb::loadImageFormats();
    ff.insert(fstb.begin(), fstb.end());
#endif
    return ff;
}

inline std::shared_ptr<unsigned char> loadImageData(
    const std::string& filename,
    int&               w,
    int&               h)
{
    FileFormat ff = FileInfo::fileFormat(filename);

#ifdef VCLIB_WITH_QT
    if (qt::loadImageFormats().contains(ff)) {
        return qt::loadImageData(filename, w, h);
    }
#endif

#ifdef VCLIB_WITH_STB
    if (stb::loadImageFormats().contains(ff)) {
        return stb::loadImageData(filename, w, h);
    }
#endif
    throw UnknownFileFormatException(ff.extensions().front());
}

inline Image loadImage(const std::string& filename)
{
    int  w, h;
    auto data = loadImageData(filename, w, h);
    if (!data) {
        return Image();
    }
    return Image(data.get(), w, h);
}

} // namespace vcl

#endif // VCL_IO_IMAGE_LOAD_H
