// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_CAMERA_LOAD_H
#define VCL_IO_CAMERA_LOAD_H

#ifdef VCLIB_WITH_TINYGLTF
#include "gltf/load.h"
#endif

#include <vclib/io/file_format.h>

#ifdef VCLIB_WITH_TINYGLTF
#include <vclib/io/mesh/gltf/capability.h>
#endif

#include <vclib/space/core.h>

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
std::vector<CameraType> loadCameras(const std::string& filename)
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
CameraType loadCamera(const std::string& filename)
{
    auto cams = loadCameras<CameraType>(filename);
    if (cams.empty())
        throw std::runtime_error("No cameras in file: " + filename);
    return cams.front();
}

} // namespace vcl

#endif // VCL_IO_CAMERA_LOAD_H
