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

#ifndef VCL_RENDER_DRAWABLE_ABSTRACT_DRAWABLE_MESH_H
#define VCL_RENDER_DRAWABLE_ABSTRACT_DRAWABLE_MESH_H

#include "drawable_object.h"
#include "mesh/mesh_render_settings.h"

#include <vclib/space/core/matrix.h>

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

public:
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

    virtual uint vertexNumber() const = 0;

    virtual uint faceNumber() const = 0;

    virtual uint edgeNumber() const = 0;

    virtual vcl::Matrix44d transformMatrix() const = 0;

    // DrawableObject implementation

    inline bool isVisible() const { return mMRS.isVisible(); }

    inline void setVisibility(bool vis) { mMRS.setVisibility(vis); }

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
