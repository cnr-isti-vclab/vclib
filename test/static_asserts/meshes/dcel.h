#ifndef DCEL_H
#define DCEL_H

#include "mesh_views.h"

#include <vclib/meshes/dcel_mesh.h>

void dcelStaticAsserts()
{
	using namespace vcl;

	static_assert(MeshConcept<DcelMesh>, "The DcelMesh is not a valid Mesh.");
	static_assert(FaceMeshConcept<DcelMesh>, "The DcelMesh is not a valid Mesh with Faces.");
	static_assert(!TriangleMeshConcept<DcelMesh>, "The DcelMesh is a static Triangle Mesh.");
	static_assert(!QuadMeshConcept<DcelMesh>, "The DcelMesh is a static Quad Mesh.");
	static_assert(!EdgeMeshConcept<DcelMesh>, "The DcelMesh is a Edge Mesh.");
	static_assert(PolygonMeshConcept<DcelMesh>, "The DcelMesh is not a PolygonMesh.");
	static_assert(DcelMeshConcept<DcelMesh>, "The DcelMesh is not a valid DcelMesh.");

	static_assert(HasVertices<DcelMesh>, "");
	static_assert(HasFaces<DcelMesh>, "");
	static_assert(!HasEdges<DcelMesh>, "");
	static_assert(HasHalfEdges<DcelMesh>, "");

	static_assert(!HasTriangles<DcelMesh>, "");
	static_assert(!HasQuads<DcelMesh>, "");
	static_assert(HasPolygons<DcelMesh>, "");

	meshViewsStaticAsserts<DcelMesh>();
}

#endif // DCEL_H
