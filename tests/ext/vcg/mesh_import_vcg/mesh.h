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
