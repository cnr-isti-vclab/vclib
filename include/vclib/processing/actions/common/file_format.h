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

#include <string>
#include <vector>

#include <vclib/concepts/ranges/range.h>

namespace vcl {

class FileFormat
{
    std::vector<std::string> mExtensions;
    std::string mDescription;

public:
    FileFormat(std::string extenison, std::string description) :
            mExtensions{extenison}, mDescription(description)
    {
    }

    FileFormat(Range auto extensions, std::string description) :
            mExtensions(extensions), mDescription(description)
    {
    }

    const std::string& description() const
    {
        return mDescription;
    }

    const std::vector<std::string>& extensions() const
    {
        return mExtensions;
    }
};

} // namespace vcl

#endif // VCL_PROCESSING_ACTIONS_COMMON_FILE_FORMAT_H
