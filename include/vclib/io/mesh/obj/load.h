/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_IO_OBJ_LOAD_H
#define VCL_IO_OBJ_LOAD_H

#include <map>

#include <vclib/algorithms/polygon.h>
#include <vclib/io/read.h>
#include <vclib/misc/file_info.h>
#include <vclib/misc/logger.h>
#include <vclib/misc/mesh_info.h>

#include "material.h"

namespace vcl{

namespace detail {

template<MeshConcept MeshType>
using ObjNormalsMap =
    std::conditional_t<
        HasPerVertexNormal<MeshType>,
        std::map<uint, typename MeshType::VertexType::NormalType>,
        std::map<uint, vcl::Point3d>>;

template<MeshConcept MeshType>
void loadObjMaterials(
    std::map<std::string, ObjMaterial>& materialMap,
    MeshType& mesh,
    const std::string& mtllib)
{
    std::ifstream file = openInputFileStream(mtllib);
    std::string matName;
    ObjMaterial mat;

    do {
        vcl::Tokenizer tokens = readAndTokenizeNextNonEmptyLineNoThrow(file);
        if (file) {
            // counter for texture images, used when mesh has no texture files
            uint nt = 0;
            vcl::Tokenizer::iterator token = tokens.begin();
            std::string header = *token++;
            if (header == "newmtl"){
                if (!matName.empty())
                    materialMap[matName] = mat;
                mat = ObjMaterial();
                matName = *token;
            }
            if (header == "Ka") {
                if (tokens.size() >= 4) {
                    if (*token != "spectral" && *token != "xyz") {
                        mat.Ka.x() = io::readFloat<float>(token);
                        mat.Ka.y() = io::readFloat<float>(token);
                        mat.Ka.z() = io::readFloat<float>(token);
                    }
                }
            }
            if (header == "Kd") {
                if (tokens.size() >= 4) {
                    if (*token != "spectral" && *token != "xyz") {
                        mat.Kd.x() = io::readFloat<float>(token);
                        mat.Kd.y() = io::readFloat<float>(token);
                        mat.Kd.z() = io::readFloat<float>(token);
                        mat.hasColor = true;
                    }
                }
            }
            if (header == "Ks") {
                if (tokens.size() >= 4) {
                    if (*token != "spectral" && *token != "xyz") {
                        mat.Ks.x() = io::readFloat<float>(token);
                        mat.Ks.y() = io::readFloat<float>(token);
                        mat.Ks.z() = io::readFloat<float>(token);
                    }
                }
            }
            if (header == "d") {
                if ((*token)[0] == '-') token++;
                mat.d = io::readFloat<float>(token);
            }
            if (header == "Tr") {
                if ((*token)[0] == '-') token++;
                mat.d = 1 - io::readFloat<float>(token);
            }
            if (header == "Ns") {
                mat.Ns = io::readFloat<float>(token);
            }
            if (header == "illum") {
                mat.illum = io::readFloat<int>(token);
            }
            if (header == "map_Kd") {
                // need to manage args
                while ((*token)[0] == '-') {
                    if (*token == "-o" || *token == "-s" || *token == "-t"){
                        // ignore the argument and the three values
                        ++token; ++token; ++token; ++token;
                    }
                    if (*token == "-mm"){
                        // ignore the argument and the two values
                        ++token; ++token; ++token;
                    }
                    if (*token == "-blendu" || *token == "-blendv" ||
                        *token == "-cc" || *token == "-clamp" ||
                        *token == "-texres")
                    {
                        // ignore the argument and the value
                        ++token; ++token;
                    }
                }
                mat.map_Kd = *token;
                mat.hasTexture = true;
                if constexpr (HasTexturePaths<MeshType>) {
                    mat.mapId = mesh.textureNumber();
                    mesh.pushTexturePath(mat.map_Kd);
                }
                else {
                    mat.mapId = nt++;
                }
            }
        }
    } while(file);
    if (!matName.empty())
        materialMap[matName] = mat;
}

template<MeshConcept MeshType>
void loadObjVertex(
    MeshType&                 m,
    vcl::Tokenizer::iterator& token,
    MeshInfo&                 loadedInfo,
    const vcl::Tokenizer&     tokens,
    const ObjMaterial&      currentMaterial,
    bool                      enableOptionalComponents)
{
    // first, need to set that I'm loading vertices
    if (m.vertexNumber() == 0) {
        loadedInfo.setVertices();
        loadedInfo.setVertexCoords();
    }
    uint vid = m.addVertex();
    for (uint i = 0; i < 3; ++i) {
        m.vertex(vid).coord()[i] = io::readDouble<double>(token);
    }
    if constexpr (HasPerVertexColor<MeshType>){
        if (vid == 0) {
            // if the current material has a valid color, of the file stores the
            // vertex color in the non-standard way (color values after the
            // coordinates)
            if (currentMaterial.hasColor || tokens.size() > 6) {
                if (enableOptionalComponents) {
                    enableIfPerVertexColorOptional(m);
                    loadedInfo.setVertexColors();
                }
                else {
                    if (isPerVertexColorAvailable(m))
                        loadedInfo.setVertexColors();
                }
            }
        }
        if (loadedInfo.hasVertexColors()) {
            // the file has the nonstandard way to store vertex colors, after
            // the coords...
            if (tokens.size() > 6) {
                m.vertex(vid).color().setRedF(
                    io::readFloat<float>(token));
                m.vertex(vid).color().setGreenF(
                    io::readFloat<float>(token));
                m.vertex(vid).color().setBlueF(
                    io::readFloat<float>(token));
            }
            else if (currentMaterial.hasColor) {
                m.vertex(vid).color() = currentMaterial.color();
            }
        }
    }
}

template<MeshConcept MeshType>
void loadObjVertexNormal(
    MeshType&                       m,
    detail::ObjNormalsMap<MeshType>& mapNormalsCache,
    uint                            vn,
    vcl::Tokenizer::iterator&       token,
    MeshInfo&                       loadedInfo,
    bool                            enableOptionalComponents)
{
    using NormalType = MeshType::VertexType::NormalType;

    // first, need to check if I can store normals in the mesh
    if (vn == 0) {
        if (enableOptionalComponents) {
            enableIfPerVertexNormalOptional(m);
            loadedInfo.setVertexNormals();
        }
        else {
            if (isPerVertexNormalAvailable(m))
                loadedInfo.setVertexNormals();
        }
    }
    if (loadedInfo.hasVertexNormals()) {
        // read the normal
        NormalType n;
        for (uint i = 0; i < 3; ++i) {
            n[i] = io::readDouble<typename NormalType::ScalarType>(token);
        }
        // I can store the normal in its vertex
        if (m.vertexNumber() > vn) {
            m.vertex(vn).normal() = n;
        }
        // read the normal and save it in the cache map, because we still don't
        // have read the vertex corresponding to the current normal
        else {
            mapNormalsCache[vn] = n;
        }
    }
}

template<FaceMeshConcept MeshType>
void loadObjFace(
    MeshType&                          m,
    MeshInfo&                          loadedInfo,
    const vcl::Tokenizer&              tokens,
    const std::vector<vcl::TexCoordd>& wedgeTexCoords,
    const ObjMaterial&               currentMaterial,
    bool                               enableOptionalComponents)
{
    using FaceType = MeshType::FaceType;

    std::vector<uint> vids;
    std::vector<uint> wids;

    // actual read - load vertex indices and texcoords indices, if present
    vcl::Tokenizer::iterator token = tokens.begin();
    ++token;
    vids.resize(tokens.size()-1);
    wids.reserve(tokens.size()-1);
    for (uint i = 0; i < tokens.size()-1; ++i) {
        vcl::Tokenizer subt(*token, '/', false);
        auto t = subt.begin();
        vids[i] = io::readUInt<uint>(t) - 1;
        if (subt.size() > 1) {
            if (!t->empty()) {
                wids.push_back(io::readUInt<uint>(t) - 1);
            }
        }
        ++token;
    }

    // add the face
    uint fid = m.addFace();
    FaceType& f = m.face(fid);

    // check if we need to split the face we read into triangles
    bool splitFace = false;
    // we have a polygonal mesh, no need to split
    if constexpr (FaceType::VERTEX_NUMBER < 0) {
        // need to resize to the right number of verts
        f.resizeVertices(tokens.size()-1);
    }
    else if (FaceType::VERTEX_NUMBER != tokens.size()-1) {
        // we have faces with static sizes (triangles), but we are loading faces
        // with number of verts > 3. Need to split the face we are loading in n
        // faces!
        splitFace = true;
    }

    // create the face in the mesh, for now we manage only vertex indices
    if (!splitFace) { // no need to split face case
        for (uint i = 0; i < vids.size(); ++i) {
            if (vids[i] >= m.vertexNumber()) {
                throw vcl::MalformedFileException(
                    "Bad vertex index for face " + std::to_string(fid));
            }
            f.vertex(i) = &m.vertex(vids[i]);
        }
    }
    else { // split needed
        addTriangleFacesFromPolygon(m, f, vids);
    }

    // color
    if (HasPerFaceColor<MeshType>){
        // if the first face, we need to check if I can store colors
        if (fid == 0) {
            // if the current material has no color, we assume that the file has
            // no face color
            if (currentMaterial.hasColor) {
                if (enableOptionalComponents) {
                    enableIfPerFaceColorOptional(m);
                    loadedInfo.setFaceColors();
                }
                else {
                    if (isPerFaceColorAvailable(m))
                        loadedInfo.setFaceColors();
                }
            }
        }
        if (loadedInfo.hasFaceColors()) {
            if (currentMaterial.hasColor) {
                // in case the loaded polygon has been triangulated in the last
                // n triangles of mesh
                for (uint ff = fid; ff < m.faceNumber(); ++ff) {
                    m.face(ff).color() = currentMaterial.color();
                }
            }
        }
    }

    // wedge coords
    if constexpr(HasPerFaceWedgeTexCoords<MeshType>) {
        // first, need to check if I can store wedge texcoords in the mesh
        if (fid == 0) {
            // if the current face has the right number of wedge texcoords, we
            // assume that we can load wedge texcoords
            if (wids.size() == vids.size()) {
                if (enableOptionalComponents) {
                    enableIfPerFaceWedgeTexCoordsOptional(m);
                    loadedInfo.setFaceWedgeTexCoords();
                }
                else {
                    if (isPerFaceWedgeTexCoordsAvailable(m))
                        loadedInfo.setFaceWedgeTexCoords();
                }
            }
        }
        if (loadedInfo.hasFaceWedgeTexCoords()) {
            if (wids.size() == vids.size()) {
                if (!splitFace) { // there wasn't a triangulation of the face
                    // it is safe to assign each wedge texcoord to its position
                    // in the face
                    for (uint i = 0; i < wids.size(); ++i) {
                        if (wids[i] >= wedgeTexCoords.size()) {
                            throw vcl::MalformedFileException(
                                "Bad texcoord index for face " +
                                std::to_string(fid));
                        }
                        f.wedgeTexCoord(i) =
                            wedgeTexCoords[wids[i]]
                                .cast<typename FaceType::WedgeTexCoordType::
                                          ScalarType>();
                        if (currentMaterial.hasTexture){
                            f.textureIndex() = currentMaterial.mapId;
                        }
                    }
                }
                else {
                    // take read texcoords and map them in the triangulated
                    // faces for each face of the triangulation of the polygon
                    for (uint ff = fid; ff < m.faceNumber(); ++ff) {
                        FaceType& f = m.face(ff);
                        // for each vertex of the face
                        for (uint i = 0; i < f.vertexNumber(); ++i){
                            uint vid = m.index(f.vertex(i));
                            // find the position of the vertex in the vids array
                            auto it = std::find(vids.begin(), vids.end(), vid);
                            assert(it != vids.end());
                            uint pos = it - vids.begin();
                            // check that the texcoord id is valid
                            if (wids[pos] >= wedgeTexCoords.size()) {
                                throw vcl::MalformedFileException(
                                    "Bad texcoord index for face " +
                                    std::to_string(fid));
                            }
                            // set the wedge texcoord in the same position of
                            // the vertex
                            f.wedgeTexCoord(i) =
                                wedgeTexCoords[wids[pos]]
                                    .cast<typename FaceType::WedgeTexCoordType::
                                              ScalarType>();
                            if (currentMaterial.hasTexture){
                                f.textureIndex() = currentMaterial.mapId;
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadObj(
    MeshType&          m,
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    std::ifstream file = openInputFileStream(filename);
    // save normals if they can't be stored directly into vertices
    detail::ObjNormalsMap<MeshType> mapNormalsCache;
    uint vn = 0; // number of vertex normals read
    // save array of texcoords, that are stored later (into wedges when loading
    // faces or into vertices as a fallback)
    std::vector<vcl::TexCoordd> texCoords;

    // map of materials loaded
    std::map<std::string, detail::ObjMaterial> materialMap;
    detail::ObjMaterial
        currentMaterial; // the current material, set by 'usemtl'

    // some obj files do not declare the material file name with mtllib, but
    // they assume that material file has the same name of the obj file.
    // Therefore, we first load this file if it exists.
    std::string stdmtlfile = FileInfo::pathWithoutFileName(filename) +
                             FileInfo::fileNameWithoutExtension(filename) +
                             ".mtl";
    try {
        detail::loadObjMaterials(materialMap, m, stdmtlfile);
    }
    catch(vcl::CannotOpenFileException){
        // nothing to do, this file was missing but was a fallback for some type
        // of files...
    }

    if constexpr (HasTexturePaths<MeshType>) {
        m.meshBasePath() = FileInfo::pathWithoutFileName(filename);
    }

    if constexpr (HasName<MeshType>) {
        m.name() = FileInfo::fileNameWithoutExtension(filename);
    }

    // cycle that reads line by line
    do {
        vcl::Tokenizer tokens = readAndTokenizeNextNonEmptyLineNoThrow(file);
        if (file) {
            vcl::Tokenizer::iterator token = tokens.begin();
            std::string header = *token++;
            if (header == "mtllib") { // material file
                std::string mtlfile =
                    FileInfo::pathWithoutFileName(filename) + *token;
                detail::loadObjMaterials(materialMap, m, mtlfile);
            }
            // use a new material - change currentMaterial
            if (header == "usemtl") {
                std::string matname = *token;
                auto it = materialMap.find(matname);
                if (it != materialMap.end()) {
                    currentMaterial = it->second;
                }
            }
            // read vertex (and for some non-standard obj files, also vertex
            // color)
            if (header == "v") {
                detail::loadObjVertex(
                    m,
                    token,
                    loadedInfo,
                    tokens,
                    currentMaterial,
                    enableOptionalComponents);
            }
            // read vertex normal (and save in vn how many normals we read)
            if constexpr(HasPerVertexNormal<MeshType>) {
                if (header == "vn") {
                    detail::loadObjVertexNormal(
                        m,
                        mapNormalsCache,
                        vn,
                        token,
                        loadedInfo,
                        enableOptionalComponents);
                    vn++;
                }
            }
            // read texcoords and save them in the vector of texcoords, we will
            // store them in the mesh later
            if constexpr (
                HasPerVertexTexCoord<MeshType> ||
                HasPerFaceWedgeTexCoords<MeshType>)
            {
                if (header == "vt") {
                    // save the texcoord for later
                    TexCoordd tf;
                    for (uint i = 0; i < 2; ++i) {
                        tf[i] = io::readDouble<double>(token);
                    }
                    texCoords.push_back(tf);
                }
            }
            // read faces and manage:
            // - color
            // - eventual texcoords
            // - possibility to split polygonal face into several triangles
            if constexpr (HasFaces<MeshType>) {
                if (header == "f") {
                    detail::loadObjFace(
                        m,
                        loadedInfo,
                        tokens,
                        texCoords,
                        currentMaterial,
                        enableOptionalComponents);
                }
            }
        }
    } while (file);

    if constexpr (HasPerVertexNormal<MeshType>) {
        // set all vertex normals that have not been stored in vertices
        for (const auto& p : mapNormalsCache) {
            if (p.first < m.vertexNumber()) {
                m.vertex(p.first).normal() = p.second;
            }
        }
    }
    if constexpr (HasPerVertexTexCoord<MeshType>) {
        using VertexType = MeshType::VertexType;
        if (!loadedInfo.hasFaceWedgeTexCoords()) {
            // we can set the loaded texCoords to vertices, also if they are not
            // supported in obj
            if (texCoords.size() == m.vertexNumber()) {
                if (enableOptionalComponents) {
                    enableIfPerVertexTexCoordOptional(m);
                    loadedInfo.setVertexTexCoords();
                }
                else {
                    if (isPerVertexTexCoordAvailable(m))
                        loadedInfo.setVertexTexCoords();
                }
                if (loadedInfo.hasVertexTexCoords()) {
                    uint i = 0;
                    for (VertexType& v : m.vertices()) {
                        v.texCoord() =
                            texCoords[i++]
                                .cast<typename VertexType::TexCoordType::
                                      ScalarType>();
                    }
                }
            }
        }
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadObj(
    MeshType&          m,
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    loadObj(m, filename, loadedInfo, log, enableOptionalComponents);
}


template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadObj(
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshType m;
    loadObj(m, filename, loadedInfo, log, enableOptionalComponents);
    return m;
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadObj(
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    return loadObj<MeshType>(
        filename, loadedInfo, log, enableOptionalComponents);
}


} // namespace vcl

#endif // VCL_IO_OBJ_LOAD_H
