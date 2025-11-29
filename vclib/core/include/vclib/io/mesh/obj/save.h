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

#ifndef VCL_IO_MESH_OBJ_SAVE_H
#define VCL_IO_MESH_OBJ_SAVE_H

#include "material.h"

#include <vclib/io/file_info.h>
#include <vclib/io/image/save.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/io/write.h>

#include <vclib/space/complex.h>
#include <vclib/space/core.h>

namespace vcl {

namespace detail {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
std::vector<std::string> saveObjMaterials(
    const MeshType&     m,
    std::ostream&       mtlfp,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    std::vector<std::string> materials;
    if constexpr (HasMaterials<MeshType>) {
        materials.reserve(m.materialsNumber());
        for (uint i = 0; i < m.materialsNumber(); ++i) {
            const Material& mat = m.material(i);
            ObjMaterial     omat(mat, i);
            std::string     matName = mat.name();
            if (matName.empty()) {
                matName      = "MATERIAL_" + std::to_string(i);
                omat.matName = matName;
            }

            // TODO: save all the supported textures
            mtlfp << "newmtl " << matName << std::endl;
            mtlfp << omat << std::endl;
            materials.push_back(matName);

            if (settings.saveTextureImages) {
                const TextureDescriptor& t = mat.baseColorTextureDescriptor();
                const Image& img = m.textureImage(t.path());
                if (t.isNull()) {
                    log.log(
                        "Cannot save empty texture " + t.path(),
                        LogType::WARNING_LOG);
                }
                else {
                    try {
                        saveImage(img, m.meshBasePath() + t.path());
                    }
                    catch (const std::runtime_error& e) {
                        log.log(e.what(), LogType::WARNING_LOG);
                    }
                }
            }
        }
    }
    return materials;
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType&     m,
    const std::string&  filename,
    std::ostream&       fp,
    std::ostream*       mtlfp,
    bool                saveMtlFile,
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

    // if the mesh has both vertex and wedge texcords, will be saved just wedges
    // because obj does not allow to save them both. In any case, also vertex
    // texcoords will result saved as wedge texcoords in the final file.
    if (meshInfo.hasPerVertexTexCoord() &&
        meshInfo.hasPerFaceWedgeTexCoords()) {
        meshInfo.setPerVertexTexCoord(false);
    }

    std::ofstream mtlftmp;

    std::vector<std::string> materialNames;

    bool useMtl = meshInfo.hasMaterials();
    if (useMtl) {
        if (saveMtlFile) {
            std::string mtlFileName =
                FileInfo::fileNameWithExtension(filename) + ".mtl";

            mtlftmp = openOutputFileStream(
                FileInfo::pathWithoutFileName(filename) + mtlFileName);
            mtlfp = &mtlftmp;

            fp << "mtllib ./" << mtlFileName << std::endl;
        }
        else if (mtlfp == nullptr) {
            useMtl = false;
        }
    }

    if (useMtl) {
        materialNames = saveObjMaterials(m, *mtlfp, settings, log);
    }

    uint lastMaterial = UINT_NULL;

    // vertices
    using VertexType = MeshType::VertexType;

    fp << std::endl << "# Vertices" << std::endl;

    for (const VertexType& v : m.vertices()) {
        fp << "v ";
        io::writeDouble(fp, v.position().x(), false);
        io::writeDouble(fp, v.position().y(), false);
        io::writeDouble(fp, v.position().z(), false);
        if constexpr (HasPerVertexColor<MeshType>) {
            if (meshInfo.hasPerVertexColor()) {
                io::writeFloat(fp, v.color().redF(), false);
                io::writeFloat(fp, v.color().greenF(), false);
                io::writeFloat(fp, v.color().blueF(), false);
            }
        }
        fp << std::endl;

        if constexpr (HasPerVertexNormal<MeshType>) {
            if (meshInfo.hasPerVertexNormal()) {
                fp << "vn ";
                io::writeDouble(fp, v.normal().x(), false);
                io::writeDouble(fp, v.normal().y(), false);
                io::writeDouble(fp, v.normal().z(), false);
                fp << std::endl;
            }
        }
        if constexpr (HasPerVertexTexCoord<MeshType>) {
            if (meshInfo.hasPerVertexTexCoord()) {
                fp << "vt ";
                io::writeFloat(fp, v.texCoord().u(), false);
                io::writeFloat(fp, v.texCoord().v(), false);
                fp << std::endl;
            }
        }
    }

    // faces
    if constexpr (HasFaces<MeshType>) {
        using VertexType = MeshType::VertexType;
        using FaceType   = MeshType::FaceType;

        if (meshInfo.hasFaces()) {
            fp << std::endl << "# Faces" << std::endl;

            // indices of vertices that do not consider deleted vertices
            std::vector<uint> vIndices = m.vertexCompactIndices();

            uint wedgeTexCoord = 1;
            for (const FaceType& f : m.faces()) {
                if constexpr (HasMaterials<MeshType>) {
                    if (useMtl) { // mtl management
                        if (f.materialIndex() != lastMaterial) {
                            lastMaterial = f.materialIndex();
                            fp << "usemtl " << materialNames[lastMaterial]
                               << std::endl;
                        }
                    }
                }
                if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                    if (meshInfo.hasPerFaceWedgeTexCoords()) {
                        using WedgeTexCoordType = FaceType::WedgeTexCoordType;
                        for (const WedgeTexCoordType wt : f.wedgeTexCoords()) {
                            fp << "vt ";
                            io::writeFloat(fp, wt.u(), false);
                            io::writeFloat(fp, wt.v(), false);
                            fp << std::endl;
                        }
                    }
                }

                fp << "f ";
                for (const VertexType* v : f.vertices()) {
                    fp << vIndices[m.index(v)] + 1;
                    if constexpr (HasPerVertexTexCoord<MeshType>) {
                        // we wrote texcoords along with vertices, each texcoord
                        // has the same index of its vertex
                        if (meshInfo.hasPerVertexTexCoord()) {
                            fp << "/" << vIndices[m.index(v)] + 1;
                        }
                    }
                    if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                        // we wrote texcoords before the face; indices are
                        // consecutive and wedge coords are the same of the
                        // number of vertices of the face
                        if (meshInfo.hasPerFaceWedgeTexCoords()) {
                            fp << "/" << wedgeTexCoord++;
                        }
                    }
                    fp << " ";
                }
                fp << std::endl;
            }
        }
    }

    if constexpr (HasEdges<MeshType>) {
        using VertexType = MeshType::VertexType;
        using EdgeType   = MeshType::EdgeType;

        if (meshInfo.hasEdges()) {
            fp << std::endl << "# Edges" << std::endl;

            // indices of vertices that do not consider deleted vertices
            std::vector<uint> vIndices = m.vertexCompactIndices();

            for (const EdgeType& e : m.edges()) {
                fp << "l ";
                fp << vIndices[m.index(e.vertex(0))] + 1 << " ";
                fp << vIndices[m.index(e.vertex(1))] + 1 << std::endl;
            }
        }
    }
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType&     m,
    std::ostream&       fp,
    std::ostream&       mtlfp,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    detail::saveObj(m, "materials", fp, &mtlfp, false, settings, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType&     m,
    std::ostream&       fp,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    detail::saveObj(m, "", fp, nullptr, false, settings, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    std::ofstream fp = openOutputFileStream(filename, "obj");

    detail::saveObj(m, filename, fp, nullptr, true, settings, log);
}

} // namespace vcl

#endif // VCL_IO_MESH_OBJ_SAVE_H
