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

#ifndef VCL_IO_MESH_PLY_DETAIL_MATERIAL_H
#define VCL_IO_MESH_PLY_DETAIL_MATERIAL_H

#include "header.h"

#include <vclib/io/read.h>
#include <vclib/io/write.h>

#include <vclib/mesh.h>

namespace vcl::detail {

template<MeshConcept MeshType>
void writePlyMaterials(
    std::ostream&    file,
    const PlyHeader& header,
    const MeshType&  mesh)
{
    FileType format;
    if (header.format() == ply::ASCII) {
        format.isBinary = false;
    }
    else if (header.format() == ply::BINARY_BIG_ENDIAN) {
        format.endian = std::endian::big;
    }

    for (const Material& m : mesh.materials()) {
        for (const PlyProperty& p : header.materialProperties()) {
            using enum Material::TextureType;
            bool hasBeenWritten = false;
            if (p.name >= ply::red && p.name <= ply::alpha) {
                io::writeProperty(
                    file, m.baseColor()[p.name - ply::red], p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::metallic) {
                io::writeProperty(file, m.metallic(), p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::roughness) {
                io::writeProperty(file, m.roughness(), p.type, format);
                hasBeenWritten = true;
            }
            if (p.name >= ply::emissive_red && p.name <= ply::emissive_blue) {
                io::writeProperty(
                    file,
                    m.emissiveColor()[p.name - ply::emissive_red],
                    p.type,
                    format);
                hasBeenWritten = true;
            }
            if (p.name == ply::alpha_mode) {
                io::writeProperty(
                    file, toUnderlying(m.alphaMode()), p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::alpha_cutoff) {
                io::writeProperty(file, m.alphaCutoff(), p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::normal_scale) {
                io::writeProperty(file, m.normalScale(), p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::occlusion_strength) {
                io::writeProperty(file, m.occlusionStrength(), p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::double_sided) {
                io::writeProperty(
                    file, static_cast<uint>(m.doubleSided()), p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::base_color_texture) {
                const std::string& path =
                    m.textureDescriptor(BASE_COLOR).path();
                io::writeProperty(file, path.size(), p.listSizeType, format);
                for (const char& c : path) {
                    io::writeProperty(file, c, p.type, format);
                }
                hasBeenWritten = true;
            }
            if (p.name == ply::metallic_roughness_texture) {
                const std::string& path =
                    m.textureDescriptor(METALLIC_ROUGHNESS).path();
                io::writeProperty(file, path.size(), p.listSizeType, format);
                for (const char& c : path) {
                    io::writeProperty(file, c, p.type, format);
                }
                hasBeenWritten = true;
            }
            if (p.name == ply::normal_texture) {
                const std::string& path =
                    m.textureDescriptor(NORMAL).path();
                io::writeProperty(file, path.size(), p.listSizeType, format);
                for (const char& c : path) {
                    io::writeProperty(file, c, p.type, format);
                }
                hasBeenWritten = true;
            }
            if (p.name == ply::occlusion_texture) {
                const std::string& path =
                    m.textureDescriptor(OCCLUSION).path();
                io::writeProperty(file, path.size(), p.listSizeType, format);
                for (const char& c : path) {
                    io::writeProperty(file, c, p.type, format);
                }
                hasBeenWritten = true;
            }
            if (p.name == ply::emissive_texture) {
                const std::string& path =
                    m.textureDescriptor(EMISSIVE).path();
                io::writeProperty(file, path.size(), p.listSizeType, format);
                for (const char& c : path) {
                    io::writeProperty(file, c, p.type, format);
                }
                hasBeenWritten = true;
            }

            if (!hasBeenWritten) {
                // be sure to write something if the header declares some
                // property that is not in the mesh
                io::writeProperty(file, 0, p.type, format);
            }
        }
        if (!format.isBinary)
            file << std::endl;
    }
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_PLY_DETAIL_MATERIAL_H
