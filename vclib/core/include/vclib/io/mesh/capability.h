// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_CAPABILITY_H
#define VCL_IO_MESH_CAPABILITY_H

#include "obj/capability.h"
#include "off/capability.h"
#include "ply/capability.h"
#include "stl/capability.h"

#ifdef VCLIB_WITH_TINYGLTF
#include "gltf/capability.h"
#endif

#include <vclib/io/exceptions.h>

namespace vcl {

inline MeshInfo formatCapability(const std::string& format)
{
    std::string ext = toLower(format);
    if (ext == "obj") {
        return objFormatCapability();
    }
    else if (ext == "off") {
        return offFormatCapability();
    }
    else if (ext == "ply") {
        return plyFormatCapability();
    }
    else if (ext == "stl") {
        return stlFormatCapability();
    }
#ifdef VCLIB_WITH_TINYGLTF
    else if (ext == "gltf" || ext == "glb") {
        return gltfFormatCapability();
    }
#endif
    else {
        throw UnknownFileFormatException(ext);
    }
}

} // namespace vcl

#endif // VCL_IO_MESH_CAPABILITY_H
