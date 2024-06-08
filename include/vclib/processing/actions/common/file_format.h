/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_PROCESSING_ACTIONS_COMMON_FILE_FORMAT_H
#define VCL_PROCESSING_ACTIONS_COMMON_FILE_FORMAT_H

#include <algorithm>
#include <string>
#include <vector>

#include <vclib/concepts/ranges/range.h>
#include <vclib/misc/string.h>

namespace vcl::proc {

class FileFormat
{
    std::vector<std::string> mExtensions;
    std::string mDescription;

public:
    FileFormat(std::string extenison, std::string description = "") :
            mExtensions{extenison}, mDescription(description)
    {
        clearExtension(mExtensions[0]);
    }

    FileFormat(Range auto extensions, std::string description = "") :
            mExtensions(extensions), mDescription(description)
    {
        for (auto& ext : mExtensions) {
            clearExtension(ext);
        }

        // make sure extensions are sorted - this is important for the
        // comparison operator
        std::sort(mExtensions.begin(), mExtensions.end());
    }

    const std::string& description() const
    {
        return mDescription;
    }

    const std::vector<std::string>& extensions() const
    {
        return mExtensions;
    }

    bool matchExtension(const std::string& extension) const
    {
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
    std::strong_ordering operator <=> (const FileFormat& other) const
    {
        for (const auto& ext : mExtensions) {
            if (other.matchExtension(ext)) {
                return std::strong_ordering::equal;
            }
        }

        return mExtensions[0] <=> other.mExtensions[0];
    }

private:
    static void clearExtension(std::string& extension)
    {
        if (extension.front() == '.') {
            extension.erase(0, 1);
        }
        extension = vcl::toLower(extension);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_COMMON_FILE_FORMAT_H
