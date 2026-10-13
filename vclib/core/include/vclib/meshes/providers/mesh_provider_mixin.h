// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_PROVIDERS_MESH_PROVIDER_MIXIN_H
#define VCL_MESHES_PROVIDERS_MESH_PROVIDER_MIXIN_H

#include "abstract_mesh_provider.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>

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
 *
 * @ingroup mesh_providers
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

    Point3d vertexPosition(uint vertId) const override
    {
        return getMesh().vertex(vertId).position().template cast<double>();
    }

    std::vector<Point3d> facePositions(uint faceId) const override
    {
        std::vector<Point3d> pos;
        if constexpr (HasFaces<MeshType>) {
            const auto& f = getMesh().face(faceId);
            for (auto* v : f.vertices()) {
                pos.push_back(v->position().template cast<double>());
            }
        }
        return pos;
    }

    Point3d faceBarycenter(uint faceId) const override
    {
        if constexpr (HasFaces<MeshType>) {
            return vcl::faceBarycenter(getMesh().face(faceId))
                .template cast<double>();
        }
        return Point3d();
    }

    std::pair<Point3d, Point3d> edgePositions(uint edgeId) const override
    {
        if constexpr (HasEdges<MeshType> && HasVertices<MeshType>) {
            const auto& e = getMesh().edge(edgeId);
            return {
                e.vertex(0)->position().template cast<double>(),
                e.vertex(1)->position().template cast<double>()};
        }
        return {Point3d(), Point3d()};
    }

    Box3d boundingBox() const override
    {
        if constexpr (HasBoundingBox<MeshType>) {
            if (!getMesh().boundingBox().isNull()) {
                return getMesh().boundingBox().template cast<double>();
            }
        }
        return computeBoundingBox();
    }

    Box3d transformedBoundingBox() const override
    {
        if constexpr (HasBoundingBox<MeshType>) {
            if (!getMesh().boundingBox().isNull()) {
                return getMesh()
                    .transformedBoundingBox()
                    .template cast<double>();
            }
        }

        // Compute the bounding box if the mesh has no bounding box component,
        // or if it has one but it is empty/null.
        Box3d bb = computeBoundingBox();
        if constexpr (HasTransformMatrix<MeshType>) {
            bb = transformBox(
                bb,
                Matrix44d(getMesh().transformMatrix().template cast<double>()));
        }
        return bb;
    }

    void queryVertexPosition(uint vertId, VertexPositionCallback cb)
        const override
    {
        using PosType =
            std::decay_t<decltype(getMesh().vertex(vertId).position())>;
        if constexpr (std::is_same_v<PosType, Point3d>) {
            cb(getMesh().vertex(vertId).position());
        }
        else {
            cb(getMesh().vertex(vertId).position().template cast<double>());
        }
    }

    /* Normals */

    bool hasVertexNormals() const override
    {
        if constexpr (HasPerVertexNormal<MeshType>) {
            return isPerVertexNormalAvailable(getMesh());
        }
        return false;
    }

    bool hasFaceNormals() const override
    {
        if constexpr (HasPerFaceNormal<MeshType>) {
            return isPerFaceNormalAvailable(getMesh());
        }
        return false;
    }

    Point3d vertexNormal(uint vertId) const override
    {
        if constexpr (HasPerVertexNormal<MeshType>) {
            return getMesh().vertex(vertId).normal().template cast<double>();
        }
        return Point3d();
    }

    Point3d faceNormal(uint faceId) const override
    {
        if constexpr (HasPerFaceNormal<MeshType>) {
            return getMesh().face(faceId).normal().template cast<double>();
        }
        return Point3d();
    }

    /* Topology */

    uint vertexCount() const override { return getMesh().vertexCount(); }

    uint faceCount() const override
    {
        if constexpr (HasFaces<MeshType>) {
            return getMesh().faceCount();
        }
        return 0;
    }

    uint edgeCount() const override
    {
        if constexpr (HasEdges<MeshType>) {
            return getMesh().edgeCount();
        }
        return 0;
    }

    std::vector<uint> faceVertices(uint faceId) const override
    {
        std::vector<uint> ids;
        if constexpr (HasFaces<MeshType>) {
            const auto& f = getMesh().face(faceId);
            for (auto* v : f.vertices()) {
                ids.push_back(v->index());
            }
        }
        return ids;
    }

    std::pair<uint, uint> edgeVertices(uint edgeId) const override
    {
        if constexpr (HasEdges<MeshType>) {
            const auto& e = getMesh().edge(edgeId);
            return {e.vertex(0)->index(), e.vertex(1)->index()};
        }
        return {0, 0};
    }

    uint selectedVertexCount() const override
    {
        return vertexSelectionCount(getMesh());
    }

    uint selectedFaceCount() const override
    {
        if constexpr (HasFaces<MeshType>) {
            return faceSelectionCount(getMesh());
        }
        return 0;
    }

    uint selectedEdgeCount() const override
    {
        if constexpr (HasEdges<MeshType>) {
            return edgeSelectionCount(getMesh());
        }
        return 0;
    }

    /* Transform */

    Matrix44d transformMatrix() const override
    {
        if constexpr (HasTransformMatrix<MeshType>) {
            return getMesh().transformMatrix().template cast<double>();
        }
        return Matrix44d::Identity();
    }

    /* Appearance / Materials */

    View<MatIt> materials() const override
    {
        if constexpr (HasMaterials<MeshType>) {
            return getMesh().materials();
        }
        return View<MatIt>();
    }

    const Image& textureImage(const std::string& path) const override
    {
        if constexpr (HasMaterials<MeshType>) {
            return getMesh().textureImage(path);
        }
        static const Image EMPTY_IMAGE;
        return EMPTY_IMAGE;
    }

private:
    const MeshType& getMesh() const
    {
        // If the CRTP Derived class is also the MeshType, we can cast directly
        // to it. Otherwise, we cast to Derived and use its mesh() method.
        if constexpr (std::is_base_of_v<MeshType, Derived>) {
            return *static_cast<const Derived*>(this);
        }
        else {
            return static_cast<const Derived*>(this)->mesh();
        }
    }

    Box3d computeBoundingBox() const
    {
        Box3d b;
        for (const auto& v : getMesh().vertices()) {
            b.add(v.position().template cast<double>());
        }
        return b;
    }
};

} // namespace vcl

#endif // VCL_MESHES_PROVIDERS_MESH_PROVIDER_MIXIN_H
