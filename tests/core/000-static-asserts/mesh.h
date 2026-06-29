// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
