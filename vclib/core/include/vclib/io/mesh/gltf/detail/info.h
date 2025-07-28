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

#ifndef VCL_IO_MESH_GLTF_DETAIL_INFO_H
#define VCL_IO_MESH_GLTF_DETAIL_INFO_H

#include <vclib/space/core.h>

#include <tiny_gltf.h>

namespace vcl::detail {

/**
 * @brief Recursive function that returns the number of meshes contained
 * in the current node (0 or 1) plus the number of meshes contained in the
 * children of the node.
 *
 * Call this function from a root node to know how many meshes are referred
 * in the scene.
 *
 * @param model
 * @param node
 * @return
 */
inline uint gltfNodeNumberMeshes(const tinygltf::Model& model, uint node)
{
    uint nMeshes = 0;
    if (model.nodes[node].mesh >= 0) {
        nMeshes = 1;
    }
    for (int c : model.nodes[node].children) {
        if (c >= 0) {
            nMeshes += gltfNodeNumberMeshes(model, c);
        }
    }
    return nMeshes;
}

/**
 * @brief returns the number of meshes referred by the nodes contained in the
 * gltf file.
 *
 * Note: this number may differ from model.meshes.size().
 * This is because some gltf file may duplicate a mesh in the scene, and place
 * it in different positions using the node hierarchy.
 *
 * This function actually returns how many (referenced) nodes contain a mesh.
 *
 * @param model
 * @return
 */
inline uint gltfNumberMeshes(const tinygltf::Model& model)
{
    uint nMeshes = 0;
    for (uint s = 0; s < model.scenes.size(); ++s) {
        const tinygltf::Scene& scene = model.scenes[s];
        for (uint n = 0; n < scene.nodes.size(); ++n) {
            nMeshes += gltfNodeNumberMeshes(model, scene.nodes[n]);
        }
    }
    return nMeshes;
}

/**
 * @brief Gets the 4x4 transformation matrix contained in the node itself,
 * without taking into account parent transformations of the node.
 *
 * @param model
 * @param currentNode
 * @return
 */
template<Matrix44Concept MatrixType>
MatrixType gltfCurrentNodeMatrix(const tinygltf::Model& model, uint currentNode)
{
    using QuatType = Quaternion<typename MatrixType::Scalar>;

    MatrixType currentMatrix = MatrixType::Identity();
    // if the current node contains a 4x4 matrix
    if (model.nodes[currentNode].matrix.size() == 16) {
        for (uint i = 0; i < 4; ++i) {
            for (uint j = 0; j < 4; ++j) {
                // set the current matrix element
                currentMatrix(i, j) =
                    model.nodes[currentNode].matrix[j * 4 + i];
            }
        }
    }
    // if the current node contains rotation quaternion, scale vector or
    //  translation vector
    else {
        // note: if one or more of these are missing, identity is used.
        // note: final matrix is computed as M = T * R * S, as specified by
        // gltf docs:
        // https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_004_ScenesNodes.md
        // 4x4 matrices associated to rotation, translation and scale
        MatrixType rot   = MatrixType::Identity();
        MatrixType scale = MatrixType::Identity();
        MatrixType trans = MatrixType::Identity();

        // if node contains rotation quaternion
        if (model.nodes[currentNode].rotation.size() == 4) {
            QuatType qr(
                model.nodes[currentNode].rotation[3],
                model.nodes[currentNode].rotation[0],
                model.nodes[currentNode].rotation[1],
                model.nodes[currentNode].rotation[2]);
            // Convert quaternion to 3x3 rotation matrix and insert into
            // top-left of 4x4 matrix
            rot.template block<3, 3>(0, 0) = qr.normalized().toRotationMatrix();
        }
        // if node contains scale
        if (model.nodes[currentNode].scale.size() == 3) {
            // set 4x4 matrix scale
            scale(0, 0) = model.nodes[currentNode].scale[0];
            scale(1, 1) = model.nodes[currentNode].scale[1];
            scale(2, 2) = model.nodes[currentNode].scale[2];
        }
        // if node contains translation
        if (model.nodes[currentNode].translation.size() == 3) {
            // set 4x4 matrix translation
            trans(0, 3) = model.nodes[currentNode].translation[0];
            trans(1, 3) = model.nodes[currentNode].translation[1];
            trans(2, 3) = model.nodes[currentNode].translation[2];
        }

        // M = T * R * S
        currentMatrix = trans * rot * scale;
    }
    return currentMatrix;
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_GLTF_DETAIL_INFO_H
