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

#ifndef VCL_IO_MESH_OBJ_LOAD_H
#define VCL_IO_MESH_OBJ_LOAD_H

#include "material.h"

#include <vclib/algorithms/mesh/face_topology.h>
#include <vclib/io/file_info.h>
#include <vclib/io/image/load.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/io/read.h>
#include <vclib/misc/logger.h>
#include <vclib/space/complex/mesh_info.h>
#include <vclib/space/core/texture.h>

#include <algorithm>
#include <map>

namespace vcl {

namespace detail {

template<MeshConcept MeshType>
using ObjNormalsMap = std::conditional_t<
    HasPerVertexNormal<MeshType>,
    std::map<uint, typename MeshType::VertexType::NormalType>,
    std::map<uint, Point3d>>;

template<MeshConcept MeshType>
void loadObjMaterials(
    std::map<std::string, ObjMaterial>& materialMap,
    MeshType&                           mesh,
    std::istream&                       stream,
    MeshInfo&                           loadedInfo,
    const LoadSettings&                 settings)
{
    std::string matName;
    ObjMaterial mat;

    do {
        Tokenizer tokens = readAndTokenizeNextNonEmptyLineNoThrow(stream);
        if (stream) {
            // counter for texture images, used when mesh has no texture files
            uint                nt     = 0;
            Tokenizer::iterator token  = tokens.begin();
            std::string         header = *token++;
            if (header == "newmtl") {
                if (!matName.empty())
                    materialMap[matName] = mat;
                mat     = ObjMaterial();
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
                        mat.Kd.x()   = io::readFloat<float>(token);
                        mat.Kd.y()   = io::readFloat<float>(token);
                        mat.Kd.z()   = io::readFloat<float>(token);
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
                if ((*token)[0] == '-')
                    token++;
                mat.d = io::readFloat<float>(token);
            }
            if (header == "Tr") {
                if ((*token)[0] == '-')
                    token++;
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
                    if (*token == "-o" || *token == "-s" || *token == "-t") {
                        // ignore the argument and the three values
                        ++token;
                        ++token;
                        ++token;
                        ++token;
                    }
                    if (*token == "-mm") {
                        // ignore the argument and the two values
                        ++token;
                        ++token;
                        ++token;
                    }
                    if (*token == "-blendu" || *token == "-blendv" ||
                        *token == "-cc" || *token == "-clamp" ||
                        *token == "-texres") {
                        // ignore the argument and the value
                        ++token;
                        ++token;
                    }
                }
                mat.map_Kd = *token;
                // replace backslashes with slashes - windows compatibility
                std::ranges::replace(mat.map_Kd, '\\', '/');
                mat.hasTexture = true;
                if constexpr (HasTexturePaths<MeshType>) {
                    loadedInfo.setTextures();
                    mat.mapId = mesh.textureNumber();
                    mesh.pushTexturePath(mat.map_Kd);
                }
                else {
                    mat.mapId = nt++;
                }
            }
        }
    } while (stream);
    if (!matName.empty())
        materialMap[matName] = mat;
}

template<MeshConcept MeshType>
void loadObjMaterials(
    std::map<std::string, ObjMaterial>& materialMap,
    MeshType&                           mesh,
    const std::string&                  mtllib,
    MeshInfo&                           loadedInfo,
    const LoadSettings&                 settings)
{
    std::ifstream file = openInputFileStream(mtllib);
    loadObjMaterials(materialMap, mesh, file, loadedInfo, settings);
}

template<MeshConcept MeshType>
void readObjVertex(
    MeshType&            m,
    Tokenizer::iterator& token,
    MeshInfo&            loadedInfo,
    const Tokenizer&     tokens,
    const ObjMaterial&   currentMaterial,
    const LoadSettings&  settings)
{
    uint vid = m.addVertex();
    for (uint i = 0; i < 3; ++i) {
        m.vertex(vid).position()[i] = io::readDouble<double>(token);
    }
    if constexpr (HasPerVertexColor<MeshType>) {
        if (vid == 0) {
            // if the current material has a valid color, or the file stores the
            // vertex color in the non-standard way (color values after the
            // positions)
            if (currentMaterial.hasColor || tokens.size() > 6) {
                if (settings.enableOptionalComponents) {
                    enableIfPerVertexColorOptional(m);
                    loadedInfo.setPerVertexColor();
                }
                else {
                    if (isPerVertexColorAvailable(m))
                        loadedInfo.setPerVertexColor();
                }
            }
        }
        if (loadedInfo.hasPerVertexColor()) {
            // the file has the nonstandard way to store vertex colors, after
            // the positions...
            if (tokens.size() > 6) {
                m.vertex(vid).color().setRedF(io::readFloat<float>(token));
                m.vertex(vid).color().setGreenF(io::readFloat<float>(token));
                m.vertex(vid).color().setBlueF(io::readFloat<float>(token));
            }
            else if (currentMaterial.hasColor) {
                m.vertex(vid).color() = currentMaterial.color();
            }
        }
    }
}

template<MeshConcept MeshType>
void readObjVertexNormal(
    MeshType&                        m,
    detail::ObjNormalsMap<MeshType>& mapNormalsCache,
    uint                             vn,
    Tokenizer::iterator&             token,
    MeshInfo&                        loadedInfo,
    const LoadSettings&              settings)
{
    using NormalType = MeshType::VertexType::NormalType;

    // first, need to check if I can store normals in the mesh
    if (vn == 0) {
        if (settings.enableOptionalComponents) {
            enableIfPerVertexNormalOptional(m);
            loadedInfo.setPerVertexNormal();
        }
        else {
            if (isPerVertexNormalAvailable(m))
                loadedInfo.setPerVertexNormal();
        }
    }
    if (loadedInfo.hasPerVertexNormal()) {
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
void readObjFace(
    MeshType&                            m,
    MeshInfo&                            loadedInfo,
    const Tokenizer&                     tokens,
    const std::vector<TexCoordIndexedd>& wedgeTexCoords,
    const ObjMaterial&                   currentMaterial,
    const LoadSettings&                  settings)
{
    using FaceType = MeshType::FaceType;

    std::vector<uint> vids;
    std::vector<uint> wids;

    loadedInfo.updateMeshType(tokens.size() - 1);

    // actual read - load vertex indices and texcoords indices, if present
    Tokenizer::iterator token = tokens.begin();
    ++token;
    vids.resize(tokens.size() - 1);
    wids.reserve(tokens.size() - 1);
    for (uint i = 0; i < tokens.size() - 1; ++i) {
        Tokenizer subt(*token, '/', false);
        auto      t = subt.begin();
        vids[i]     = io::readUInt<uint>(t) - 1;
        if (subt.size() > 1) {
            if (!t->empty()) {
                wids.push_back(io::readUInt<uint>(t) - 1);
            }
        }
        ++token;
    }

    // add the face
    uint      fid = m.addFace();
    FaceType& f   = m.face(fid);

    // check if we need to split the face we read into triangles
    bool splitFace = false;
    // we have a polygonal mesh, no need to split
    if constexpr (FaceType::VERTEX_NUMBER < 0) {
        // need to resize to the right number of verts
        f.resizeVertices(tokens.size() - 1);
    }
    else if (FaceType::VERTEX_NUMBER != tokens.size() - 1) {
        // we have faces with static sizes (triangles), but we are loading faces
        // with number of verts > 3. Need to split the face we are loading in n
        // faces!
        splitFace = true;
    }

    // create the face in the mesh, for now we manage only vertex indices
    if (!splitFace) { // no need to split face case
        for (uint i = 0; i < vids.size(); ++i) {
            if (vids[i] >= m.vertexNumber()) {
                throw MalformedFileException(
                    "Bad vertex index for face " + std::to_string(fid));
            }
            f.setVertex(i, vids[i]);
        }
    }
    else { // split needed
        addTriangleFacesFromPolygon(m, f, vids);
    }

    // color
    if (HasPerFaceColor<MeshType>) {
        // if the first face, we need to check if I can store colors
        if (fid == 0) {
            // if the current material has no color, we assume that the file has
            // no face color
            if (currentMaterial.hasColor) {
                if (settings.enableOptionalComponents) {
                    enableIfPerFaceColorOptional(m);
                    loadedInfo.setPerFaceColor();
                }
                else {
                    if (isPerFaceColorAvailable(m))
                        loadedInfo.setPerFaceColor();
                }
            }
        }
        if (loadedInfo.hasPerFaceColor()) {
            if (currentMaterial.hasColor) {
                // in case the loaded polygon has been triangulated in the last
                // n triangles of mesh
                for (uint ff = fid; ff < m.faceNumber(); ++ff) {
                    m.face(ff).color() = currentMaterial.color();
                }
            }
        }
    }

    // wedge texcoords
    if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
        // first, need to check if I can store wedge texcoords in the mesh
        if (fid == 0) {
            // if the current face has the right number of wedge texcoords, we
            // assume that we can load wedge texcoords
            if (wids.size() == vids.size()) {
                if (settings.enableOptionalComponents) {
                    enableIfPerFaceWedgeTexCoordsOptional(m);
                    loadedInfo.setPerFaceWedgeTexCoords();
                }
                else {
                    if (isPerFaceWedgeTexCoordsAvailable(m))
                        loadedInfo.setPerFaceWedgeTexCoords();
                }
            }
        }
        if (loadedInfo.hasPerFaceWedgeTexCoords()) {
            if (wids.size() == vids.size()) {
                if (!splitFace) { // there wasn't a triangulation of the face
                    // it is safe to assign each wedge texcoord to its position
                    // in the face
                    for (uint i = 0; i < wids.size(); ++i) {
                        if (wids[i] >= wedgeTexCoords.size()) {
                            throw MalformedFileException(
                                "Bad texcoord index for face " +
                                std::to_string(fid));
                        }
                        f.wedgeTexCoord(i) =
                            ((vcl::TexCoordd) wedgeTexCoords[wids[i]])
                                .cast<typename FaceType::WedgeTexCoordType::
                                          ScalarType>();
                        if (currentMaterial.hasTexture) {
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
                        for (uint i = 0; i < f.vertexNumber(); ++i) {
                            uint vid = m.index(f.vertex(i));
                            // find the position of the vertex in the vids array
                            auto it = std::find(vids.begin(), vids.end(), vid);
                            assert(it != vids.end());
                            uint pos = it - vids.begin();
                            // check that the texcoord id is valid
                            if (wids[pos] >= wedgeTexCoords.size()) {
                                throw MalformedFileException(
                                    "Bad texcoord index for face " +
                                    std::to_string(fid));
                            }
                            // set the wedge texcoord in the same position of
                            // the vertex
                            f.wedgeTexCoord(i) =
                                ((vcl::TexCoordd) wedgeTexCoords[wids[pos]])
                                    .cast<typename FaceType::WedgeTexCoordType::
                                              ScalarType>();
                            if (currentMaterial.hasTexture) {
                                f.textureIndex() = currentMaterial.mapId;
                            }
                        }
                    }
                }
            }
        }
    }
}

template<EdgeMeshConcept MeshType>
void readObjEdge(
    MeshType&           m,
    MeshInfo&           loadedInfo,
    const Tokenizer&    tokens,
    const ObjMaterial&  currentMaterial,
    const LoadSettings& settings)
{
    using EdgeType = MeshType::EdgeType;

    // add the edge
    uint      eid = m.addEdge();
    EdgeType& e   = m.edge(eid);

    // actual read - load vertex indices
    Tokenizer::iterator token = tokens.begin();
    ++token;
    uint vid1 = io::readUInt<uint>(token) - 1;
    uint vid2 = io::readUInt<uint>(token) - 1;
    e.setVertices(vid1, vid2);

    // color
    if (HasPerEdgeColor<MeshType>) {
        // if the first edge, we need to check if I can store colors
        if (eid == 0) {
            // if the current material has no color, we assume that the file has
            // no edge color
            if (currentMaterial.hasColor) {
                if (settings.enableOptionalComponents) {
                    enableIfPerEdgeColorOptional(m);
                    loadedInfo.setPerEdgeColor();
                }
                else {
                    if (isPerEdgeColorAvailable(m))
                        loadedInfo.setPerEdgeColor();
                }
            }
        }
        if (loadedInfo.hasPerEdgeColor()) {
            if (currentMaterial.hasColor) {
                // set the current color to the edge
                m.edge(eid).color() = currentMaterial.color();
            }
        }
    }
}

/**
 * @brief Actual implementation of loading an obj from a stream or a file.
 *
 * @param[in] m: The mesh to fill with the data read from the file.
 * @param[in] inputObjStream: The stream from which to read the obj file.
 * @param[in] inputMtlStreams: A vector of streams from which to read the mtl
 * files. It is used if the list of material files are known before reading the
 * obj file.
 * @param[out] loadedInfo: The info about the loaded mesh.
 * @param[in] filename: The name of the file from which the obj is read. It is
 * used to find the material files if they are not known before reading the obj
 * file.
 * @param[in] ignoreMtlLib: If true, the material files are not loaded when a
 * `mtllib` keyword is read in the obj file. It is used when the material files
 * are known before reading the obj file, and they are passed trough the
 * inputMtlStreams parameter.
 * @param[in] log: The logger to use to log messages.
 * @param[in] settings: settings for loading the file/stream.
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadObj(
    MeshType&                         m,
    std::istream&                     inputObjStream,
    const std::vector<std::istream*>& inputMtlStreams,
    MeshInfo&                         loadedInfo,
    const std::string&                filename     = "",
    bool                              ignoreMtlLib = false,
    LogType&                          log          = nullLogger,
    const LoadSettings&               settings     = LoadSettings())
{
    loadedInfo.clear();

    // save normals if they can't be stored directly into vertices
    detail::ObjNormalsMap<MeshType> mapNormalsCache;
    uint                            vn = 0; // number of vertex normals read
    // save array of texcoords, that are stored later (into wedges when loading
    // faces or into vertices as a fallback)
    std::vector<TexCoordIndexedd> texCoords;

    // map of materials loaded
    std::map<std::string, detail::ObjMaterial> materialMap;

    // load materials from the material files, if any
    for (auto* stream : inputMtlStreams) {
        detail::loadObjMaterials(materialMap, m, *stream, loadedInfo, settings);
    }

    // the current material, set by 'usemtl'
    detail::ObjMaterial currentMaterial;

    if constexpr (HasTexturePaths<MeshType>) {
        m.meshBasePath() = FileInfo::pathWithoutFileName(filename);
    }

    if constexpr (HasName<MeshType>) {
        m.name() = FileInfo::fileNameWithoutExtension(filename);
    }

    inputObjStream.seekg(0, inputObjStream.end);
    std::size_t fsize = inputObjStream.tellg();
    inputObjStream.seekg(0, inputObjStream.beg);
    log.startProgress("Loading OBJ file", fsize);

    // cycle that reads line by line
    do {
        Tokenizer tokens =
            readAndTokenizeNextNonEmptyLineNoThrow(inputObjStream);
        if (inputObjStream) {
            Tokenizer::iterator token  = tokens.begin();
            std::string         header = *token++;
            if (header == "mtllib" && !ignoreMtlLib) { // material file
                // we load the material file if they are not ignored
                std::string mtlfile =
                    FileInfo::pathWithoutFileName(filename) + *token;
                try {
                    detail::loadObjMaterials(
                        materialMap, m, mtlfile, loadedInfo, settings);
                }
                catch (CannotOpenFileException) {
                    log.log(
                        "Cannot open material file " + mtlfile,
                        LogType::WARNING_LOG);
                }
            }
            // use a new material - change currentMaterial
            if (header == "usemtl") {
                std::string matname = *token;
                auto        it      = materialMap.find(matname);
                if (it != materialMap.end()) {
                    currentMaterial = it->second;
                }
                else { // material not found - warning
                    log.log(
                        "Material " + matname + " not found.",
                        LogType::WARNING_LOG);
                }
            }
            // read vertex (and for some non-standard obj files, also vertex
            // color)
            if (header == "v") {
                loadedInfo.setVertices();
                loadedInfo.setPerVertexPosition();
                detail::readObjVertex(
                    m, token, loadedInfo, tokens, currentMaterial, settings);
            }
            // read vertex normal (and save in vn how many normals we read)
            if (header == "vn") {
                loadedInfo.setPerVertexNormal();
                if constexpr (HasPerVertexNormal<MeshType>) {
                    detail::readObjVertexNormal(
                        m, mapNormalsCache, vn, token, loadedInfo, settings);
                    vn++;
                }
            }
            // read texcoords and save them in the vector of texcoords, we will
            // store them in the mesh later
            if constexpr (
                HasPerVertexTexCoord<MeshType> ||
                HasPerFaceWedgeTexCoords<MeshType>) {
                if (header == "vt") {
                    // save the texcoord for later
                    TexCoordIndexedd tf;
                    for (uint i = 0; i < 2; ++i) {
                        tf[i] = io::readDouble<double>(token);
                    }
                    if (currentMaterial.hasTexture) {
                        tf.index() = currentMaterial.mapId;
                    }
                    texCoords.push_back(tf);
                }
            }
            // read faces and manage:
            // - color
            // - eventual texcoords
            // - possibility to split polygonal face into several triangles
            if (header == "f") {
                loadedInfo.setFaces();
                loadedInfo.setPerFaceVertexReferences();
                if constexpr (HasFaces<MeshType>) {
                    detail::readObjFace(
                        m,
                        loadedInfo,
                        tokens,
                        texCoords,
                        currentMaterial,
                        settings);
                }
            }
            // read edges and manage their color
            if (header == "l") {
                loadedInfo.setEdges();
                loadedInfo.setPerEdgeVertexReferences();
                if constexpr (HasEdges<MeshType>) {
                    detail::readObjEdge(
                        m, loadedInfo, tokens, currentMaterial, settings);
                }
            }
            log.progress(inputObjStream.tellg());
        }
    } while (inputObjStream);

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
        if (!loadedInfo.hasPerFaceWedgeTexCoords()) {
            // we can set the loaded texCoords to vertices, also if they are not
            // supported in obj
            if (texCoords.size() == m.vertexNumber()) {
                if (settings.enableOptionalComponents) {
                    enableIfPerVertexTexCoordOptional(m);
                    loadedInfo.setPerVertexTexCoord();
                }
                else {
                    if (isPerVertexTexCoordAvailable(m))
                        loadedInfo.setPerVertexTexCoord();
                }
                if (loadedInfo.hasPerVertexTexCoord()) {
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

    if constexpr (HasTextureImages<MeshType>) {
        if (settings.loadTextureImages) {
            for (Texture& texture : m.textures()) {
                texture.image() = loadImage(m.meshBasePath() + texture.path());
                if (texture.image().isNull()) {
                    log.log(
                        "Cannot load texture " + texture.path(),
                        LogType::WARNING_LOG);
                }
            }
        }
    }

    log.endProgress();
}

} // namespace detail

/**
 * @brief Loads from the given input obj stream and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * The info about what elements and components have been loaded from the stream
 * will be stored into the loadedInfo argument.
 *
 * In case of materials used in the obj stream that were not found in the
 * material streams, a warning will be logged in the log argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputObjStream: the stream to read from
 * @param[in] inputMtlStreams: the streams to read the material from (if any)
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadObj(
    MeshType&                         m,
    std::istream&                     inputObjStream,
    const std::vector<std::istream*>& inputMtlStreams,
    MeshInfo&                         loadedInfo,
    LogType&                          log      = nullLogger,
    const LoadSettings&               settings = LoadSettings())
{
    detail::loadObj(
        m,
        inputObjStream,
        inputMtlStreams,
        loadedInfo,
        "",
        true,
        log,
        settings);
}

/**
 * @brief Loads from the given input obj stream and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * In case of materials used in the obj stream that were not found in the
 * material streams, a warning will be logged in the log argument.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputObjStream: the stream to read from
 * @param[in] inputMtlStreams: the streams to read the material from (if any)
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadObj(
    MeshType&                         m,
    std::istream&                     inputObjStream,
    const std::vector<std::istream*>& inputMtlStreams,
    LogType&                          log      = nullLogger,
    const LoadSettings&               settings = LoadSettings())
{
    MeshInfo loadedInfo;
    detail::loadObj(
        m,
        inputObjStream,
        inputMtlStreams,
        loadedInfo,
        "",
        true,
        log,
        settings);
}

/**
 * @brief Loads from the given input obj stream and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * The info about what elements and components have been loaded from the stream
 * will be stored into the loadedInfo argument.
 *
 * In case of materials used in the obj stream that were not found in the
 * material streams, a warning will be logged in the log argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] inputObjStream: the stream to read from
 * @param[in] inputMtlStreams: the streams to read the material from (if any)
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadObj(
    std::istream&                     inputObjStream,
    const std::vector<std::istream*>& inputMtlStreams,
    MeshInfo&                         loadedInfo,
    LogType&                          log      = nullLogger,
    const LoadSettings&               settings = LoadSettings())
{
    MeshType m;
    loadObj(m, inputObjStream, inputMtlStreams, loadedInfo, log, settings);
    return m;
}

/**
 * @brief Loads from the given input obj stream and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * In case of materials used in the obj stream that were not found in the
 * material streams, a warning will be logged in the log argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] inputObjStream: the stream to read from
 * @param[in] inputMtlStreams: the streams to read the material from (if any)
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadObj(
    std::istream&                     inputObjStream,
    const std::vector<std::istream*>& inputMtlStreams,
    LogType&                          log      = nullLogger,
    const LoadSettings&               settings = LoadSettings())
{
    MeshType m;
    loadObj(m, inputObjStream, inputMtlStreams, log, settings);
    return m;
}

/**
 * @brief Loads from the given input obj file and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * The info about what elements and components have been loaded from the stream
 * will be stored into the loadedInfo argument.
 *
 * In case of materials used in the obj file that were not found in the
 * material files, a warning will be logged in the log argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the file
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadObj(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    std::ifstream file = openInputFileStream(filename);

    // some obj files do not declare the material file name with mtllib, but
    // they assume that material file has the same name of the obj file.
    // Therefore, we first load this file if it exists.
    std::string stdmtlfile = FileInfo::pathWithoutFileName(filename) +
                             FileInfo::fileNameWithoutExtension(filename) +
                             ".mtl";

    std::ifstream              f;
    std::vector<std::istream*> mtlStreams;
    try {
        f = openInputFileStream(stdmtlfile);
        mtlStreams.push_back(&f);
    }
    catch (CannotOpenFileException) {
        // nothing to do, this file was missing, but this was a fallback for
        // some type of files...
    }

    detail::loadObj(
        m, file, mtlStreams, loadedInfo, filename, false, log, settings);
}

/**
 * @brief Loads from the given input obj file and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * In case of materials used in the obj file that were not found in the
 * material files, a warning will be logged in the log argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the file to read from
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadObj(
    MeshType&           m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    loadObj(m, filename, loadedInfo, log, settings);
}

/**
 * @brief Loads from the given input obj file and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * The info about what elements and components have been loaded from the stream
 * will be stored into the loadedInfo argument.
 *
 * In case of materials used in the obj file that were not found in the
 * material files, a warning will be logged in the log argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the file
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadObj(
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshType m;
    loadObj(m, filename, loadedInfo, log, settings);
    return m;
}

/**
 * @brief Loads from the given input obj file and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * In case of materials used in the obj file that were not found in the
 * material files, a warning will be logged in the log argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: the file to read from
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadObj(
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    return loadObj<MeshType>(filename, loadedInfo, log, settings);
}

} // namespace vcl

#endif // VCL_IO_MESH_OBJ_LOAD_H
