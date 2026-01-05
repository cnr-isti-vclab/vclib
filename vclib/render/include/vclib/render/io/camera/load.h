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

#ifndef VCL_RENDER_IO_CAMERA_LOAD_H
#define VCL_RENDER_IO_CAMERA_LOAD_H

#ifdef VCLIB_WITH_TINYGLTF
#include "gltf/load.h"
#endif

#include <vclib/io/file_format.h>
#include <vclib/io/mesh/gltf/capability.h>

#include <set>
#include <string>
#include <vector>

namespace vcl {

/**
 * @brief Returns the set of camera formats supported for loading.
 *
 * The set contains all the camera formats that can be loaded using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of camera formats supported for loading.
 */
inline std::set<FileFormat> loadCameraFormats()
{
    std::set<FileFormat> ff;

#ifdef VCLIB_WITH_TINYGLTF
    ff.insert(gltfFileFormat());
#endif

    return ff;
}

template<CameraConcept CameraType = Camera<float>>
inline std::vector<CameraType> loadCameras(const std::string& filename)
{
    FileFormat ff = FileInfo::fileFormat(filename);
#ifdef VCLIB_WITH_TINYGLTF
    if (ff == gltfFileFormat())
        return loadCamerasGltf<CameraType>(filename);
    else
#endif
        throw UnknownFileFormatException(ff.extensions().front());
}

template<CameraConcept CameraType = Camera<float>>
inline CameraType loadCamera(const std::string& filename)
{
    auto cams = loadCameras<CameraType>(filename);
    if (cams.empty())
        throw std::runtime_error("No cameras in file: " + filename);
    return cams.front();
}

} // namespace vcl

#endif // VCL_RENDER_IO_CAMERA_LOAD_H
