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

#ifndef VCL_IO_MESH_OBJ_MATERIAL_H
#define VCL_IO_MESH_OBJ_MATERIAL_H

#include <vclib/space/core/color.h>
#include <vclib/space/core/point.h>

namespace vcl::detail {

struct ObjMaterial
{
    Point3f Ka = Point3f(0.2f, 0.2f, 0.2f); // ambient
    Point3f Kd = Point3f(1.0f, 1.0f, 1.0f); // diffuse
    Point3f Ks = Point3f(1.0f, 1.0f, 1.0f); // specular

    float d = 1.0f; // alpha

    int   illum = 2; // specular illumination
    float Ns    = 0.f;

    std::string map_Kd; // filename texture

    // id of the texture in the mesh, used when loading materials
    uint mapId;

    bool hasColor   = false;
    bool hasTexture = false;

    ObjMaterial() = default;

    ObjMaterial(const Color& c) : hasColor(true)
    {
        Kd.x() = c.redF();
        Kd.y() = c.greenF();
        Kd.z() = c.blueF();
        d      = c.alphaF();
    }

    ObjMaterial(const std::string& txtName) : map_Kd(txtName), hasTexture(true)
    {
    }

    ObjMaterial(const Color& c, const std::string& txtName) :
            map_Kd(txtName), hasColor(true), hasTexture(true)
    {
        Kd.x() = c.redF();
        Kd.y() = c.greenF();
        Kd.z() = c.blueF();
        d      = c.alphaF();
    }

    bool isEmpty() const { return !hasColor && !hasTexture; }

    Color color() const
    {
        return Color(Kd.x() * 255, Kd.y() * 255, Kd.z() * 255, d * 255);
    }

    const std::string& texture() const { return map_Kd; }

    uint textureId() const { return mapId; }

    /**
     * @brief Operator that allows to sort materials
     * first we sort trough color
     * - if a material has no color, is < than one that has a color
     * - if both materials have color, order by color: if same, check texture
     *   sort trough texture
     * - if a material has no texture, is < than one that has texture
     * - if both materials have texture, order by texture name
     */
    bool operator<(const ObjMaterial& m) const
    {
        if (hasColor) {
            if (!m.hasColor) // color > no color
                return false;
            if (Kd != m.Kd)
                return Kd < m.Kd;
            if (d != m.d)
                return d < m.d;
        }
        else if (m.hasColor) { // no color < color
            return true;
        }
        // will arrive here only if:
        // - this Material and m have both no color
        // - this Material has the same color of m
        if (hasTexture) {
            if (!m.hasTexture) // texture > no texture
                return false;
            return map_Kd < m.map_Kd;
        }
        else if (m.hasTexture) { // no texture < texture
            return true;
        }
        else { // no color and texture in both materials
            return false;
        }
    }

    bool operator==(const ObjMaterial& m) const
    {
        return !(*this < m) && !(m < *this);
    }

    bool operator!=(const ObjMaterial& m) const { return !(*this == m); }
};

inline std::ostream& operator<<(std::ostream& out, const ObjMaterial& m)
{
    if (m.hasColor) {
        out << "Kd " << m.Kd.x() << " " << m.Kd.y() << " " << m.Kd.z()
            << std::endl;
        out << "d " << m.d << std::endl;
    }
    if (m.hasTexture) {
        out << "map_Kd " << m.map_Kd << std::endl;
    }
    return out;
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_OBJ_MATERIAL_H
