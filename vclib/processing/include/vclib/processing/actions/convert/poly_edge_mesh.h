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

#ifndef VCL_PROCESSING_ACTIONS_CONVERT_POLY_EDGE_MESH_H
#define VCL_PROCESSING_ACTIONS_CONVERT_POLY_EDGE_MESH_H

#include <vclib/processing/engine.h>

namespace vcl::proc {

template<MeshConcept MeshType>

class PolyEdgeMeshConvert : public ConvertActionT<MeshType>
{
    using Base = ConvertActionT<MeshType>;

    std::string name() const final { return "Convert to PolyEdgeMesh"; }

    std::pair<MeshTypeId, std::any> convert(
        const MeshType&                  inputMesh,
        AbstractLogger&                  log) const final
    {
        using PolyEdgeMeshType = GetMeshType<MeshTypeId::POLYGON_MESH>;

        PolyEdgeMeshType polyEdgeMesh;
        polyEdgeMesh.importFrom(inputMesh);

        return {MeshTypeId::POLYGON_MESH, std::any(polyEdgeMesh)};
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_CONVERT_POLY_EDGE_MESH_H
