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

#ifndef VCL_LOAD_SAVE_CAPABILITY_H
#define VCL_LOAD_SAVE_CAPABILITY_H

#include "obj/capability.h"
#include "off/capability.h"
#include "ply/capability.h"
#include "stl/capability.h"

#include <vclib/exceptions/io.h>
#include <vclib/misc/string.h>

namespace vcl {

inline MeshInfo formatCapability(const std::string& format)
{
    std::string ext = vcl::toLower(format);
    if (ext == "obj") {
        return objFormatCapability();
    }
    else if (ext == "off") {
        return offFormatCapability();
    }
    else if (ext == "ply") {
        return plyFormatCapability();
    }
    else if (ext == "stl") {
        return stlFormatCapability();
    }
    else {
        throw vcl::UnknownFileFormatException(ext);
    }
}

} // namespace vcl

#endif // VCL_LOAD_SAVE_CAPABILITY_H
