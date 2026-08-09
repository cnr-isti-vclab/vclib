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
#include <vclib/mesh.h>
#include <vclib/space/core.h>

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
protected:
    MeshRenderSettings mMRS;

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

    virtual void updateRenderSettingsCapabilities() = 0;

    virtual void setRenderSettings(const MeshRenderSettings& rs) { mMRS = rs; }

    virtual const AbstractMeshProvider& meshProvider() const = 0;

    virtual void computeSelection(const SelectionParameters& params) {}

    virtual vcl::BitVector<true> vertexSelectionBitVector() const { return {}; }

    virtual void setVertexSelectionBitVector(const vcl::BitVector<true>&) {}

    virtual vcl::BitVector<true> faceSelectionBitVector() const { return {}; }

    virtual void setFaceSelectionBitVector(const vcl::BitVector<true>&) {}

    void setOnSelectionUpdatedCallback(std::function<void()> cb)
    {
        mOnSelectionUpdated = std::move(cb);
    }

    virtual bool isSelectionReadbackPending() const { return false; }

    // DrawableObject implementation

    Box3d boundingBox() const override
    {
        return meshProvider().transformedBoundingBox().template cast<double>();
    }

    inline bool isVisible() const override { return mMRS.isVisible(); }

    inline void setVisibility(bool vis) override { mMRS.setVisibility(vis); }

protected:
    void swap(AbstractDrawableMesh& other)
    {
        using std::swap;
        vcl::DrawableObject::swap(other);
        swap(mMRS, other.mMRS);
    }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_ABSTRACT_DRAWABLE_MESH_H
