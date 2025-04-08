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

#ifndef VCL_IO_MESH_PLY_CAPABILITY_H
#define VCL_IO_MESH_PLY_CAPABILITY_H

#include <vclib/io/file_format.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl {

constexpr FileFormat plyFileFormat()
{
    return FileFormat("ply", "PLY Polygon File Format .ply");
}

inline MeshInfo plyFormatCapability()
{
    MeshInfo info;

    info.setPolygonMesh();

    info.setVertices();
    info.setFaces();
    info.setEdges();

    info.setTextures();

    info.setVertexCoords();
    info.setVertexNormals();
    info.setVertexColors();
    info.setVertexQuality();
    info.setVertexTexCoords();
    info.setVertexCustomComponents();

    info.setFaceVRefs();
    info.setFaceColors();
    info.setFaceNormals();
    info.setFaceQuality();
    info.setFaceWedgeTexCoords();
    info.setFaceCustomComponents();

    info.setEdgeVRefs();
    info.setEdgeColors();

    return info;
}

} // namespace vcl

#endif // VCL_IO_MESH_PLY_CAPABILITY_H
