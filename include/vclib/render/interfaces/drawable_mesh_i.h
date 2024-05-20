/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_RENDER_INTERFACES_DRAWABLE_MESH_I_H
#define VCL_RENDER_INTERFACES_DRAWABLE_MESH_I_H

#include "drawable_object_i.h"

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

namespace vcl {

class DrawableMeshI : public vcl::DrawableObjectI
{
protected:
    MeshRenderSettings mMRS;

public:
    DrawableMeshI() = default;

    template<MeshConcept MeshType>
    DrawableMeshI(const MeshType& m) : mMRS(m)
    {
    }

    const MeshRenderSettings& renderSettings() const { return mMRS; }

    virtual void setRenderSettings(const MeshRenderSettings& rs) { mMRS = rs; }

    // DrawableObject implementation

    inline bool isVisible() const { return mMRS.isVisible(); }

    inline void setVisibility(bool vis) { mMRS.setVisibility(vis); }
};

} // namespace vcl

#endif // VCL_RENDER_INTERFACES_DRAWABLE_MESH_I_H
