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

#ifndef VCL_IO_PLY_EXTRA_H
#define VCL_IO_PLY_EXTRA_H

#include <fstream>

#include <vclib/exceptions/io.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/io/read.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/logger.h>

#include "header.h"

namespace vcl::detail {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void readPlyTextures(
    const PlyHeader&    header,
    MeshType&           mesh,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    if constexpr (vcl::HasTexturePaths<MeshType>) {
        for (const std::string& str : header.textureFileNames()) {
            mesh.pushTexturePath(str);
        }
    }
    if constexpr (vcl::HasTextureImages<MeshType>) {
        if (settings.loadTextureImages) {
            for (const std::string& str : header.textureFileNames()) {
                vcl::Texture t;
                t.path() = str;
                bool b   = t.image().load(mesh.meshBasePath() + str);
                if (!b) {
                    log.log(LogType::WARNING, "Cannot load texture " + str);
                }
                mesh.pushTexture(t);
            }
        }
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void writePlyTextures(
    PlyHeader&          header,
    const MeshType&     mesh,
    LogType&            log,
    const SaveSettings& settings)
{
    if constexpr (vcl::HasTexturePaths<MeshType>) {
        for (const std::string& str : mesh.texturePaths()) {
            header.pushTextureFileName(str);
        }
    }
    if constexpr (vcl::HasTextureImages<MeshType>) {
        if (settings.saveTextureImages) {
            for (const vcl::Texture& t : mesh.textures()) {
                try {
                    t.image().save(mesh.meshBasePath() + t.path());
                }
                catch (const std::runtime_error& e) {
                    log.log(LogType::WARNING, e.what());
                }
            }
        }
    }
}

inline void readPlyUnknownElement(
    std::istream&    file,
    const PlyHeader& header,
    PlyElement       el)
{
    if (header.format() == ply::ASCII) {
        for (uint i = 0; i < el.numberElements; ++i) {
            readAndTokenizeNextNonEmptyLine(file);
        }
    }
    else {
        for (uint i = 0; i < el.numberElements; ++i) {
            for (const PlyProperty& p : el.properties) {
                if (p.list) {
                    uint s = io::readPrimitiveType<int>(file, p.listSizeType);
                    for (uint i = 0; i < s; ++i)
                        io::readPrimitiveType<int>(file, p.type);
                }
                else {
                    io::readPrimitiveType<int>(file, p.type);
                }
            }
        }
    }
}

} // namespace vcl::detail

#endif // VCL_IO_PLY_EXTRA_H
