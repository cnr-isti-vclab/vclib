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

#ifndef VCL_IO_IMAGE_SAVE_H
#define VCL_IO_IMAGE_SAVE_H

#include "bmp/save.h"

#ifdef VCLIB_WITH_QT
#include "qt/save.h"
#endif

#ifdef VCLIB_WITH_STB
#include "stb/save.h"
#endif

#include <vclib/base.h>
#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <set>

namespace vcl {

/**
 * @brief Returns the set of image formats supported for saving.
 *
 * The set contains all the image formats that can be saved using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of image formats supported for saving.
 */
inline std::set<FileFormat> saveImageFormats()
{
    std::set<FileFormat> ff;

#ifdef VCLIB_WITH_QT
    auto fqt = qt::saveImageFormats();
    ff.insert(fqt.begin(), fqt.end());
#endif

#ifdef VCLIB_WITH_STB
    auto fstb = stb::saveImageFormats();
    ff.insert(fstb.begin(), fstb.end());
#endif
    return ff;
}

inline void saveImageData(
    const std::string&   filename,
    int                  w,
    int                  h,
    const unsigned char* data,
    uint                 quality = 90)
{
    FileFormat ff = FileInfo::fileFormat(filename);

#ifdef VCLIB_WITH_QT
    if (qt::saveImageFormats().contains(ff)) {
        return qt::saveImageData(filename, w, h, data, quality);
    }
#endif

#ifdef VCLIB_WITH_TINYGLTF
    if (stb::saveImageFormats().contains(ff)) {
        return stb::saveImageData(filename, w, h, data, quality);
    }
#endif
    if (ff == FileFormat("bmp")) {
        // save rgb image data into bmp file
        return saveImageToBmp(filename, w, h, data);
    }
    throw UnknownFileFormatException(ff.extensions().front());
}

inline void saveImage(const Image& image, const std::string& filename)
{
    saveImageData(filename, image.width(), image.height(), image.data());
}

/**
 * @brief Saves the texture images of the materials of a mesh to files.
 *
 * The images are saved in the given base path, using the paths stored in the
 * texture descriptors of the materials.
 *
 * @tparam MeshType: The type of the mesh.
 * @tparam LogType: The type of the logger.
 * @param[in] mesh: The mesh containing the materials and their texture images.
 * @param[in] basePath: The base path where to save the texture images.
 * @param[in] textureTypesToSave: A BitSet indicating which texture types to
 * save. By default, all texture types are saved.
 * @param[in] log: The logger to use to log messages.
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveTextureImages(
    const MeshType&    mesh,
    const std::string& basePath,
    const BitSet8&     textureTypesToSave = BitSet8::ALL(),
    LogType&           log = nullLogger) requires HasMaterials<MeshType>
{
    // to avoid saving the same image multiple times
    std::set<std::string> savedImages;

    for (const Material& mat : mesh.materials()) {
        using enum Material::TextureType;
        const uint N_TEXTURE_TYPES = toUnderlying(COUNT);
        for (uint i = 0; i < N_TEXTURE_TYPES; ++i) {
            // supported textures to save
            if (textureTypesToSave[i]) {
                const TextureDescriptor& t   = mat.textureDescriptor(i);
                if (!savedImages.contains(t.path())) {
                    const Image&             img = mesh.textureImage(t.path());
                    if (t.isNull()) {
                        log.log(
                            "Cannot save empty texture " + t.path(),
                            LogType::WARNING_LOG);
                    }
                    else {
                        saveImage(img, basePath + t.path());
                        savedImages.insert(t.path());
                    }
                }
            }
        }
    }
}

} // namespace vcl

#endif // VCL_IO_IMAGE_SAVE_H
