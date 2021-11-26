/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_MESH_REQUIREMENTS_H
#define VCL_MESH_MESH_REQUIREMENTS_H

#include "face_requirements.h"
#include "vertex_requirements.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

// Triangles

template<typename MeshType>
bool constexpr hasTriangles();

template<typename MeshType>
bool isTriangleMesh(const MeshType&);

// Quads

template<typename MeshType>
bool constexpr hasQuads();

template<typename MeshType>
bool isQuadMesh(const MeshType&);

// Bounding Box

template<typename MeshType>
bool constexpr hasBoundingBox();

// Mark

template<typename MeshType>
bool constexpr hasMark();

// Transform Matrix

template<typename MeshType>
bool constexpr hasTransformMatrix();

/*********************
 * require functions *
 *********************/

// Triangles

template<typename MeshType>
void requireTriangleMesh(const MeshType&);

// Quads

template<typename MeshType>
void requireQuadMesh(const MeshType&);

// Bounding Box

template<typename MeshType>
void constexpr requireBoundingBox();

// Mark

template<typename MeshType>
void constexpr requireMark();

// Transform Matrix

template<typename MeshType>
void constexpr requireTransformMatrix();

} // namespace vcl

#include "mesh_requirements.cpp"

#endif // VCL_MESH_MESH_REQUIREMENTS_H
