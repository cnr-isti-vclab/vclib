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

#include <map>

namespace vcl {

namespace detail {

template<FaceConcept FaceType, MeshConcept MeshType>
ObjMaterial objMaterialFromFace(
    const FaceType& f,
    const MeshType& m,
    const MeshInfo& fi)
{
    ObjMaterial mat;
    if constexpr (HasPerFaceColor<MeshType>) {
        if (fi.hasPerFaceColor()) {
            mat.hasColor = true;
            mat.Kd.x()   = f.color().redF();
            mat.Kd.y()   = f.color().greenF();
            mat.Kd.z()   = f.color().blueF();
        }
    }
    // todo
    if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
        if (fi.hasPerFaceWedgeTexCoords()) {
            // todo
            if constexpr (HasTexturePaths<MeshType>) {
                mat.hasTexture = true;
                mat.map_Kd = m.texturePath(f.textureIndex());
            }
            if constexpr (HasMaterials<MeshType>) {
                const Material& matFace = m.material(f.textureIndex()); // todo
                if (matFace.baseColor() != Color::White) {
                    mat.hasColor = true;
                    mat.Kd.x()   = matFace.baseColor().redF();
                    mat.Kd.y()   = matFace.baseColor().greenF();
                    mat.Kd.z()   = matFace.baseColor().blueF();
                }
                if (!matFace.baseColorTexture().path().empty()) {
                    mat.hasTexture = true;
                    mat.map_Kd = matFace.baseColorTexture().path();
                    mat.mapId = f.textureIndex(); // todo
                }
                mat.Ns = (1.0f / (matFace.roughness() * matFace.roughness())) -
                         2.0f; // check
            }
        }
    }
    return mat;
}

template<
    FaceConcept   FaceType,
    MeshConcept   MeshType,
    LoggerConcept LogType = NullLogger>
void writeFaceObjMaterial(
    const FaceType&                     f,
    const MeshType&                     m,
    const MeshInfo&                     fi,
    ObjMaterial&                        lastMaterial,
    std::map<ObjMaterial, std::string>& materialMap,
    std::ostream&                       fp,
    std::ostream&                       mtlfp,
    const SaveSettings&                 settings,
    LogType&                            log = nullLogger)
{
    ObjMaterial    mat;

    mat = objMaterialFromFace(f, m, fi);

    if (!mat.isEmpty()) {
        static const std::string MATERIAL_PREFIX = "MATERIAL_";
        std::string              mname; // name of the material of the vertex
        auto                     it = materialMap.find(mat);
        if (it == materialMap.end()) { // if it is a new material
            // add the new material to the map

            // TODO: try to preserve original material names
            mname = MATERIAL_PREFIX + std::to_string(materialMap.size());
            materialMap[mat] = mname;
            // save the material in the mtl file
            mtlfp << "newmtl " << mname << std::endl;
            mtlfp << mat << std::endl;
            if constexpr (HasTextureImages<MeshType>) {
                if (settings.saveTextureImages && mat.hasTexture) {
                    // we need to save the texture image
                    // first, get the index of the texture: 0 if vertex,
                    // textureIndex if face
                    uint textureIndex = f.textureIndex();
                    const Texture& t = m.texture(textureIndex);
                    try {
                        saveImage(t.image(), m.meshBasePath() + mat.map_Kd);
                    }
                    catch (const std::runtime_error& e) {
                        log.log(e.what(), LogType::WARNING_LOG);
                    }
                }
            }
        }
        else { // get the name of the material
            mname = it->second;
        }
        // if the material of the vertex is different from the last used, need
        // to add usemtl
        if (mat != lastMaterial) {
            lastMaterial = mat;
            fp << "usemtl " << mname << std::endl;
        }
    }
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

    std::ofstream                              mtlftmp;
    std::map<detail::ObjMaterial, std::string> materialMap;

    bool useMtl =
        meshInfo.hasPerFaceColor() ||
        (meshInfo.hasTextures() && (meshInfo.hasPerVertexTexCoord() ||
                                    meshInfo.hasPerFaceWedgeTexCoords()));
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

    detail::ObjMaterial lastMaterial;

    // vertices
    using VertexType = MeshType::VertexType;

    fp << std::endl << "# Vertices" << std::endl;

    for (const VertexType& v : m.vertices()) {
        fp << "v ";
        io::writeDouble(fp, v.position().x(), false);
        io::writeDouble(fp, v.position().y(), false);
        io::writeDouble(fp, v.position().z(), false);
        if constexpr(HasPerVertexColor<MeshType>) {
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
                if (useMtl) { // mtl management
                    detail::writeFaceObjMaterial(
                        f,
                        m,
                        meshInfo,
                        lastMaterial,
                        materialMap,
                        fp,
                        *mtlfp,
                        settings,
                        log);
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
