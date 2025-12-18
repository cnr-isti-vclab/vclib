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

template<typename Stream>
void readPlyMaterialProperty(
    Stream&     file,
    Material&   mat,
    PlyProperty p,
    std::endian end = std::endian::little)
{
    bool hasBeenRead = false;
    using enum Material::TextureType;
    if (p.name == ply::name) {
        uint size = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        std::string name;
        name.resize(size);
        for (uint i = 0; i < size; ++i) {
            name[i] = io::readPrimitiveType<char>(file, p.type, end);
        }
        mat.name() = name;
        hasBeenRead = true;
    }
    if (p.name >= ply::red && p.name <= ply::alpha) {
        uint idx = p.name - ply::red;
        mat.baseColor()[idx] =
            io::readPrimitiveType<float>(file, p.type, end);
        hasBeenRead = true;
    }
    if (p.name == ply::metallic) {
        mat.metallic() = io::readPrimitiveType<float>(file, p.type, end);
        hasBeenRead = true;
    }
    if (p.name == ply::roughness) {
        mat.roughness() = io::readPrimitiveType<float>(file, p.type, end);
        hasBeenRead = true;
    }
    if (p.name >= ply::emissive_red && p.name <= ply::emissive_blue) {
        uint idx = p.name - ply::emissive_red;
        mat.emissiveColor()[idx] =
            io::readPrimitiveType<float>(file, p.type, end);
        hasBeenRead = true;
    }
    if (p.name == ply::alpha_mode) {
        mat.alphaMode() =
            static_cast<Material::AlphaMode>(
                io::readPrimitiveType<uint>(file, p.type, end));
        hasBeenRead = true;
    }
    if (p.name == ply::alpha_cutoff) {
        mat.alphaCutoff() = io::readPrimitiveType<float>(file, p.type, end);
        hasBeenRead = true;
    }
    if (p.name == ply::normal_scale) {
        mat.normalScale() = io::readPrimitiveType<float>(file, p.type, end);
        hasBeenRead = true;
    }
    if (p.name == ply::occlusion_strength) {
        mat.occlusionStrength() =
            io::readPrimitiveType<float>(file, p.type, end);
        hasBeenRead = true;
    }
    if (p.name == ply::double_sided) {
        mat.doubleSided() =
            static_cast<bool>(
                io::readPrimitiveType<uint>(file, p.type, end));
        hasBeenRead = true;
    }
    if (p.name == ply::base_color_texture) {
        uint size = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        std::string path;
        path.resize(size);
        for (uint i = 0; i < size; ++i) {
            path[i] = io::readPrimitiveType<char>(file, p.type, end);
        }
        mat.textureDescriptor(BASE_COLOR) = TextureDescriptor(path);
        hasBeenRead = true;
    }
    if (p.name == ply::metallic_roughness_texture) {
        uint size = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        std::string path;
        path.resize(size);
        for (uint i = 0; i < size; ++i) {
            path[i] = io::readPrimitiveType<char>(file, p.type, end);
        }
        mat.textureDescriptor(METALLIC_ROUGHNESS) = TextureDescriptor(path);
        hasBeenRead = true;
    }
    if (p.name == ply::normal_texture) {
        uint size = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        std::string path;
        path.resize(size);
        for (uint i = 0; i < size; ++i) {
            path[i] = io::readPrimitiveType<char>(file, p.type, end);
        }
        mat.textureDescriptor(NORMAL) = TextureDescriptor(path);
        hasBeenRead = true;
    }
    if (p.name == ply::occlusion_texture) {
        uint size = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        std::string path;
        path.resize(size);
        for (uint i = 0; i < size; ++i) {
            path[i] = io::readPrimitiveType<char>(file, p.type, end);
        }
        mat.textureDescriptor(OCCLUSION) = TextureDescriptor(path);
        hasBeenRead = true;
    }
    if (p.name == ply::emissive_texture) {
        uint size = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        std::string path;
        path.resize(size);
        for (uint i = 0; i < size; ++i) {
            path[i] = io::readPrimitiveType<char>(file, p.type, end);
        }
        mat.textureDescriptor(EMISSIVE) = TextureDescriptor(path);
        hasBeenRead = true;
    }

    // if nothing has been read, it means that there is some data we don't know
    // we still need to read and discard what we read
    if (!hasBeenRead) {
        if (p.list) {
            uint s = io::readPrimitiveType<int>(file, p.listSizeType, end);
            for (uint i = 0; i < s; ++i)
                io::readPrimitiveType<int>(file, p.type, end);
        }
        else {
            io::readPrimitiveType<int>(file, p.type, end);
        }
    }
}

inline void readPlyMaterialTxt(
    std::istream&                 file,
    Material&                     mat,
    const std::list<PlyProperty>& matProperties)
{
    Tokenizer           spaceTokenizer = readAndTokenizeNextNonEmptyLine(file);
    Tokenizer::iterator token          = spaceTokenizer.begin();
    for (const PlyProperty& p : matProperties) {
        if (token == spaceTokenizer.end()) {
            throw MalformedFileException("Unexpected end of line.");
        }
        readPlyMaterialProperty(token, mat, p);
    }
}

inline void readPlyMaterialBin(
    std::istream&                 file,
    Material&                     mat,
    const std::list<PlyProperty>& matProperties,
    std::endian                   end)
{
    for (const PlyProperty& p : matProperties) {
        readPlyMaterialProperty(file, mat, p, end);
    }
}

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
        using enum Material::TextureType;
        for (const PlyProperty& p : header.materialProperties()) {
            bool hasBeenWritten = false;
            if (p.name == ply::name) {
                const std::string& path = m.name();
                io::writeProperty(file, path.size(), p.listSizeType, format);
                for (const char& c : path) {
                    io::writeProperty(file, c, p.type, format);
                }
                hasBeenWritten = true;
            }
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

template<MeshConcept MeshType, LoggerConcept LogType>
void readPlyMaterials(
    std::istream&    file,
    const PlyHeader& header,
    MeshType&        mesh,
    LogType&         log)
{
    log.startProgress("Reading materials", header.numberMaterials());

    for (uint mid = 0; mid < header.numberMaterials(); ++mid) {
        Material mat;
        if (header.format() == ply::ASCII) {
            detail::readPlyMaterialTxt(
                file, mat, header.materialProperties());
        }
        else {
            std::endian end = header.format() == ply::BINARY_BIG_ENDIAN ?
                                  std::endian::big :
                                  std::endian::little;
            detail::readPlyMaterialBin(
                file, mat, header.materialProperties(), end);
        }
        mesh.pushMaterial(mat);

        log.progress(mid);
    }

    log.endProgress();
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_PLY_DETAIL_MATERIAL_H
