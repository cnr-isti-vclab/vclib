// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_PLY_DETAIL_PLY_H
#define VCL_IO_MESH_PLY_DETAIL_PLY_H

#include <vclib/base.h>

#include <list>

namespace vcl::detail {

// put all these enumeration names inside a ply namespace, to avoid collisions

namespace ply {

typedef enum { ASCII, BINARY_LITTLE_ENDIAN, BINARY_BIG_ENDIAN, UNKNOWN } Format;

typedef enum { VERTEX, FACE, EDGE, TRISTRIP, MATERIAL, OTHER } ElementType;

typedef enum { RGB, RGBA } ColorMode;

typedef enum {
    unknown = -1,
    x,
    y,
    z,
    nx,
    ny,
    nz,
    red,
    green,
    blue,
    alpha,
    bit_flags,
    quality,
    texture_u,
    texture_v,
    material_index,
    vertex_indices,
    texcoord,
    vertex1,
    vertex2,
    // material properties (base color is red, green, blue, alpha)
    name,
    metallic,
    roughness,
    emissive_red,
    emissive_green,
    emissive_blue,
    alpha_mode,
    alpha_cutoff,
    normal_scale,
    occlusion_strength,
    double_sided,
    base_color_texture,
    metallic_roughness_texture,
    normal_texture,
    occlusion_texture,
    emissive_texture,
    // gaussian splat properties
    f_dc_0,
    f_dc_1,
    f_dc_2,
    f_rest_0,
    f_rest_1,
    f_rest_2,
    f_rest_3,
    f_rest_4,
    f_rest_5,
    f_rest_6,
    f_rest_7,
    f_rest_8,
    f_rest_9,
    f_rest_10,
    f_rest_11,
    f_rest_12,
    f_rest_13,
    f_rest_14,
    f_rest_15,
    f_rest_16,
    f_rest_17,
    f_rest_18,
    f_rest_19,
    f_rest_20,
    f_rest_21,
    f_rest_22,
    f_rest_23,
    f_rest_24,
    f_rest_25,
    f_rest_26,
    f_rest_27,
    f_rest_28,
    f_rest_29,
    f_rest_30,
    f_rest_31,
    f_rest_32,
    f_rest_33,
    f_rest_34,
    f_rest_35,
    f_rest_36,
    f_rest_37,
    f_rest_38,
    f_rest_39,
    f_rest_40,
    f_rest_41,
    f_rest_42,
    f_rest_43,
    f_rest_44,
    opacity,
    scale_0,
    scale_1,
    scale_2,
    rot_0,
    rot_1,
    rot_2,
    rot_3
} PropertyName;

using PropertyType = PrimitiveType;

} // namespace ply

struct PlyProperty
{
    ply::PropertyName name;
    ply::PropertyType type;
    bool              list = false;
    ply::PropertyType listSizeType;
    std::string       unknownPropertyName; // when a property is not recognized
};

struct PlyElement
{
    ply::ElementType       type;
    std::list<PlyProperty> properties;
    uint                   elementCount;
    std::string unknownElementType; // when an element is not recognized
};

} // namespace vcl::detail

#endif // VCL_IO_MESH_PLY_DETAIL_PLY_H
