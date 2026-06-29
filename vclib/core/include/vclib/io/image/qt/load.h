// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
