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

#ifndef VCL_IO_IMAGE_QT_LOAD_H
#define VCL_IO_IMAGE_QT_LOAD_H

#include <vclib/io/file_info.h>

#include <QImage>

#include <memory>
#include <set>
#include <string>

namespace vcl::qt {

inline std::set<FileFormat> loadImageFormats()
{
    return {
        FileFormat("bmp", "Bitmap"),
        FileFormat(
            std::vector<std::string> {"jpg", "jpeg"},
            "Joint Photographic Experts Group"),
        FileFormat("png", "Portable Network Graphics"),
        FileFormat("pbm", "Portable Bitmap"),
        FileFormat("pgm", "Portable Graymap"),
        FileFormat("ppm", "Portable Pixmap"),
        FileFormat("xbm", "X11 Bitmap"),
        FileFormat("xpm", "X11 Pixmap"),
    };
}

inline std::shared_ptr<unsigned char> loadImageData(
    const std::string& filename,
    int&               w,
    int&               h)
{
    std::shared_ptr<unsigned char> ptr;
    QImage                         img(filename.c_str());
    if (!img.isNull()) {
        img.convertTo(QImage::Format_RGBA8888);

        ptr = std::shared_ptr<unsigned char>(
            new unsigned char[img.width() * img.height() * 4]);
        std::copy(
            img.bits(), img.bits() + img.width() * img.height() * 4, ptr.get());

        w = QImage(filename.c_str()).width();
        h = QImage(filename.c_str()).height();
    }
    return ptr;
}

} // namespace vcl::qt

#endif // VCL_IO_IMAGE_QT_LOAD_H
