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

#ifndef VCL_IO_MESH_FORMATS_H
#define VCL_IO_MESH_FORMATS_H

#include <vclib/exceptions/io.h>
#include <vclib/io/file_format.h>

namespace vcl {

constexpr FileFormat objFileFormat()
{
    return FileFormat("obj", "OBJ Wavefront .obj");
}

constexpr FileFormat offFileFormat()
{
    return FileFormat("off", "OFF Object File Format .off");
}

constexpr FileFormat plyFileFormat()
{
    return FileFormat("ply", "PLY Polygon File Format .ply");
}

constexpr FileFormat stlFileFormat()
{
    return FileFormat("stl", "STL Stereolithography .stl");
}

inline FileFormat fileFormat(const std::string& format)
{
    std::string ext = toLower(format);
    if (ext == "obj") {
        return objFileFormat();
    }
    else if (ext == "off") {
        return offFileFormat();
    }
    else if (ext == "ply") {
        return plyFileFormat();
    }
    else if (ext == "stl") {
        return stlFileFormat();
    }
    else {
        throw UnknownFileFormatException(ext);
    }
}

} // namespace vcl

#endif // VCL_IO_MESH_FORMATS_H
