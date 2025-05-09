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

#ifndef VCL_IO_MESH_OBJ_CAPABILITY_H
#define VCL_IO_MESH_OBJ_CAPABILITY_H

#include <vclib/io/file_format.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl {

constexpr FileFormat objFileFormat()
{
    return FileFormat("obj", "OBJ Wavefront .obj");
}

inline MeshInfo objFormatCapability()
{
    MeshInfo info;

    info.setPolygonMesh();

    info.setVertices();
    info.setFaces();
    info.setEdges();

    info.setTextures();

    info.setPerVertexCoordinate();
    info.setPerVertexNormal();
    info.setPerVertexColor();
    info.setPerVertexTexCoord();

    info.setPerFaceVertexReferences();
    info.setPerFaceColor();
    info.setPerFaceWedgeTexCoords();

    info.setPerEdgeVertexReferences();
    info.setPerEdgeColor();

    return info;
}

} // namespace vcl

#endif // VCL_IO_MESH_OBJ_CAPABILITY_H
