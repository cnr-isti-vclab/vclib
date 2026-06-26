// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_IMAGE_STB_SAVE_H
#define VCL_IO_IMAGE_STB_SAVE_H

#include <vclib/io/exceptions.h>
#include <vclib/io/file_info.h>

// disable deprecated warnings - just for stb
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <stb_image_write.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <set>
#include <string>

namespace vcl::stb {

inline std::set<FileFormat> saveImageFormats()
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

inline void saveImageData(
    const std::string&   filename,
    int                  w,
    int                  h,
    const unsigned char* data,
    uint                 quality = 90)
{
    std::string ext = FileInfo::extension(filename);
    ext             = toLower(ext);
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
        throw UnknownFileFormatException(ext);
    }

    if (ret == 0) {
        throw CannotOpenFileException(filename);
    }
}

} // namespace vcl::stb

#endif // VCL_IO_IMAGE_STB_SAVE_H
