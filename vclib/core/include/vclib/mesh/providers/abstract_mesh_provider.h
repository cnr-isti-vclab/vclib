// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_PROVIDERS_ABSTRACT_MESH_PROVIDER_H
#define VCL_MESH_PROVIDERS_ABSTRACT_MESH_PROVIDER_H

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <functional>
#include <vector>

namespace vcl {

/**
 * @brief An abstract interface that exposes the data of a mesh in a type-erased
 * way, allowing it to be used in generic contexts (e.g., UI editors, renderers)
 * without knowing its concrete type.
 */
class AbstractMeshProvider
{
public:
    using VertexPositionCallback = std::function<void(const vcl::Point3d&)>;
    using MatIt                  = std::vector<vcl::Material>::const_iterator;

    virtual ~AbstractMeshProvider() = default;

    /* Geometry */
    virtual std::vector<vcl::Point3d> facePositions(uint faceId) const  = 0;
    virtual vcl::Point3d              vertexPosition(uint vertId) const = 0;
    
    virtual vcl::Box3d boundingBox() const            = 0;
    virtual vcl::Box3d transformedBoundingBox() const = 0;

    /**
     * @brief Utility method that calls the provided callback passing the vertex
     * position. Using this callback can prevent deep copies and temporary objects 
     * if the underlying mesh scalar type matches the requested vcl::Point3d type.
     */
    virtual void queryVertexPosition(uint vertId, VertexPositionCallback cb)
        const = 0;

    /* Topology */
    virtual uint vertexCount() const = 0;
    virtual uint faceCount() const   = 0;
    virtual uint edgeCount() const   = 0;

    virtual uint selectedVertexCount() const = 0;
    virtual uint selectedFaceCount() const   = 0;

    /* Transform */
    virtual vcl::Matrix44d transformMatrix() const = 0;

    /* Appearance / Materials */
    virtual View<MatIt>  materials() const                           = 0;
    virtual const Image& textureImage(const std::string& path) const = 0;
};

} // namespace vcl

#endif // VCL_MESH_PROVIDERS_ABSTRACT_MESH_PROVIDER_H
