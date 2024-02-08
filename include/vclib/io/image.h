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

#ifndef VCL_IO_IMAGE_H
#define VCL_IO_IMAGE_H

#include <vclib/io/file_info.h>
#include <vclib/misc/string.h>

#if __has_include(<stb/stb_image.h>)
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for
// header-only
#define STB_IMAGE_STATIC         // make stb static
#define STB_IMAGE_IMPLEMENTATION // and then include the implementation
#include "../../../external/stb-master/stb/stb_image.h"
#define STB_IMAGE_WRITE_STATIC         // make stb static
#define STB_IMAGE_WRITE_IMPLEMENTATION // and then include the implementation
#include "../../../external/stb-master/stb/stb_image_write.h"
#endif

#include <memory>
#include <string>

namespace vcl {

inline std::shared_ptr<unsigned char> loadImageData(
    const std::string& filename,
    int&               w,
    int&               h)
{
    std::shared_ptr<unsigned char> ptr(
        stbi_load(filename.c_str(), &w, &h, nullptr, 4), stbi_image_free);
    return ptr;
}

inline void saveImageData(
    const std::string&   filename,
    int                  w,
    int                  h,
    const unsigned char* data,
    uint                 quality = 90)
{
    std::string ext = FileInfo::extension(filename);
    ext             = vcl::toLower(ext);
    int ret         = 0;
    if (ext == ".png") {
        ret = stbi_write_png(filename.c_str(), w, h, 4, data, 0);
    }
    else if (ext == ".bmp") {
        ret = stbi_write_bmp(filename.c_str(), w, h, 4, data);
    }
    else if (ext == ".tga") {
        ret = stbi_write_tga(filename.c_str(), w, h, 4, data);
    }
    else if (ext == ".jpg" || ext == ".jpeg") {
        ret = stbi_write_jpg(filename.c_str(), w, h, 4, data, quality);
    }
    else {
        throw vcl::UnknownFileFormatException(ext);
    }

    if (ret == 0) {
        throw vcl::CannotOpenFileException(filename);
    }
}

} // namespace vcl

#endif // VCL_IO_IMAGE_H
