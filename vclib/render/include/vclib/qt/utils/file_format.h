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

#ifndef VCL_QT_UTILS_FILE_FORMAT_H
#define VCL_QT_UTILS_FILE_FORMAT_H

#include <vclib/io/file_format.h>

#include <QString>

namespace vcl::qt {

inline QString filterFormatToQString(const FileFormat& format)
{
    QString filter = QString::fromStdString(format.description());
    filter += " (";
    for (const auto& ext : format.extensions()) {
        filter += "*." + QString::fromStdString(ext) + " ";
    }
    filter.chop(1);
    filter += ")";
    return filter;
}

inline QString filterFormatsToQString(
    const std::vector<FileFormat>& formats,
    bool                           allFormats = false)
{
    QString filter;
    if (allFormats) {
        filter += "All supported formats (";
        for (const auto& format : formats) {
            for (const auto& ext : format.extensions()) {
                filter += "*." + QString::fromStdString(ext) + " ";
            }
        }
        filter.chop(1);
        filter += ");;";
    }
    for (const auto& format : formats) {
        filter += filterFormatToQString(format);
        filter += ";;";
    }
    // remove last two chars
    filter.chop(2);
    return filter;
}

inline FileFormat formatFromQStringFilter(const QString& filter)
{
    // find the first "(*."
    int start = filter.indexOf("(*.") + 3;
    // find the first ")" after start
    int end = filter.indexOf(")", start);
    // extract the extension between start and end
    QString ext = filter.mid(start, end - start);
    return FileFormat(ext.toStdString());
}

} // namespace vcl::qt

#endif // VCL_QT_UTILS_FILE_FORMAT_H
