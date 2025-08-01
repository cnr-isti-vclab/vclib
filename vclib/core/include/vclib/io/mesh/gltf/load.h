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

#ifndef VCL_IO_MESH_GLTF_LOAD_H
#define VCL_IO_MESH_GLTF_LOAD_H

#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>

#include "detail/info.h"
#include "detail/load_mesh.h"

namespace vcl {

namespace detail {

template<Matrix44Concept MatrixType, LoggerConcept LogType>
void loadGltfMeshesWhileTraversingNodes(
    const tinygltf::Model& model,
    auto&                  currentMeshIt,
    auto&                  currentInfoIt,
    MatrixType             currentMatrix,
    uint                   currentNode,
    const LoadSettings&    settings,
    LogType&               log)
{
    currentMatrix =
        currentMatrix * gltfCurrentNodeMatrix<MatrixType>(model, currentNode);

    if (model.nodes[currentNode].mesh >= 0) {
        int meshid = model.nodes[currentNode].mesh;
        gltfLoadMesh(
            *currentMeshIt,
            *currentInfoIt,
            model.meshes[meshid],
            model,
            currentMatrix,
            settings,
            log);

        ++currentMeshIt;
        ++currentInfoIt;
    }
    // for each child
    for (int c : model.nodes[currentNode].children) {
        if (c >= 0) { // if it is valid
            // visit child
            loadGltfMeshesWhileTraversingNodes(
                model,
                currentMeshIt,
                currentInfoIt,
                currentMatrix,
                c,
                settings,
                log);
        }
    }
}

template<MeshConcept MeshType, LoggerConcept LogType>
void loadGltf(
    tinygltf::Model&       model,
    std::vector<MeshType>& meshes,
    std::vector<MeshInfo>& infos,
    const LoadSettings&    settings = LoadSettings(),
    LogType&               log      = nullLogger)
{
    using ScalarType = MeshType::ScalarType;

    using Matrix44m = std::conditional_t<
        HasTransformMatrix<MeshType>,
        typename MeshType::TransformMatrixType,
        Matrix44<ScalarType>>;

    uint nMeshes = gltfNumberMeshes(model);

    Matrix44m identityMatrix = Matrix44m::Identity();

    meshes.resize(nMeshes);
    infos.resize(nMeshes);

    log.startProgress("Reading meshes", nMeshes);

    auto mit = meshes.begin();
    auto iit = infos.begin();

    for (unsigned int s = 0; s < model.scenes.size(); ++s) {
        const tinygltf::Scene& scene = model.scenes[s];
        for (unsigned int n = 0; n < scene.nodes.size(); ++n) {
            loadGltfMeshesWhileTraversingNodes(
                model, mit, iit, identityMatrix, scene.nodes[n], settings, log);

            log.progress(s * scene.nodes.size() + n);
        }
    }

    log.endProgress();
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadGltf(
    std::vector<MeshType>& m,
    const std::string&     filename,
    std::vector<MeshInfo>& loadedInfo,
    const LoadSettings&    settings = LoadSettings(),
    LogType&               log      = nullLogger)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model    model;

    std::string err;
    std::string warn;

    // Determine the file format from the filename extension
    std::string ext = toLower(FileInfo::extension(filename));

    bool ret = false;

    if (ext == ".gltf") {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    }
    else if (ext == ".glb") {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    }
    else {
        throw UnknownFileFormatException(ext);
    }

    if (!ret) {
        throw CannotOpenFileException(
            "Failed to load glTF file: " + filename + "\n" + err);
    }

    if (!warn.empty()) {
        log.log(
            "Warnings while loading glTF file: " + warn,
            LogType::LogLevel::WARNING_LOG);
    }

    detail::loadGltf(model, m, loadedInfo, settings, log);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadGltf(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings = LoadSettings(),
    LogType&            log      = nullLogger)
{
    std::vector<MeshType> meshes;
    std::vector<MeshInfo> infos;
    loadGltf(meshes, filename, infos, settings, log);

    m          = std::move(meshes.front());
    loadedInfo = std::move(infos.front());
    for (std::size_t i = 1; i < meshes.size(); ++i) {
        m.append(std::move(meshes[i]));
        // todo manage infos
    }
}

} // namespace vcl

#endif // VCL_IO_MESH_GLTF_LOAD_H
