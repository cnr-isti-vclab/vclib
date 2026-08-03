// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_PROVIDERS_MESH_PROVIDER_MIXIN_H
#define VCL_MESH_PROVIDERS_MESH_PROVIDER_MIXIN_H

#include <vclib/mesh/providers/abstract_mesh_provider.h>
#include <vclib/mesh/requirements/mesh_requirements.h>

#include <type_traits>

namespace vcl {

/**
 * @brief A Mixin class that automatically implements the AbstractMeshProvider
 * interface for a Mesh.
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
 * @tparam Derived: The class inheriting from this Mixin.
 * @tparam MeshType: The type of the mesh that provides the components. Defaults
 * to Derived.
 */
template<typename Derived, typename MeshType = Derived>
class MeshProviderMixin : public virtual AbstractMeshProvider
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
    MeshProviderMixin()
    {
        static_assert(
            MeshConcept<MeshType>,
            "The MeshType in MeshProviderMixin must satisfy the MeshConcept.");
    }

    /* Geometry */

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

    void queryVertexPosition(uint vertId, VertexPositionCallback cb)
        const override
    {
        if constexpr (vcl::HasVertices<MeshType>) {
            using PosType = std::decay_t<decltype(getMesh().vertex(vertId).position())>;
            if constexpr (std::is_same_v<PosType, vcl::Point3d>) {
                cb(getMesh().vertex(vertId).position());
            } else {
                cb(getMesh().vertex(vertId).position().template cast<double>());
            }
        } else {
            cb(vcl::Point3d());
        }
    }

    /* Topology */

    uint vertexCount() const override
    {
        if constexpr (vcl::HasVertices<MeshType>) {
            return getMesh().vertexCount();
        }
        return 0;
    }

    uint faceCount() const override
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            return getMesh().faceCount();
        }
        return 0;
    }

    uint edgeCount() const override
    {
        if constexpr (vcl::HasEdges<MeshType>) {
            return getMesh().edgeCount();
        }
        return 0;
    }

    /* Transform */

    vcl::Matrix44d transformMatrix() const override
    {
        if constexpr (vcl::HasTransformMatrix<MeshType>) {
            return getMesh().transformMatrix().template cast<double>();
        }
        return vcl::Matrix44d::Identity();
    }

    /* Appearance / Materials */

    View<MatIt> materials() const override
    {
        if constexpr (vcl::HasMaterials<MeshType>) {
            return getMesh().materials();
        }
        return View<MatIt>();
    }

    const Image& textureImage(const std::string& path) const override
    {
        if constexpr (vcl::HasMaterials<MeshType>) {
            return getMesh().textureImage(path);
        }
        static const Image EMPTY_IMAGE;
        return EMPTY_IMAGE;
    }
};

} // namespace vcl

#endif // VCL_MESH_PROVIDERS_MESH_PROVIDER_MIXIN_H
