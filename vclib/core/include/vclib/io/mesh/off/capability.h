// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_OFF_CAPABILITY_H
#define VCL_IO_MESH_OFF_CAPABILITY_H

#include <vclib/io/file_format.h>

#include <vclib/space/complex.h>

namespace vcl {

constexpr FileFormat offFileFormat()
{
    return FileFormat("off", "OFF Object File Format .off");
}

inline MeshInfo offFormatCapability()
{
    MeshInfo info;

    info.setPolygonMesh();

    info.setVertices();
    info.setFaces();

    info.setPerVertexPosition();
    info.setPerVertexNormal();
    info.setPerVertexColor();
    info.setPerVertexTexCoord();

    info.setPerFaceVertexReferences();
    info.setPerFaceColor();

    return info;
}

} // namespace vcl

#endif // VCL_IO_MESH_OFF_CAPABILITY_H
