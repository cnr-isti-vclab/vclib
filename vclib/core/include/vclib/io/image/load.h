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

#ifndef VCL_IO_IMAGE_LOAD_H
#define VCL_IO_IMAGE_LOAD_H

#ifdef VCLIB_WITH_QT
#include "qt/load.h"
#endif

#ifdef VCLIB_WITH_STB
#include "stb/load.h"
#endif

#include <vclib/io/file_format.h>
#include <vclib/io/file_info.h>

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <memory>
#include <set>
#include <string>

namespace vcl {

/**
 * @brief Returns the set of image formats supported for loading.
 *
 * The set contains all the image formats that can be loaded using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of image formats supported for loading.
 */
inline std::set<FileFormat> loadImageFormats()
{
    std::set<FileFormat> ff;

#ifdef VCLIB_WITH_QT
    auto fqt = qt::loadImageFormats();
    ff.insert(fqt.begin(), fqt.end());
#endif

#ifdef VCLIB_WITH_STB
    auto fstb = stb::loadImageFormats();
    ff.insert(fstb.begin(), fstb.end());
#endif
    return ff;
}

inline std::shared_ptr<unsigned char> loadImageData(
    const std::string& filename,
    int&               w,
    int&               h)
{
    FileFormat ff = FileInfo::fileFormat(filename);

#ifdef VCLIB_WITH_QT
    if (qt::loadImageFormats().contains(ff)) {
        return qt::loadImageData(filename, w, h);
    }
#endif

#ifdef VCLIB_WITH_STB
    if (stb::loadImageFormats().contains(ff)) {
        return stb::loadImageData(filename, w, h);
    }
#endif
    throw UnknownFileFormatException(ff.extensions().front());
}

inline Image loadImage(const std::string& filename)
{
    int  w, h;
    auto data = loadImageData(filename, w, h);
    if (!data) {
        return Image();
    }
    return Image(data.get(), w, h);
}

/**
 * @brief Loads from files the texture images of the materials of a mesh.
 *
 * The images are loaded from the given base path (or the mesh base path if
 * `basePath` is left empty), using the paths stored in the texture descriptors
 * of the materials.
 *
 * @tparam MeshType: The type of the mesh.
 * @tparam LogType: The type of the logger.
 * @param[in] mesh: The mesh containing the materials.
 * @param[in] basePath: The base path where to load the texture images.
 * @param[in] textureTypesToSave: A BitSet indicating which texture types to
 * load. By default, all texture types are loaded.
 * @param[in] log: The logger to use to log messages.
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadTextureImages(
    MeshType&      mesh,
    std::string    basePath           = "",
    const BitSet8& textureTypesToLoad = BitSet8::ALL(),
    LogType&       log = nullLogger) requires HasMaterials<MeshType>
{
    if (basePath.empty()) {
        basePath = mesh.meshBasePath();
    }

    for (const Material& mat : mesh.materials()) {
        using enum Material::TextureType;
        const uint N_TEXTURE_TYPES = toUnderlying(COUNT);
        for (uint i = 0; i < N_TEXTURE_TYPES; ++i) {
            auto texType = static_cast<Material::TextureType>(i);
            // supported textures to load
            if (textureTypesToLoad[i]) {
                const TextureDescriptor& tex = mat.textureDescriptor(i);
                if (!tex.isNull()) {
                    // if not already loaded
                    if (mesh.textureImage(tex.path()).isNull()) {
                        Image img = loadImage(basePath + tex.path());
                        if (img.isNull()) {
                            log.log(
                                "Cannot load texture " + tex.path(),
                                LogType::WARNING_LOG);
                        }
                        else {
                            img.colorSpace() =
                                Material::textureTypeToColorSpace(texType);
                            mesh.pushTextureImage(tex.path(), std::move(img));
                        }
                    }
                }
            }
        }
    }
}

} // namespace vcl

#endif // VCL_IO_IMAGE_LOAD_H
