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

#ifndef VCL_IO_MESH_OFF_LOAD_H
#define VCL_IO_MESH_OFF_LOAD_H

#include <vclib/algorithms/mesh/face_topology.h>
#include <vclib/exceptions/io.h>
#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/io/read.h>
#include <vclib/misc/logger.h>
#include <vclib/misc/tokenizer.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl {

namespace detail {

constexpr float OFF_GEOMVIEW_COLOR_MAP[148][4] = {
    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },

    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.2f,  0.2f,  0.2f,  0.2f },

    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.1f,  0.1f,  0.1f,  0.1f },
    {0.1f,  0.1f,  0.1f,  0.1f },
    {0.8f,  0.8f,  0.8f,  0.8f },

    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.9f,  0.9f,  0.9f,  0.9f },

    {0.2f,  0.2f,  0.2f,  0.2f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.75f, 0.75f, 0.75f, 0.75f},
    {0.8f,  0.8f,  0.8f,  0.8f },

    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.8f,  0.8f,  0.8f,  0.8f },

    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },

    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.75f, 0.75f, 0.75f, 0.75f},
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.4f,  0.4f,  0.4f,  0.4f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.8f,  0.8f,  0.8f,  0.8f },

    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.9f,  0.9f,  0.9f,  0.9f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.75f, 0.75f, 0.75f, 0.75f},
    {0.8f,  0.8f,  0.8f,  0.8f },

    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.4f,  0.4f,  0.4f,  0.4f },

    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.75f, 0.75f, 0.75f, 0.75f},

    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.8f,  0.8f,  0.8f,  0.8f },
    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },

    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },
    {1.0f,  1.0f,  1.0f,  1.0f },

    {0.05f, 0.05f, 0.05f, 0.05f},
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.2f,  0.2f,  0.2f,  0.2f },
    {0.9f,  0.9f,  0.9f,  0.9f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.1f,  0.1f,  0.1f,  0.1f },
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.7f,  0.7f,  0.7f,  0.7f },

    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.75f, 0.75f, 0.75f, 0.75f},

    {0.75f, 0.75f, 0.75f, 0.75f},
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.4f,  0.4f,  0.4f,  0.4f },

    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.7f,  0.7f,  0.7f,  0.7f },

    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.75f, 0.75f, 0.75f, 0.75f},

    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.7f,  0.7f,  0.7f,  0.7f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.75f, 0.75f, 0.75f, 0.75f},
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.7f,  0.7f,  0.7f,  0.7f },

    {0.7f,  0.7f,  0.7f,  0.7f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.9f,  0.9f,  0.9f,  0.9f },
    {0.0f,  0.0f,  0.0f,  0.0f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.75f, 0.75f, 0.75f, 0.75f},
    {0.8f,  0.8f,  0.8f,  0.8f },
    {0.4f,  0.4f,  0.4f,  0.4f },

    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  0.0f,  0.0f },
    {0.4f,  0.4f,  0.4f,  0.4f },
    {0.8f,  0.8f,  0.8f,  0.8f }
};

inline void readOffHeader(
    std::istream& file,
    MeshInfo&     fileInfo,
    uint&         nv,
    uint&         nf,
    uint&         ne)
{
    fileInfo.clear();
    Tokenizer           tokens = readAndTokenizeNextNonEmptyLine(file);
    Tokenizer::iterator token  = tokens.begin();
    std::string         header = *token;

    // the OFF string is in the header go on parsing it.
    if (header.rfind("OFF") != std::basic_string<char>::npos) {
        for (int u = header.rfind("OFF") - 1; u >= 0; u--) {
            if (header[u] == 'C')
                fileInfo.setPerVertexColor();
            else if (header[u] == 'N')
                fileInfo.setPerVertexNormal();
            else if (u > 0 && header[u - 1] == 'S' && header[u] == 'T')
                fileInfo.setPerVertexTexCoord();
            else if (header[u] == '4')
                throw MalformedFileException(
                    "Unsupported Homogeneus components in OFF.");
            else if (header[u] == 'n')
                throw MalformedFileException("Unsupported High Dimension OFF.");
        }
    }
    else
        throw MalformedFileException("Missing OFF header in file.");

    // If the file is slightly malformed and it has nvert and nface AFTER the
    // OFF string instead of in the next line we manage it here...
    if (tokens.size() == 1) {
        tokens = readAndTokenizeNextNonEmptyLine(file);
        token  = tokens.begin();
    }
    else
        ++token;

    nv = io::readUInt<uint>(token);
    nf = io::readUInt<uint>(token);
    ne = io::readUInt<uint>(token);
    if (nv > 0)
        fileInfo.setVertices();
    if (nf > 0)
        fileInfo.setFaces();
    // if (ne > 0)
    //    loadedInfo.setEdges();
}

inline Color readOffColor(Tokenizer::iterator& token, int nColorComponents)
{
    uint red, green, blue, alpha = 255;

    if (nColorComponents == 1) {
        uint k = io::readUInt<uint>(token);

        red   = OFF_GEOMVIEW_COLOR_MAP[k][0] * 255;
        green = OFF_GEOMVIEW_COLOR_MAP[k][1] * 255;
        blue  = OFF_GEOMVIEW_COLOR_MAP[k][2] * 255;
        alpha = OFF_GEOMVIEW_COLOR_MAP[k][3] * 255;
    }
    else {
        double r = io::readDouble<double>(token);
        double g = io::readDouble<double>(token);
        double b = io::readDouble<double>(token);
        double a = -1;
        if (nColorComponents == 4) {
            a = io::readDouble<double>(token);
        }
        if (r > 1 || g > 1 || b > 1) {
            red   = r;
            green = g;
            blue  = b;
            alpha = a != -1 ? a : alpha;
        }
        else {
            red   = r * 255;
            green = g * 255;
            blue  = b * 255;
            alpha = a != -1 ? a * 255 : alpha;
        }
    }
    return Color(red, green, blue, alpha);
}

template<MeshConcept MeshType, LoggerConcept LogType>
void readOffVertices(
    MeshType&       mesh,
    std::istream&   file,
    const MeshInfo& fileInfo,
    uint            nv,
    LogType&        log)
{
    using VertexType = MeshType::VertexType;

    const uint nTexCoords = fileInfo.hasPerVertexTexCoord() ? 2 : 0;

    log.startProgress("Reading vertices", nv);
    mesh.addVertices(nv);
    for (uint i = 0; i < nv; i++) {
        VertexType& v = mesh.vertex(i);

        Tokenizer           tokens = readAndTokenizeNextNonEmptyLine(file);
        Tokenizer::iterator token  = tokens.begin();

        // Read 3 vertex coordinates
        for (unsigned int j = 0; j < 3; j++) {
            // Read vertex coordinate
            v.coord()[j] = io::readDouble<double>(token);
        }

        if constexpr (HasPerVertexNormal<MeshType>) {
            if (isPerVertexNormalAvailable(mesh) &&
                fileInfo.hasPerVertexNormal()) {
                // Read 3 normal coordinates
                for (unsigned int j = 0; j < 3; j++) {
                    v.normal()[j] = io::readDouble<double>(token);
                }
            }
        }
        // need to read and throw away data
        else if (fileInfo.hasPerVertexNormal()) {
            for (unsigned int j = 0; j < 3; j++) {
                io::readDouble<double>(token);
            }
        }

        const uint nReadComponents = token - tokens.begin();
        const int  nColorComponents =
            (int) tokens.size() - nReadComponents - nTexCoords;

        if constexpr (HasPerVertexColor<MeshType>) {
            if (isPerVertexColorAvailable(mesh) &&
                fileInfo.hasPerVertexColor()) {
                if (nColorComponents != 1 && nColorComponents != 3 &&
                    nColorComponents != 4)
                    throw MalformedFileException(
                        "Wrong number of components in line.");
                v.color() = readOffColor(token, nColorComponents);
            }
        }
        // need to read and throw away data
        else if (fileInfo.hasPerVertexColor()) {
            if (nColorComponents != 1 && nColorComponents != 3 &&
                nColorComponents != 4)
                throw MalformedFileException(
                    "Wrong number of components in line.");
            readOffColor(token, nColorComponents);
        }

        if constexpr (HasPerVertexTexCoord<MeshType>) {
            if (isPerVertexTexCoordAvailable(mesh) &&
                fileInfo.hasPerVertexTexCoord()) {
                // Read 2 tex coordinates
                for (unsigned int j = 0; j < 2; j++) {
                    v.texCoord()[j] = io::readDouble<double>(token);
                }
            }
        }
        // need to read and throw away data
        else if (fileInfo.hasPerVertexTexCoord()) {
            for (unsigned int j = 0; j < 2; j++) {
                io::readDouble<double>(token);
            }
        }

        log.progress(i);
    }
    log.endProgress();
}

template<FaceMeshConcept MeshType, LoggerConcept LogType>
void readOffFaces(
    MeshType&           mesh,
    std::istream&       file,
    MeshInfo&           loadedInfo,
    uint                nf,
    const LoadSettings& settings,
    LogType&            log)
{
    if constexpr (HasFaces<MeshType>) {
        using FaceType = MeshType::FaceType;

        mesh.reserveFaces(nf);
        log.startProgress("Reading faces", nf);

        for (uint fid = 0; fid < nf; ++fid) {
            Tokenizer           tokens = readAndTokenizeNextNonEmptyLine(file);
            Tokenizer::iterator token  = tokens.begin();
            mesh.addFace();
            FaceType& f = mesh.face(mesh.faceNumber() - 1);

            // read vertex indices
            uint fSize = io::readUInt<uint>(token);
            loadedInfo.updateMeshType(fSize);
            // contains the vertex ids of the actual face
            std::vector<uint> vids;
            vids.resize(fSize);
            // read vertex indices
            for (uint i = 0; i < fSize; ++i) {
                vids[i] = io::readUInt<uint>(token);
            }

            // load vertex indices into face
            bool splitFace = false;
            // we have a polygonal mesh
            if constexpr (FaceType::VERTEX_NUMBER < 0) {
                // need to resize to the right number of verts
                f.resizeVertices(vids.size());
            }
            else if (FaceType::VERTEX_NUMBER != vids.size()) {
                // we have faces with static sizes (triangles), but we are
                // loading faces with number of verts > 3. Need to split the
                // face we are loading in n faces!
                splitFace = true;
            }
            if (!splitFace) { // classic load, no split needed
                for (uint i = 0; i < vids.size(); ++i) {
                    if (vids[i] >= mesh.vertexNumber()) {
                        throw MalformedFileException(
                            "Bad vertex index for face " + std::to_string(i));
                    }
                    f.setVertex(i, vids[i]);
                }
            }
            else { // split needed
                addTriangleFacesFromPolygon(mesh, f, vids);
            }

            // read face color
            if (token != tokens.end()) { // there are colors to read
                if constexpr (HasPerFaceColor<MeshType>) {
                    if (isPerFaceColorAvailable(mesh) ||
                        (settings.enableOptionalComponents &&
                         enableIfPerFaceColorOptional(mesh))) {
                        loadedInfo.setPerFaceColor();
                        f.color() = readOffColor(
                            token, tokens.size() - (token - tokens.begin()));
                        // in case the loaded polygon has been triangulated in
                        // the last n triangles
                        for (uint ff = mesh.index(f); ff < mesh.faceNumber();
                             ++ff) {
                            mesh.face(ff).color() = f.color();
                        }
                    }
                }
            }
            log.progress(fid);
        }
        log.endProgress();
    }
    else { // mesh does not have face, read nf lines and throw them away
        for (uint i = 0; i < nf; ++i)
            readAndTokenizeNextNonEmptyLine(file);
    }
}

} // namespace detail

/**
 * @brief Loads from the given input off stream and puts the content into the
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
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputOffStream: the stream to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadOff(
    MeshType&           m,
    std::istream&       inputOffStream,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    uint nVertices, nFaces, nEdges;

    MeshInfo fileInfo; // data that needs to be read from the file

    detail::readOffHeader(inputOffStream, fileInfo, nVertices, nFaces, nEdges);
    loadedInfo = fileInfo; // data that will be stored in the mesh!
    if (settings.enableOptionalComponents)
        enableOptionalComponentsFromInfo(loadedInfo, m);

    if (nVertices == 0) {
        log.log("The file has no vertices", LogType::WARNING_LOG);
        return;
    }

    int percVertices = nVertices / (nVertices + nFaces) * 100;
    int percFaces    = 100 - percVertices;

    log.startNewTask(0, percVertices, "Reading vertices");
    detail::readOffVertices(m, inputOffStream, fileInfo, nVertices, log);
    log.endTask("Reading vertices");
    if constexpr (HasFaces<MeshType>) {
        log.startNewTask(percVertices, 100, "Reading faces");
        detail::readOffFaces(
            m, inputOffStream, fileInfo, nFaces, settings, log);
        log.endTask("Reading faces");
    }
    else {
        log.log(100, "Ignored faces reading");
    }
    if (settings.enableOptionalComponents)
        loadedInfo = fileInfo;
}

/**
 * @brief Loads from the given input off stream and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputOffStream: the stream to read from
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadOff(
    MeshType&           m,
    std::istream&       inputOffStream,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    loadOff(m, inputOffStream, loadedInfo, log, settings);
}

/**
 * @brief Loads from the given input off stream and puts the content into the
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
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] inputOffStream: the stream to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadOff(
    std::istream&       inputOffStream,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshType m;
    loadOff(m, inputOffStream, loadedInfo, log, settings);
    return m;
}

/**
 * @brief Loads from the given input off stream and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] inputOffStream: the stream to read from
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadOff(
    std::istream&       inputOffStream,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    return loadOff<MeshType>(inputOffStream, loadedInfo, log, settings);
}

/**
 * @brief Loads from the given input off file and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * The info about what elements and components have been loaded from the file
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the name of the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the file
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadOff(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    std::ifstream file = openInputFileStream(filename);

    if constexpr (HasName<MeshType>) {
        m.name() = FileInfo::fileNameWithoutExtension(filename);
    }

    loadOff(m, file, loadedInfo, log, settings);
}

/**
 * @brief Loads from the given input off file and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the name of the file to read from
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadOff(
    MeshType&           m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    loadOff(m, filename, loadedInfo, log, settings);
}

/**
 * @brief Loads from the given input off file and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * The info about what elements and components have been loaded from the file
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: the name of the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the file
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadOff(
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshType m;
    loadOff(m, filename, loadedInfo, log, settings);
    return m;
}

/**
 * @brief Loads from the given input off file and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: the name of the file to read from
 * @param[in] log: the logger to use
 * @param[in] settings: settings for loading the file/stream.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadOff(
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    return loadOff<MeshType>(filename, loadedInfo, log, settings);
}

} // namespace vcl

#endif // VCL_IO_MESH_OFF_LOAD_H
