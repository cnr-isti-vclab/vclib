// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_PROVIDERS_MESH_PROVIDER_MIXIN_H
#define VCL_MESH_PROVIDERS_MESH_PROVIDER_MIXIN_H

#include "abstract_mesh_provider.h"

#include <vclib/mesh/elem_algorithms/selection.h>
#include <vclib/mesh/requirements/mesh_requirements.h>

#include <vclib/algorithms/core.h>

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

    vcl::Point3d vertexPosition(uint vertId) const override
    {
        return getMesh().vertex(vertId).position().template cast<double>();
    }

    std::vector<vcl::Point3d> facePositions(uint faceId) const override
    {
        std::vector<vcl::Point3d> pos;
        if constexpr (vcl::HasFaces<MeshType>) {
            const auto& f = getMesh().face(faceId);
            for (auto* v : f.vertices()) {
                pos.push_back(v->position().template cast<double>());
            }
        }
        return pos;
    }

    std::pair<vcl::Point3d, vcl::Point3d> edgePositions(
        uint edgeId) const override
    {
        if constexpr (vcl::HasEdges<MeshType> && vcl::HasVertices<MeshType>) {
            const auto& e = getMesh().edge(edgeId);
            return {
                e.vertex(0)->position().template cast<double>(),
                e.vertex(1)->position().template cast<double>()};
        }
        return {vcl::Point3d(), vcl::Point3d()};
    }

    vcl::Box3d boundingBox() const override
    {
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (!getMesh().boundingBox().isNull()) {
                return getMesh().boundingBox().template cast<double>();
            }
        }
        return computeBoundingBox();
    }

    vcl::Box3d transformedBoundingBox() const override
    {
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (!getMesh().boundingBox().isNull()) {
                return getMesh()
                    .transformedBoundingBox()
                    .template cast<double>();
            }
        }

        vcl::Box3d bb = computeBoundingBox();
        if constexpr (vcl::HasTransformMatrix<MeshType>) {
            bb = vcl::transformBox(
                bb,
                vcl::Matrix44d(
                    getMesh().transformMatrix().template cast<double>()));
        }
        return bb;
    }

    void queryVertexPosition(uint vertId, VertexPositionCallback cb)
        const override
    {
        using PosType =
            std::decay_t<decltype(getMesh().vertex(vertId).position())>;
        if constexpr (std::is_same_v<PosType, vcl::Point3d>) {
            cb(getMesh().vertex(vertId).position());
        }
        else {
            cb(getMesh().vertex(vertId).position().template cast<double>());
        }
    }

    /* Topology */

    uint vertexCount() const override { return getMesh().vertexCount(); }

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

    std::vector<uint> faceVertices(uint faceId) const override
    {
        std::vector<uint> ids;
        if constexpr (vcl::HasFaces<MeshType>) {
            const auto& f = getMesh().face(faceId);
            for (auto* v : f.vertices()) {
                ids.push_back(v->index());
            }
        }
        return ids;
    }

    std::pair<uint, uint> edgeVertices(uint edgeId) const override
    {
        if constexpr (vcl::HasEdges<MeshType>) {
            const auto& e = getMesh().edge(edgeId);
            return {e.vertex(0)->index(), e.vertex(1)->index()};
        }
        return {0, 0};
    }

    uint selectedVertexCount() const override
    {
        return vcl::vertexSelectionCount(getMesh());
    }

    uint selectedFaceCount() const override
    {
        if constexpr (vcl::HasFaces<MeshType>) {
            return vcl::faceSelectionCount(getMesh());
        }
        return 0;
    }

    uint selectedEdgeCount() const override
    {
        if constexpr (vcl::HasEdges<MeshType>) {
            return vcl::edgeSelectionCount(getMesh());
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

    vcl::Box3d computeBoundingBox() const
    {
        vcl::Box3d b;
        for (const auto& v : getMesh().vertices()) {
            b.add(v.position().template cast<double>());
        }
        return b;
    }
};

} // namespace vcl

#endif // VCL_MESH_PROVIDERS_MESH_PROVIDER_MIXIN_H
