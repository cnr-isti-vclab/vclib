// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_PROVIDERS_ABSTRACT_GEOMETRY_PROVIDER_H
#define VCL_MESH_PROVIDERS_ABSTRACT_GEOMETRY_PROVIDER_H

#include <vclib/space/core.h>

#include <functional>
#include <vector>

namespace vcl {

class AbstractGeometryProvider
{
public:
    using FacePositionsCallback =
        std::function<void(const std::vector<vcl::Point3d>&)>;
    using VertexPositionCallback = std::function<void(const vcl::Point3d&)>;

    virtual ~AbstractGeometryProvider() = default;

    // Direct Getters
    virtual std::vector<vcl::Point3d> facePositions(uint faceId) const  = 0;
    virtual vcl::Point3d              vertexPosition(uint vertId) const = 0;

    // Callback Getters
    virtual void queryFacePositions(uint faceId, FacePositionsCallback cb)
        const = 0;
    virtual void queryVertexPosition(uint vertId, VertexPositionCallback cb)
        const = 0;
};

} // namespace vcl

#endif // VCL_MESH_PROVIDERS_ABSTRACT_GEOMETRY_PROVIDER_H
