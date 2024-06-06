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

#ifndef VCL_IO_SAVE_H
#define VCL_IO_SAVE_H

#include "obj/save.h"
#include "off/save.h"
#include "ply/save.h"
#include "stl/save.h"

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void save(
    const MeshType&     m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
{
    std::string ext = FileInfo::extension(filename);
    ext             = vcl::toLower(ext);
    if (ext == ".obj") {
        saveObj(m, filename, log, settings);
    }
    else if (ext == ".off") {
        saveOff(m, filename, log, settings);
    }
    else if (ext == ".ply") {
        savePly(m, filename, log, settings);
    }
    else if (ext == ".stl") {
        saveStl(m, filename, log, settings);
    }
    else {
        throw vcl::UnknownFileFormatException(ext);
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void save(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    save(m, filename, log, settings);
}

} // namespace vcl

#endif // VCL_IO_SAVE_H
