#ifndef DCEL_H
#define DCEL_H

#include "mesh_views.h"

#include <vclib/mesh/dcel_mesh.h>

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

	meshViewsStaticAsserts<DcelMesh>();
}

#endif // DCEL_H
