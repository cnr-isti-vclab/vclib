/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#if __has_include(<stb/stb_image.h>)
#include <stb/stb_image.h>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for
// header-only
#define STB_IMAGE_STATIC         // make stb static
#define STB_IMAGE_IMPLEMENTATION // and then include the implementation
#include "../../../external/stb-master/stb/stb_image.h"
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

} // namespace vcl

#endif // VCL_IO_IMAGE_H
