// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_PROVIDERS_GEOMETRY_PROVIDER_MIXIN_H
#define VCL_MESH_PROVIDERS_GEOMETRY_PROVIDER_MIXIN_H

#include "abstract_geometry_provider.h"

#include <vclib/mesh/requirements/mesh_requirements.h>

#include <type_traits>

namespace vcl {

/**
 * @brief A Mixin class that automatically implements the
 * AbstractGeometryProvider interface for a Mesh.
 *
 * This class uses the Curiously Recurring Template Pattern (CRTP). It must be
 * inherited by a class that also inherits from a valid Mesh type.
 *
 * The template parameter `Derived` must be the final class that derives from
 * both this Mixin and the Mesh. The Mixin uses `static_cast` to cast itself to
 * `Derived` and access the mesh elements, so if `Derived` is not the actual
 * derived class or does not inherit from a Mesh, the cast or the compilation
 * will fail.
 *
 * Example of usage:
 * @code{.cpp}
 * template<MeshConcept MeshType>
 * class MyCustomMesh :
 *     public MeshType,
 *     public GeometryProviderMixin<MyCustomMesh<MeshType>>
 * {
 *     // GeometryProviderMixin automatically implements facePositions() etc.
 * };
 * @endcode
 *
 * @tparam Derived: The class inheriting from this Mixin.
 * @tparam MeshType: The type of the mesh that provides the components. Defaults
 * to Derived.
 */
template<typename Derived, typename MeshType = Derived>
class GeometryProviderMixin : public virtual AbstractGeometryProvider
{
private:
    const MeshType& getMesh() const
    {
        if constexpr (std::is_base_of_v<MeshType, Derived>) {
            return *static_cast<const Derived*>(this);
        }
        else {
            return static_cast<const Derived*>(this)->mesh();
        }
    }

public:
    /**
     * @brief Constructor that checks at compile-time if the MeshType class
     * satisfies the MeshConcept.
     */
    GeometryProviderMixin()
    {
        static_assert(
            MeshConcept<MeshType>,
            "MeshType in GeometryProviderMixin must satisfy the MeshConcept.");
    }

    std::vector<vcl::Point3d> facePositions(uint faceId) const override
    {
        std::vector<vcl::Point3d> pos;
        if constexpr (vcl::HasFaces<MeshType> && vcl::HasVertices<MeshType>) {
            const auto& f = getMesh().face(faceId);
            for (auto* v : f.vertices()) {
                pos.push_back(v->position().template cast<double>());
            }
        }
        return pos;
    }

    vcl::Point3d vertexPosition(uint vertId) const override
    {
        if constexpr (vcl::HasVertices<MeshType>) {
            return getMesh().vertex(vertId).position().template cast<double>();
        }
        return vcl::Point3d();
    }

    void queryFacePositions(uint faceId, FacePositionsCallback cb)
        const override
    {
        cb(facePositions(faceId));
    }

    void queryVertexPosition(uint vertId, VertexPositionCallback cb)
        const override
    {
        cb(vertexPosition(vertId));
    }
};

} // namespace vcl

#endif // VCL_MESH_PROVIDERS_GEOMETRY_PROVIDER_MIXIN_H
