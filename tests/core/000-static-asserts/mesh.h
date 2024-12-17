/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef MESH_H
#define MESH_H

#include "mesh/components.h"
#include "mesh/containers.h"
#include "mesh/elements.h"
#include "mesh/mesh_concept.h"
#include "mesh/per_edge.h"
#include "mesh/per_face.h"
#include "mesh/per_mesh.h"
#include "mesh/per_vertex.h"

void meshStaticAsserts()
{
    componentsStaticAsserts();
    elementsStaticAsserts();
    containersStaticAsserts();

    meshConceptStaticAsserts();

    perEdgeStaticAsserts();
    perFaceStaticAsserts();
    perMeshStaticAsserts();
    perVertexStaticAsserts();
}

#endif // MESH_H
