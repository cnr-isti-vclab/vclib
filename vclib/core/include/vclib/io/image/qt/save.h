// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
