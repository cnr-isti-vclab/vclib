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

#ifndef MATERIAL_TRI_MESH_H
#define MATERIAL_TRI_MESH_H

#include <vclib/meshes.h>

// This is a TriMesh class on which materials replace the texture images.

template<typename Scalar>
class MaterialTriMeshT :
        public vcl::Mesh<
            vcl::mesh::VertexContainer<vcl::trimesh::Vertex<Scalar, false>>,
            vcl::mesh::FaceContainer<vcl::trimesh::Face<Scalar, false>>,
            vcl::mesh::BoundingBox3<Scalar>,
            vcl::mesh::Color,
            vcl::mesh::Mark,
            vcl::mesh::Name,
            vcl::mesh::Materials, // This is the key addition for materials
            vcl::mesh::TransformMatrix<Scalar>,
            vcl::mesh::CustomComponents>
{
public:
    /** @brief The scalar used to store all the data of the Mesh. */
    using ScalarType = Scalar;
};

using MaterialTriMesh = MaterialTriMeshT<double>;

using MaterialTriMeshf = MaterialTriMeshT<float>;

#endif // MATERIAL_TRI_MESH_H
