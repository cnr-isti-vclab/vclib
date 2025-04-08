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

#ifndef VCL_IO_IMAGE_QT_SAVE_H
#define VCL_IO_IMAGE_QT_SAVE_H

#include <vclib/io/file_format.h>

#include <QImage>

#include <set>

namespace vcl::qt {

inline std::set<FileFormat> saveImageFormats()
{
    return {
            FileFormat("bmp", "Bitmap"),
            FileFormat(
                std::vector<std::string> {"jpg", "jpeg"},
                "Joint Photographic Experts Group"),
            FileFormat("png", "Portable Network Graphics"),
            FileFormat("ppm", "Portable Pixmap"),
            FileFormat("xbm", "X11 Bitmap"),
            FileFormat("xpm", "X11 Pixmap"),
            };
}

inline void saveImageData(
    const std::string&   filename,
    int                  w,
    int                  h,
    const unsigned char* data,
    uint                 quality = 90)
{
    QImage image(w, h, QImage::Format_RGBA8888);
    std::copy(data, data + w * h * 4, image.bits());
    bool res = image.save(QString::fromStdString(filename), nullptr, quality);
    if (!res) {
        throw std::runtime_error(
            "Failed to save image data to file: " + filename);
    }
}

} // namespace vcl::qt

#endif // VCL_IO_IMAGE_QT_SAVE_H
