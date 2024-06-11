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

#ifndef VCL_PROCESSING_MESHES_TRI_MESH_H
#define VCL_PROCESSING_MESHES_TRI_MESH_H

#include "mesh_i.h"

#include <vclib/meshes/tri_mesh.h>
#include <vclib/processing/settings.h>

namespace vcl::proc {

class TriMesh final :
        public MeshI,
        public TriMeshT<ProcScalarType, INDEXED_MESHES>
{
public:
    TriMesh() = default;

    std::shared_ptr<MeshI> clone() const override
    {
        return std::make_shared<TriMesh>(*this);
    }

    MeshIType::Enum type() const override { return MeshIType::TRI_MESH; }

    std::string typeName() const override { return "TriMesh"; }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_MESHES_TRI_MESH_H
