// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_STL_CAPABILITY_H
#define VCL_IO_MESH_STL_CAPABILITY_H

#include <vclib/io/file_format.h>

#include <vclib/space/complex.h>

namespace vcl {

constexpr FileFormat stlFileFormat()
{
    return FileFormat("stl", "STL Stereolithography .stl");
}

inline MeshInfo stlFormatCapability()
{
    MeshInfo info;

    info.setTriangleMesh();

    info.setVertices();

    info.setPerVertexPosition();

    info.setFaces();

    info.setPerFaceVertexReferences();
    info.setPerFaceColor();
    info.setPerFaceNormal();

    return info;
}

} // namespace vcl

#endif // VCL_IO_MESH_STL_CAPABILITY_H
