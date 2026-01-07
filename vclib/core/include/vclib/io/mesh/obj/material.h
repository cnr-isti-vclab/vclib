/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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
    inline static const Point3f Ke_DEFAULT = {0.0f, 0.0f, 0.0f}; // emissive;

    inline static const float d_DEFAULT     = 1.0f; // alpha
    inline static const int   illum_DEFAULT = 2;    // specular illumination
    inline static const float Ns_DEFAULT    = 0.f;

public:
    // id of the material in the mesh, used when loading materials
    uint        matId = UINT_NULL;
    std::string matName;

    Point3f Ka = Ka_DEFAULT; // ambient
    Point3f Kd = Kd_DEFAULT; // diffuse
    Point3f Ks = Ks_DEFAULT; // specular
    Point3f Ke = Ke_DEFAULT; // emissive

    float d = d_DEFAULT; // alpha

    int   illum = illum_DEFAULT; // specular illumination
    float Ns    = Ns_DEFAULT;

    std::string map_Kd;   // filename texture
    std::string map_Ke;   // filename emissive map
    std::string map_bump; // filename bump map

    ObjMaterial() = default;

    ObjMaterial(const Material& mat, uint id)
    {
        using enum Material::TextureType;

        matId   = id;
        matName = mat.name();

        Kd.x() = mat.baseColor().redF();
        Kd.y() = mat.baseColor().greenF();
        Kd.z() = mat.baseColor().blueF();
        Ns     = std::pow(1.0f - mat.roughness(), 2) * 1000.f;
        if (mat.alphaMode() == Material::AlphaMode::ALPHA_BLEND) {
            d = mat.baseColor().alphaF();
        }

        Ke.x() = mat.emissiveColor().redF();
        Ke.y() = mat.emissiveColor().greenF();
        Ke.z() = mat.emissiveColor().blueF();

        map_Kd = mat.baseColorTextureDescriptor().path();
        map_Ke = mat.textureDescriptor(EMISSIVE).path();
        // map_bump = mat.textureDescriptor(NORMAL).path();
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
        using enum Material::TextureType;

        Material m;
        m.name() = matName;
        m.baseColor() =
            vcl::Color(Kd.x() * 255, Kd.y() * 255, Kd.z() * 255, 255);

        float ns      = std::clamp(Ns, 0.f, 1000.f);
        m.roughness() = 1.0f - std::sqrt(ns) / std::sqrt(1000.f);

        if (d < 1.0) {
            m.baseColor().alpha() = d * 255;
            m.alphaMode()         = Material::AlphaMode::ALPHA_BLEND;
        }

        m.metallic() = 0.0f; // obj materials are non-metallic;

        m.emissiveColor() =
            vcl::Color(Ke.x() * 255, Ke.y() * 255, Ke.z() * 255, 255);

        if (!map_Kd.empty()) {
            m.baseColorTextureDescriptor().path() = map_Kd;
        }
        if (!map_Ke.empty()) {
            m.textureDescriptor(EMISSIVE).path() = map_Ke;
        }
        // if (!map_bump.empty()) {
        //     m.textureDescriptor(NORMAL).path() = map_bump;
        // }

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

    if (m.Ke != m.Ke_DEFAULT)
        out << "Ke " << m.Ke.x() << " " << m.Ke.y() << " " << m.Ke.z()
            << std::endl;

    if (m.d != m.d_DEFAULT)
        out << "d " << m.d << std::endl;

    if (m.illum != m.illum_DEFAULT)
        out << "illum " << m.illum << std::endl;

    if (m.Ns != m.Ns_DEFAULT)
        out << "Ns " << m.Ns << std::endl;

    if (!m.map_Kd.empty()) {
        out << "map_Kd " << m.map_Kd << std::endl;
    }

    if (m.map_Ke.empty()) {
        out << "map_Ke " << m.map_Ke << std::endl;
    }

    // if (m.map_bump.empty()) {
    //     out << "map_bump " << m.map_bump << std::endl;
    // }

    return out;
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_OBJ_MATERIAL_H
