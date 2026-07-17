// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWABLE_ABSTRACT_DRAWABLE_MESH_H
#define VCL_RENDER_DRAWABLE_ABSTRACT_DRAWABLE_MESH_H

#include "drawable_object.h"
#include "mesh/mesh_render_settings.h"

#include <vclib/render/selection/selection_parameters.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/space/core/matrix.h>

#include <functional>

namespace vcl {

/**
 * @brief The AbstractDrawableMesh class is the base class for all the drawable
 * meshes in the VCLib render system.
 *
 * It provides a common interface for drawable meshes, allowing to handle their
 * render settings.
 */
class AbstractDrawableMesh : public vcl::DrawableObject
{
    inline static const Image EMPTY_IMAGE;

protected:
    MeshRenderSettings mMRS;

    Box3d mBoundingBox;

    std::function<void()> mOnSelectionUpdated;

public:
    using MatIt = std::vector<Material>::const_iterator;

    AbstractDrawableMesh() = default;

    AbstractDrawableMesh(const AbstractDrawableMesh& other) = default;

    template<MeshConcept MeshType>
    AbstractDrawableMesh(const MeshType& m) : mMRS(m)
    {
    }

    const MeshRenderSettings& renderSettings() const { return mMRS; }

    virtual void updateBuffers(
        MeshRenderInfo::BuffersBitSet buffersToUpdate =
            MeshRenderInfo::BUFFERS_ALL) = 0;

    virtual void setRenderSettings(const MeshRenderSettings& rs) { mMRS = rs; }

    virtual uint vertexCount() const = 0;

    virtual uint faceCount() const = 0;

    virtual uint edgeCount() const = 0;

    virtual vcl::Matrix44d modelMatrix() const = 0;

    virtual View<MatIt> materials() const { return View<MatIt>(); }

    virtual const Image& textureImage(const std::string& path) const
    {
        return EMPTY_IMAGE;
    }

    virtual uint selectedVertexCount() const { return 0; }

    virtual uint selectedFaceCount() const { return 0; }

    virtual void computeSelection(const SelectionParameters& params) {}

    void setOnSelectionUpdatedCallback(std::function<void()> cb)
    {
        mOnSelectionUpdated = std::move(cb);
    }

    virtual bool isSelectionReadbackPending() const { return false; }

    // DrawableObject implementation

    Box3d boundingBox() const override { return mBoundingBox; }

    inline bool isVisible() const override { return mMRS.isVisible(); }

    inline void setVisibility(bool vis) override { mMRS.setVisibility(vis); }

protected:
    void swap(AbstractDrawableMesh& other)
    {
        using std::swap;
        vcl::DrawableObject::swap(other);
        swap(mMRS, other.mMRS);
        swap(mBoundingBox, other.mBoundingBox);
    }

    // if the mesh does not have a bounding box, or if it has it but it is
    // null, compute it from the vertex positions. If the mesh has a
    // transformation matrix, apply it to the bounding box.
    // The DrawableMesh must return the *transformed* bounding box.
    template<MeshConcept MeshType>
    void computeBoundingBox(const MeshType& m)
    {
        bool bbToInitialize = !vcl::HasBoundingBox<MeshType>;
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (m.boundingBox().isNull()) {
                bbToInitialize = true;
            }
            else {
                mBoundingBox =
                    m.MeshType::boundingBox().template cast<double>();
            }
        }

        if (bbToInitialize) {
            mBoundingBox = vcl::boundingBox(m).template cast<double>();
        }

        if constexpr (HasTransformMatrix<MeshType>) {
            mBoundingBox = transformBox(
                mBoundingBox,
                Matrix44d(m.transformMatrix().template cast<double>()));
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_ABSTRACT_DRAWABLE_MESH_H
