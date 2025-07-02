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

#ifndef BAR_MESH_H
#define BAR_MESH_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

#include "bar_component.h"

/*
 * This file defines a BarMesh class that uses the optional BarComponent in the
 * Vertex Element.
 */

class BarMesh;

namespace barmesh {

class Vertex;

class Face;

class Vertex :
        public vcl::Vertex<
            BarMesh,
            vcl::vert::BitFlags,
            vcl::vert::Position3d,
            vcl::vert::Normal3d,
            vcl::vert::Color,
            OptionalBarComponent<Vertex>> // the optional BarComponent
{
};

class Face :
        public vcl::Face<
            BarMesh,
            vcl::face::TriangleBitFlags,
            vcl::face::TriangleVertexPtrs<Vertex, Face>,
            vcl::face::Normal3d>
{
};

} // namespace barmesh

class BarMesh :
        public vcl::Mesh<
            vcl::mesh::VertexContainer<barmesh::Vertex>,
            vcl::mesh::FaceContainer<barmesh::Face>,
            vcl::mesh::BoundingBox3d>
{
public:
    using ScalarType = double;
};

#endif // BAR_MESH_H
