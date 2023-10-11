#ifndef POLYMESH_H
#define POLYMESH_H

#include "mesh_views.h"

#include <vclib/meshes/poly_mesh.h>

void polymeshStaticAsserts()
{
    using namespace vcl;

    static_assert(MeshConcept<PolyMesh>, "The PolyMesh is not a valid Mesh.");
    static_assert(
        FaceMeshConcept<PolyMesh>,
        "The PolyMesh is not a valid Mesh with Faces.");
    static_assert(
        !TriangleMeshConcept<PolyMesh>,
        "The PolyMesh is a static Triangle Mesh.");
    static_assert(
        !QuadMeshConcept<PolyMesh>, "The PolyMesh is a static Quad Mesh.");
    static_assert(!EdgeMeshConcept<PolyMesh>, "The PolyMesh is an Edge Mesh.");
    static_assert(
        PolygonMeshConcept<PolyMesh>,
        "The PolyMesh is not a valid Polygon Mesh.");

    static_assert(HasVertices<PolyMesh>, "");
    static_assert(HasFaces<PolyMesh>, "");
    static_assert(!HasEdges<PolyMesh>, "");

    static_assert(!HasTriangles<PolyMesh>, "");
    static_assert(!HasQuads<PolyMesh>, "");
    static_assert(HasPolygons<PolyMesh>, "");

    static_assert(
        comp::IsTiedToVertexNumber<PolyMesh::Face::AdjacentFaces>, "");
    static_assert(
        !comp::IsTiedToVertexNumber<PolyMesh::Vertex::AdjacentFaces>, "");

    meshViewsStaticAsserts<PolyMesh>();
}

#endif // POLYMESH_H
