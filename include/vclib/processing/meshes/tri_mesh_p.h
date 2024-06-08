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

#ifndef VCL_PROCESSING_MESHES_TRI_MESH_P_H
#define VCL_PROCESSING_MESHES_TRI_MESH_P_H

#include "abstract_mesh.h"

#include <vclib/meshes/tri_mesh.h>
#include <vclib/processing/settings.h>

namespace vcl::proc {

class TriMeshP :
        public AbstractMesh,
        public TriMeshT<ProcScalarType, INDEXED_MESHES>
{
public:
    TriMeshP() = default;

    std::shared_ptr<AbstractMesh> clone() const override
    {
        return std::make_shared<TriMeshP>(*this);
    }

    AbsMeshType::Enum type() const override
    {
        return AbsMeshType::TRIANGLE_MESH;
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_MESHES_TRI_MESH_P_H
