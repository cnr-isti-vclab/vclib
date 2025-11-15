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

#include <vclib/space/core.h>

namespace vcl::detail {

struct ObjMaterial
{
    // id of the material in the mesh, used when loading materials
    uint mapId = UINT_NULL;

    Point3f Ka = Point3f(0.2f, 0.2f, 0.2f); // ambient
    Point3f Kd = Point3f(1.0f, 1.0f, 1.0f); // diffuse
    Point3f Ks = Point3f(1.0f, 1.0f, 1.0f); // specular

    float d = 1.0f; // alpha

    int   illum = 2; // specular illumination
    float Ns    = 0.f;

    std::string map_Kd; // filename texture

    ObjMaterial() = default;

    ObjMaterial(const Color& c)
    {
        Kd.x() = c.redF();
        Kd.y() = c.greenF();
        Kd.z() = c.blueF();
        d      = c.alphaF();
    }

    ObjMaterial(const std::string& txtName) : map_Kd(txtName)
    {
    }

    ObjMaterial(const Color& c, const std::string& txtName) : map_Kd(txtName)
    {
        Kd.x() = c.redF();
        Kd.y() = c.greenF();
        Kd.z() = c.blueF();
        d      = c.alphaF();
    }

    bool isValid() const { return mapId != UINT_NULL; }

    // returns true if all the material components except the color
    // are default
    bool justFaceColor() const {
        ObjMaterial tmp = *this;
        tmp.Kd = Point3f(1.0f, 1.0f, 1.0f);
        return tmp == ObjMaterial();
    }

    Color color() const
    {
        return Color(Kd.x() * 255, Kd.y() * 255, Kd.z() * 255, d * 255);
    }

    const std::string& texture() const { return map_Kd; }

    uint textureId() const { return mapId; }

    auto operator<=>(const ObjMaterial& m) const
    {
        std::partial_ordering res = mapId <=> m.mapId;
        if (res != std::partial_ordering::equivalent)
            return res;
        res = Ka <=> m.Ka;
        if (res != std::partial_ordering::equivalent)
            return res;
        res = Kd <=> m.Kd;
        if (res != std::partial_ordering::equivalent)
            return res;
        res = Ks <=> m.Ks;
        if (res != std::partial_ordering::equivalent)
            return res;
        res = d <=> m.d;
        if (res != std::partial_ordering::equivalent)
            return res;
        res = illum <=> m.illum;
        if (res != std::partial_ordering::equivalent)
            return res;
        res = Ns <=> m.Ns;
        if (res != std::partial_ordering::equivalent)
            return res;
        res = map_Kd <=> m.map_Kd;
        if (res != std::partial_ordering::equivalent)
            return res;
        return std::partial_ordering::equivalent;
    }

    bool operator==(const ObjMaterial& m) const
    {
        return !(*this < m) && !(m < *this);
    }

    bool operator!=(const ObjMaterial& m) const { return !(*this == m); }
};

inline std::ostream& operator<<(std::ostream& out, const ObjMaterial& m)
{
    out << "Kd " << m.Kd.x() << " " << m.Kd.y() << " " << m.Kd.z()
        << std::endl;
    out << "d " << m.d << std::endl;
    if (!m.map_Kd.empty()) {
        out << "map_Kd " << m.map_Kd << std::endl;
    }
    return out;
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_OBJ_MATERIAL_H
