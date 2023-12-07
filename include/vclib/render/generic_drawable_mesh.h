/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_RENDER_GENERIC_DRAWABLE_MESH_H
#define VCL_RENDER_GENERIC_DRAWABLE_MESH_H

#include "drawable_object.h"
#include "mesh_render_settings.h"

namespace vcl {

class GenericDrawableMesh : public vcl::DrawableObject
{
protected:
    MeshRenderSettings mrs;

public:
    GenericDrawableMesh() = default;

    template<MeshConcept MeshType>
    GenericDrawableMesh(const MeshType& m) : mrs(m)
    {
    }

    const MeshRenderSettings& renderSettings() const { return mrs; }

    void setRenderSettings(const MeshRenderSettings& rs) { mrs = rs; }

    // DrawableObject implementation
    void draw() const = 0;

    vcl::Point3d center() const = 0;

    double radius() const = 0;

    DrawableObject* clone() const = 0;

    inline bool isVisible() const { return mrs.isVisible(); }

    inline void setVisibility(bool vis) { mrs.setVisibility(vis); }
};

} // namespace vcl

#endif // VCL_RENDER_GENERIC_DRAWABLE_MESH_H
