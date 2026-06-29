// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_IMAGE_STB_LOAD_H
#define VCL_IO_IMAGE_STB_LOAD_H

#include <vclib/io/file_info.h>

// disable deprecated warnings - just for stb
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stb_image.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <memory>
#include <set>
#include <string>

namespace vcl::stb {

inline std::set<FileFormat> loadImageFormats()
{
    return {
        FileFormat("png", "Portable Network Graphics"),
        FileFormat("bmp", "Bitmap"),
        FileFormat("tga", "Truevision TGA"),
        FileFormat(
            std::vector<std::string> {"jpg", "jpeg"},
            "Joint Photographic Experts Group"),
    };
}

inline std::shared_ptr<unsigned char> loadImageData(
    const std::string& filename,
    int&               w,
    int&               h)
{
    std::shared_ptr<unsigned char> ptr(
        stbi_load(filename.c_str(), &w, &h, nullptr, 4), stbi_image_free);
    return ptr;
}

} // namespace vcl::stb

#endif // VCL_IO_IMAGE_STB_LOAD_H
