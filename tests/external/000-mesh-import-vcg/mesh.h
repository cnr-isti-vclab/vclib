// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MESH_H
#define MESH_H

#include <vcg/complex/complex.h>

class VCGEdge;
class VCGFace;
class VCGVertex;

struct VCGUsedTypes :
        public vcg::UsedTypes<
            vcg::Use<VCGVertex>::AsVertexType,
            vcg::Use<VCGFace>::AsFaceType>
{
};

class VCGVertex :
        public vcg::Vertex<
            VCGUsedTypes,
            vcg::vertex::BitFlags,
            vcg::vertex::Coord3f,
            vcg::vertex::Normal3f>
{
};

class VCGFace :
        public vcg::Face<
            VCGUsedTypes,
            vcg::face::BitFlags,
            vcg::face::VertexRef,
            vcg::face::Normal3f>
{
};

class VCGMesh :
        public vcg::tri::TriMesh<std::vector<VCGVertex>, std::vector<VCGFace>>
{
};

#endif // MESH_H
