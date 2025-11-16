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

/**
 * @brief Represents a material loaded from an OBJ file.
 *
 * This class stores the properties of a material as defined in an OBJ file,
 * including ambient, diffuse, and specular colors, alpha value, illumination
 * model, shininess, and texture map.
 */
class ObjMaterial
{
    inline static const Point3f Ka_DEFAULT = {0.2f, 0.2f, 0.2f}; // ambient;
    inline static const Point3f Kd_DEFAULT = {1.0f, 1.0f, 1.0f}; // diffuse;
    inline static const Point3f Ks_DEFAULT = {1.0f, 1.0f, 1.0f}; // specular;

    inline static const float d_DEFAULT     = 1.0f; // alpha
    inline static const int   illum_DEFAULT = 2; // specular illumination
    inline static const float Ns_DEFAULT    = 0.f;

public:
    // id of the material in the mesh, used when loading materials
    uint        matId = UINT_NULL;
    std::string matName;

    Point3f Ka = Ka_DEFAULT; // ambient
    Point3f Kd = Kd_DEFAULT; // diffuse
    Point3f Ks = Ks_DEFAULT; // specular

    float d = d_DEFAULT; // alpha

    int   illum = illum_DEFAULT; // specular illumination
    float Ns    = Ns_DEFAULT;

    std::string map_Kd; // filename texture

    ObjMaterial() = default;

    ObjMaterial(const Material& mat, uint id)
    {
        matId   = id;
        matName = mat.name();

        Kd.x() = mat.baseColor().redF();
        Kd.y() = mat.baseColor().greenF();
        Kd.z() = mat.baseColor().blueF();
        Ns     = (1.0f / (mat.roughness() * mat.roughness())) - 2.0f;
        if (mat.alphaMode() == Material::AlphaMode::ALPHA_BLEND) {
            d = 0.5f; // not accurate, but ok
        }

        map_Kd = mat.baseColorTexture().path();
    }

/**
     * @brief Converts the OBJ material to a vcl::Material object.
     *
     * This function creates a vcl::Material from the OBJ material data,
     * converting the color values and setting the appropriate properties
     * such as roughness, alpha mode, and texture path.
     *
     * @return Material The converted material object.
     */
    Material toMaterial()
    {
        Material m;
        m.name() = matName;
        m.baseColor() =
            vcl::Color(Kd.x() * 255, Kd.y() * 255, Kd.z() * 255, 255);
        m.baseColorTexture().path() = map_Kd;

        float ns      = std::clamp(Ns, 0.f, 1000.f);
        m.roughness() = std::sqrt(2.0 / (ns + 2.0)); // todo: check

        if (d < 1.0)
            m.alphaMode() = Material::AlphaMode::ALPHA_BLEND;

        m.metallic() = 0.0;
        return m;
    }

    bool isValid() const { return matId != UINT_NULL; }

    Color color() const
    {
        return Color(Kd.x() * 255, Kd.y() * 255, Kd.z() * 255, d * 255);
    }

    const std::string& texture() const { return map_Kd; }

    uint textureId() const { return matId; }

    bool operator==(const ObjMaterial& m) const = default;

    friend inline std::ostream& operator<<(
        std::ostream&      out,
        const ObjMaterial& m);
};

inline std::ostream& operator<<(std::ostream& out, const ObjMaterial& m)
{
    if (m.Ka != m.Ka_DEFAULT)
        out << "Ka " << m.Ka.x() << " " << m.Ka.y() << " " << m.Ka.z()
            << std::endl;

    if (m.Kd != m.Kd_DEFAULT)
        out << "Kd " << m.Kd.x() << " " << m.Kd.y() << " " << m.Kd.z()
            << std::endl;

    if (m.Ks != m.Ks_DEFAULT)
        out << "Ks " << m.Ks.x() << " " << m.Ks.y() << " " << m.Ks.z()
            << std::endl;

    if (m.d != m.d_DEFAULT)
        out << "d " << m.d << std::endl;

    if (m.illum != m.illum_DEFAULT)
        out << "illum " << m.illum <<  std::endl;

    if (m.Ns != m.Ns_DEFAULT)
        out << "Ns " << m.Ns << std::endl;

    if (!m.map_Kd.empty()) {
        out << "map_Kd " << m.map_Kd << std::endl;
    }
    return out;
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_OBJ_MATERIAL_H
