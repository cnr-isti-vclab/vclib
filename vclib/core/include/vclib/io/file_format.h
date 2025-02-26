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

#ifndef VCL_IO_FILE_FORMAT_H
#define VCL_IO_FILE_FORMAT_H

#include <vclib/concepts/ranges/range.h>
#include <vclib/misc/string.h>

#include <algorithm>
#include <string>
#include <vector>

namespace vcl {

/**
 * @brief The FileFormat class represents a file format.
 *
 * A format is defined by a list of extensions and a description.
 */
class FileFormat
{
    std::vector<std::string> mExtensions;
    std::string              mDescription;

public:
    constexpr FileFormat(const char* extension, std::string description = "") :
            mExtensions {extension}, mDescription(description)
    {
        clearExtension(mExtensions[0]);
    }

    constexpr FileFormat(
        const std::string& extension,
        std::string        description = "") :
            FileFormat(extension.c_str(), description)
    {
    }

    constexpr FileFormat(Range auto extensions, std::string description = "") :
            mExtensions(extensions), mDescription(description)
    {
        for (auto& ext : mExtensions) {
            clearExtension(ext);
        }

        // make sure extensions are sorted - this is important for the
        // comparison operator
        std::sort(mExtensions.begin(), mExtensions.end());
    }

    const std::string& description() const { return mDescription; }

    const std::vector<std::string>& extensions() const { return mExtensions; }

    bool matchExtension(std::string extension) const
    {
        clearExtension(extension);
        for (const auto& ext : mExtensions) {
            if (ext == extension) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief A FileFormat is equal to another if **at least** one extension is
     * equal. The description is not considered.
     *
     * Otherwise, the comparison is based on the first extension.
     *
     * @note This is a lexicographical comparison.
     *
     * @param other
     * @return a std::strong_ordering value indicating the comparison result
     */
    auto operator<=>(const FileFormat& other) const
    {
        for (const auto& ext : mExtensions) {
            if (other.matchExtension(ext)) {
                return std::strong_ordering::equal;
            }
        }

        return mExtensions[0] <=> other.mExtensions[0];
    }

    bool operator==(const FileFormat& other) const
    {
        return *this <=> other == std::strong_ordering::equal;
    }

private:
    static constexpr void clearExtension(std::string& extension)
    {
        if (!extension.empty() && extension.front() == '.') {
            extension.erase(0, 1);
        }
        extension = toLower(extension);
    }
};

} // namespace vcl

#endif // VCL_IO_FILE_FORMAT_H
