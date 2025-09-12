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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_BUFFER_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_BUFFER_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

/**
 * @defgroup import_buffer Import Mesh from Buffer Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Import Mesh from Buffer algorithms.
 *
 * They allow to import mesh data from buffers.
 */

namespace vcl {

/**
 * @brief Sets the vertex positions of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of scalars.
 * 
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the vertex container of the mesh and then adds a number of vertices
 * that depends on the given `vertexNumber`. In this scenario, all the old
 * vertices with their components stored in the mesh before calling this
 * function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the given `vertexNumber` is equal to the number of vertices of the mesh. If
 * this is not the case, an exception is thrown. Then, the function sets the
 * positions of the vertices of the mesh from the input buffer. In this
 * scenario, all the components (except the positions) of the vertices stored in
 * the mesh before calling this function are preserved.
 *
 * All the other containers of the mesh are left as they are. Pointers to
 * vertices stored in the other containers of the mesh are still valid only if
 * the number of vertices is not changed (same allocation policy of the
 * std::vector).
 *
 * @throws vcl::WrongSizeException if `clearBeforeSet` is false and
 * `vertexNumber` != mesh.vertexNumber().
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 *
 * @param[in/out] mesh: the mesh on which import the input vertices.
 * @param[in] buffer: a contiguous array containing the positions of the
 * vertices of the mesh.
 * @param[in] vertexNumber: the number of vertices contained in the input
 * buffer.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the vertices of the mesh before adding the vertices from the input buffer.
 * If `false`, the function sets the positions in the input buffer to the
 * vertices of the mesh, leaving all the other components untouched.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `vertexNumber`.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexPositionsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              vertexNumber,
    bool              clearBeforeSet = true,
    MatrixStorageType storage        = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber      = UINT_NULL)
{
    const uint ROW_NUM = rowNumber == UINT_NULL ? vertexNumber : rowNumber;

    if (clearBeforeSet) {
        mesh.clearVertices();
        mesh.resizeVertices(vertexNumber);
    }
    else {
        if (vertexNumber != mesh.vertexNumber()) {
            throw WrongSizeException(
                "The input vertex number does not match the number of vertices "
                "of the mesh\n"
                "Number of vertices in the mesh: " +
                std::to_string(mesh.vertexNumber()) +
                "\nNumber of input vertex number: " +
                std::to_string(vertexNumber));
        }
    }

    for (uint i = 0; auto& p : mesh.vertices() | views::positions) {
        if (storage == MatrixStorageType::ROW_MAJOR) {
            p.x() = buffer[i * 3 + 0];
            p.y() = buffer[i * 3 + 1];
            p.z() = buffer[i * 3 + 2];
        }
        else {
            p.x() = buffer[0 * ROW_NUM + i];
            p.y() = buffer[1 * ROW_NUM + i];
            p.z() = buffer[2 * ROW_NUM + i];
        }
        ++i;
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_BUFFER_H
