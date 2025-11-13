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

#ifndef VCL_IO_MESH_PLY_DETAIL_EXTRA_H
#define VCL_IO_MESH_PLY_DETAIL_EXTRA_H

#include "header.h"

#include <vclib/io/image.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/io/read.h>

#include <vclib/mesh.h>
#include <vclib/space/core.h>

namespace vcl::detail {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void readPlyTextures(
    const PlyHeader&    header,
    MeshType&           mesh,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    if constexpr (HasMaterials<MeshType>) {
        for (const std::string& str : header.textureFileNames()) {
            Material mat;
            mat.name() = FileInfo::fileNameWithExtension(str);
            mat.baseColorTexture().path() = str;
            if (settings.loadTextureImages) {
                mat.baseColorTexture().image() =
                    loadImage(mesh.meshBasePath() + str);
                if (mat.baseColorTexture().image().isNull()) {
                    log.log(
                        "Cannot load texture " + str, LogType::WARNING_LOG);
                }
            }
            mesh.pushMaterial(mat);
        }
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void writePlyTextures(
    PlyHeader&          header,
    const MeshType&     mesh,
    const std::string&  basePath,
    LogType&            log,
    const SaveSettings& settings)
{
    if constexpr (HasMaterials<MeshType>) {
        for (uint k = 0; const Material& mat : mesh.materials()) {
            header.pushTextureFileName(mat.baseColorTexture().path());
            if (settings.saveTextureImages) {
                try {
                    saveImage(
                        mat.baseColorTexture().image(),
                        basePath + mat.baseColorTexture().path());
                }
                catch (const std::runtime_error& e) {
                    log.log(e.what(), LogType::WARNING_LOG);
                }
            }
            ++k;
        }
    }
}

template<LoggerConcept LogType>
void readPlyUnknownElement(
    std::istream&    file,
    const PlyHeader& header,
    PlyElement       el,
    LogType&         log)
{
    log.startProgress("Reading unknown elements", el.numberElements);

    if (header.format() == ply::ASCII) {
        for (uint i = 0; i < el.numberElements; ++i) {
            readAndTokenizeNextNonEmptyLine(file);
            log.progress(i);
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
            log.progress(i);
        }
    }

    log.endProgress();
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_PLY_DETAIL_EXTRA_H
