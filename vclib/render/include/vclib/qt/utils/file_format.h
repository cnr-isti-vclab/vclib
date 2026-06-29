// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
