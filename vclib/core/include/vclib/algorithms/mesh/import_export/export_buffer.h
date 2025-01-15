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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H

#include <vclib/mesh/requirements.h>
#include <vclib/views/mesh.h>

namespace vcl {

/**
 * @brief Export the vertex coordinates of a mesh to a buffer.
 *
 * This function exports the vertex coordinates of a mesh to a buffer. The
 * buffer must be preallocated with the correct size (number of vertices times
 * the number of coordinates per vertex).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param mesh
 * @param buffer
 * @param storage
 */
template<MeshConcept MeshType>
void vertexCoordsToBuffer(
    const MeshType&         mesh,
    auto*                   buffer,
    MatrixStorageType::Enum storage = MatrixStorageType::ROW_MAJOR)
{
    if (storage == MatrixStorageType::ROW_MAJOR) {
        uint i = 0;
        for (const auto& c : mesh.vertices() | views::coords) {
            buffer[i * 3 + 0] = c.x();
            buffer[i * 3 + 1] = c.y();
            buffer[i * 3 + 2] = c.z();
            ++i;
        }
    }
    else {
        uint i = 0;
        for (const auto& c : mesh.vertices() | views::coords) {
            buffer[0 * mesh.vertexNumber() + i] = c.x();
            buffer[1 * mesh.vertexNumber() + i] = c.y();
            buffer[2 * mesh.vertexNumber() + i] = c.z();
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each triangle of a Mesh.
 *
 * This function exports the vertex indices of the triangles of a mesh to a
 * buffer. The buffer must be preallocated with the correct size (number of
 * faces times 3). The function assumes that the input mesh is a triangle mesh
 * (if there are polygonal faces, only the first three vertices are considered).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param mesh
 * @param buffer
 * @param storage
 */
template<MeshConcept MeshType>
void trianglesToBuffer(
    const MeshType&         mesh,
    auto*                   buffer,
    MatrixStorageType::Enum storage = MatrixStorageType::ROW_MAJOR)
{
    if (storage == MatrixStorageType::ROW_MAJOR) {
        uint i = 0;
        for (const auto& f : mesh.faces()) {
            buffer[i * 3 + 0] = f.vertexIndex(0);
            buffer[i * 3 + 1] = f.vertexIndex(1);
            buffer[i * 3 + 2] = f.vertexIndex(2);
            ++i;
        }
    }
    else {
        uint i = 0;
        for (const auto& f : mesh.faces()) {
            buffer[0 * mesh.faceNumber() + i] = f.vertexIndex(0);
            buffer[1 * mesh.faceNumber() + i] = f.vertexIndex(1);
            buffer[2 * mesh.faceNumber() + i] = f.vertexIndex(2);
            ++i;
        }
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
