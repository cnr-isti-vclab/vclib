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

#ifndef VCL_IO_OBJ_SAVE_H
#define VCL_IO_OBJ_SAVE_H

#include <map>

#include <vclib/exceptions/io_exceptions.h>
#include <vclib/io/file_info.h>
#include <vclib/io/write.h>
#include <vclib/mesh/utils/mesh_info.h>
#include <vclib/misc/logger.h>

#include "material.h"

namespace vcl {

namespace detail {

template<VertexConcept VertexType, MeshConcept MeshType>
ObjMaterial objMaterialFromVertex(const VertexType& v, const MeshInfo& fi)
{
    ObjMaterial mat;
    if constexpr (HasPerVertexColor<MeshType>) {
        if (fi.hasVertexColors()) {
            mat.hasColor = true;
            mat.Kd.x()   = v.color().redF();
            mat.Kd.y()   = v.color().greenF();
            mat.Kd.z()   = v.color().blueF();
        }
    }
    return mat;
}

template<FaceConcept FaceType, MeshConcept MeshType>
ObjMaterial objMaterialFromFace(
    const FaceType& f,
    const MeshType& m,
    const MeshInfo& fi)
{
    ObjMaterial mat;
    if (fi.hasFaceColors()) {
        mat.hasColor = true;
        mat.Kd.x()   = f.color().redF();
        mat.Kd.y()   = f.color().greenF();
        mat.Kd.z()   = f.color().blueF();
    }
    if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
        if (fi.hasFaceWedgeTexCoords()) {
            mat.hasTexture = true;
            mat.map_Kd     = m.texturePath(f.textureIndex());
        }
    }
    return mat;
}

template<ElementConcept ElementType, MeshConcept MeshType>
void writeElementObjMaterial(
    const ElementType&                  e,
    const MeshType&                     m,
    const MeshInfo&                     fi,
    ObjMaterial&                        lastMaterial,
    std::map<ObjMaterial, std::string>& materialMap,
    std::ostream&                       fp,
    std::ostream&                       mtlfp)
{
    ObjMaterial mat;
    if constexpr (std::is_same<ElementType, typename MeshType::VertexType>::
                      value)
        mat = objMaterialFromVertex<typename MeshType::VertexType, MeshType>(
            e, fi);
    if constexpr (HasFaces<MeshType>)
        if constexpr (std::is_same<ElementType, typename MeshType::FaceType>::
                          value)
            mat = objMaterialFromFace(e, m, fi);
    if (!mat.isEmpty()) {
        static const std::string MATERIAL_PREFIX = "MATERIAL_";
        std::string              mname; // name of the material of the vertex
        auto                     it = materialMap.find(mat);
        if (it == materialMap.end()) { // if it is a new material
            // add the new material to the map
            mname = MATERIAL_PREFIX + std::to_string(materialMap.size());
            materialMap[mat] = mname;
            // save the material in the mtl file
            mtlfp << "newmtl " << mname << std::endl;
            mtlfp << mat << std::endl;
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
    const MeshType&    m,
    const std::string& filename,
    std::ostream&      fp,
    std::ostream*      mtlfp,
    bool               saveMtlFile,
    const MeshInfo&    info,
    LogType&           log = nullLogger)
{
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    meshInfo = info.intersect(meshInfo);

    // if the mesh has both vertex and wedge texcords, will be saved just wedges
    // because obj does not allow to save them both. In any case, also vertex
    // texcoords will result saved as wedge texcoords in the final file.
    if (meshInfo.hasVertexTexCoords() && meshInfo.hasFaceWedgeTexCoords()) {
        meshInfo.setVertexTexCoords(false);
    }

    std::ofstream                              mtlftmp;
    std::map<detail::ObjMaterial, std::string> materialMap;

    bool useMtl =
        meshInfo.hasVertexColors() || meshInfo.hasFaceColors() ||
        (meshInfo.hasTextures() &&
         (meshInfo.hasVertexTexCoords() || meshInfo.hasFaceWedgeTexCoords()));
    if (useMtl) {
        if (saveMtlFile) {
            mtlftmp = std::ofstream(
                FileInfo::fileNameWithExtension(filename) + ".mtl");
            mtlfp = &mtlftmp;
            std::string mtlFileName =
                FileInfo::fileNameWithExtension(filename) + ".mtl";
            fp << "mtllib ./" << mtlFileName << std::endl;
        }
        else if (mtlfp == nullptr) {
            useMtl = false;
        }
    }

    detail::ObjMaterial lastMaterial;

    // vertices
    using VertexType = MeshType::VertexType;
    for (const VertexType& v : m.vertices()) {
        if (useMtl) { // mtl management
            detail::writeElementObjMaterial<VertexType, MeshType>(
                v, m, meshInfo, lastMaterial, materialMap, fp, *mtlfp);
        }
        fp << "v ";
        io::writeDouble(fp, v.coord().x(), false);
        io::writeDouble(fp, v.coord().y(), false);
        io::writeDouble(fp, v.coord().z(), false);
        fp << std::endl;

        if constexpr (HasPerVertexNormal<MeshType>) {
            if (meshInfo.hasVertexNormals()) {
                fp << "vn ";
                io::writeDouble(fp, v.normal().x(), false);
                io::writeDouble(fp, v.normal().y(), false);
                io::writeDouble(fp, v.normal().z(), false);
                fp << std::endl;
            }
        }
        if constexpr (HasPerVertexTexCoord<MeshType>) {
            if (meshInfo.hasVertexTexCoords()) {
                fp << "vt ";
                io::writeFloat(fp, v.texCoord().u(), false);
                io::writeFloat(fp, v.texCoord().v(), false);
                fp << std::endl;
            }
        }
    }

    // faces
    if constexpr (vcl::HasFaces<MeshType>) {
        using VertexType = MeshType::VertexType;
        using FaceType   = MeshType::FaceType;

        // indices of vertices that do not consider deleted vertices
        std::vector<uint> vIndices = m.vertexCompactIndices();

        uint wedgeTexCoord = 1;
        for (const FaceType& f : m.faces()) {
            if (useMtl) { // mtl management
                detail::writeElementObjMaterial(
                    f, m, meshInfo, lastMaterial, materialMap, fp, *mtlfp);
            }
            if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                if (meshInfo.hasFaceWedgeTexCoords()) {
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
                    // we wrote texcoords along with vertices, each texcoord has
                    // the same index of its vertex
                    if (meshInfo.hasVertexTexCoords()) {
                        fp << "/" << vIndices[m.index(v)] + 1;
                    }
                }
                if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                    // we wrote texcoords before the face; indices are
                    // consecutive and wedge coords are the same of the number
                    // of vertices of the face
                    if (meshInfo.hasFaceWedgeTexCoords()) {
                        fp << "/" << wedgeTexCoord++;
                    }
                }
                fp << " ";
            }
            fp << std::endl;
        }
    }
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType& m,
    std::ostream&   fp,
    std::ostream&   mtlfp,
    const MeshInfo& info,
    LogType&        log = nullLogger)
{
    detail::saveObj(m, "materials", fp, &mtlfp, false, info, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType& m,
    std::ostream&   fp,
    const MeshInfo& info,
    LogType&        log = nullLogger)
{
    detail::saveObj(m, "", fp, nullptr, false, info, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType& m,
    std::ostream&   fp,
    std::ostream&   mtlfp,
    LogType&        log = nullLogger)
{
    MeshInfo info(m);
    saveObj(m, fp, mtlfp, info, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(const MeshType& m, std::ostream& fp, LogType& log = nullLogger)
{
    MeshInfo info(m);
    saveObj(m, fp, info, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType&    m,
    const std::string& filename,
    const MeshInfo&    info,
    LogType&           log = nullLogger)
{
    std::ofstream fp = openOutputFileStream(filename, "obj");

    detail::saveObj(m, filename, fp, nullptr, true, info, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveObj(
    const MeshType&    m,
    const std::string& filename,
    LogType&           log = nullLogger)
{
    MeshInfo info(m);
    saveObj(m, filename, info, log);
}

} // namespace vcl

#endif // VCL_IO_OBJ_SAVE_H
