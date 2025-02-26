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

#ifndef VCL_PROCESSING_ENGINE_SETTINGS_H
#define VCL_PROCESSING_ENGINE_SETTINGS_H

#include <vclib/meshes.h>

namespace vcl::proc {

/**
 * @brief Scalar type used in the processing module.
 */
using ScalarType = double;

/**
 * @brief Flag that indicates if the meshes are indexed.
 */
constexpr bool INDEXED_MESHES = false;

enum class MeshTypeId : vcl::uint
{
    TRIANGLE_MESH = 0,
    POLYGON_MESH = 1,

    COUNT
};

/**
 * @brief List of supported mesh types supported by the processing module.
 */
using MeshTypes = TypeWrapper<
    vcl::TriEdgeMeshT<ScalarType, INDEXED_MESHES>,
    vcl::PolyEdgeMeshT<ScalarType, INDEXED_MESHES>>;

template<MeshTypeId MESH_ID>
using GetMeshType = TypeAt<toUnderlying(MESH_ID), MeshTypes>::type;

template<typename MeshType>
constexpr MeshTypeId meshTypeId()
{
    constexpr uint id = IndexInTypes<MeshType, MeshTypes>::value;
    if constexpr (id == UINT_NULL) {
        return MeshTypeId::COUNT;
    }
    return static_cast<MeshTypeId>(id);
}

template<typename MeshType>
constexpr void checkMeshTypeId()
{
    constexpr uint id = toUnderlying(meshTypeId<MeshType>());
    static_assert(id != UINT_NULL, "Mesh type not supported.");
    static_assert(
        id >= 0 && id < toUnderlying(MeshTypeId::COUNT),
        "Invalid mesh type id.");
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_ENGINE_SETTINGS_H
