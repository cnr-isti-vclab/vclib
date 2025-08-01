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

#ifndef VCL_IO_MESH_OFF_SAVE_H
#define VCL_IO_MESH_OFF_SAVE_H

#include <vclib/io/mesh/settings.h>
#include <vclib/io/write.h>

#include <vclib/exceptions.h>
#include <vclib/space/complex.h>

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveOff(
    const MeshType&     m,
    std::ostream&       fp,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    if (!settings.info.isEmpty())
        meshInfo = settings.info.intersect(meshInfo);

    if (meshInfo.hasPerVertexNormal())
        fp << "N";
    if (meshInfo.hasPerVertexColor())
        fp << "C";
    if (meshInfo.hasPerVertexTexCoord())
        fp << "ST";
    fp << "OFF" << std::endl;

    uint vn = 0;
    uint fn = 0;
    uint en = 0;
    if constexpr (HasVertices<MeshType>) {
        vn = m.vertexNumber();
    }
    if constexpr (HasFaces<MeshType>) {
        fn = m.faceNumber();
    }

    io::writeInt(fp, vn, false);
    io::writeInt(fp, fn, false);
    io::writeInt(fp, en, false);
    fp << std::endl; // remove last char (a space) and add a newline

    // vertices
    if constexpr (HasVertices<MeshType>) {
        using VertexType = MeshType::VertexType;
        for (const VertexType& v : m.vertices()) {
            io::writeDouble(fp, v.position().x(), false);
            io::writeDouble(fp, v.position().y(), false);
            io::writeDouble(fp, v.position().z(), false);

            if constexpr (HasPerVertexColor<MeshType>) {
                if (meshInfo.hasPerVertexColor()) {
                    io::writeInt(fp, v.color().red(), false);
                    io::writeInt(fp, v.color().green(), false);
                    io::writeInt(fp, v.color().blue(), false);
                    io::writeInt(fp, v.color().alpha(), false);
                }
            }
            if constexpr (HasPerVertexNormal<MeshType>) {
                if (meshInfo.hasPerVertexNormal()) {
                    io::writeDouble(fp, v.normal().x(), false);
                    io::writeDouble(fp, v.normal().y(), false);
                    io::writeDouble(fp, v.normal().z(), false);
                }
            }
            if constexpr (HasPerVertexTexCoord<MeshType>) {
                if (meshInfo.hasPerVertexTexCoord()) {
                    io::writeDouble(fp, v.texCoord().u(), false);
                    io::writeDouble(fp, v.texCoord().v(), false);
                }
            }

            fp << std::endl;
        }
    }

    // faces
    if constexpr (HasFaces<MeshType>) {
        using VertexType = MeshType::VertexType;
        using FaceType   = MeshType::FaceType;

        // indices of vertices that do not consider deleted vertices
        std::vector<uint> vIndices = m.vertexCompactIndices();

        for (const FaceType& f : m.faces()) {
            io::writeInt(fp, f.vertexNumber(), false);
            for (const VertexType* v : f.vertices()) {
                io::writeInt(fp, vIndices[m.index(v)], false);
            }
            if constexpr (HasPerFaceColor<MeshType>) {
                if (meshInfo.hasPerFaceColor()) {
                    io::writeInt(fp, f.color().red(), false);
                    io::writeInt(fp, f.color().green(), false);
                    io::writeInt(fp, f.color().blue(), false);
                    io::writeInt(fp, f.color().alpha(), false);
                }
            }

            fp << std::endl;
        }
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveOff(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    std::ofstream fp = openOutputFileStream(filename, "off");
    saveOff(m, fp, settings, log);
    fp.close();
}

} // namespace vcl

#endif // VCL_IO_MESH_OFF_SAVE_H
