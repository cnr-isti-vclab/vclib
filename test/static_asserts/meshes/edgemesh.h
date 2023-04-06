#ifndef EDGEMESH_H
#define EDGEMESH_H

#include "mesh_views.h"

#include <vclib/mesh/edge_mesh.h>

void edgemeshStaticAsserts()
{
	using namespace vcl;

	// mesh concepts
	static_assert(MeshConcept<EdgeMesh>, "The EdgeMesh is not a valid Mesh.");
	static_assert(!FaceMeshConcept<EdgeMesh>, "The EdgeMesh is a valid Mesh with Faces.");
	static_assert(!TriangleMeshConcept<EdgeMesh>, "The EdgeMesh is a static Triangle Mesh.");
	static_assert(!QuadMeshConcept<EdgeMesh>, "The EdgeMesh is a static Quad Mesh.");
	static_assert(EdgeMeshConcept<EdgeMesh>, "The EdgeMesh is not a valid Edge Mesh.");
	static_assert(!PolygonMeshConcept<EdgeMesh>, "The EdgeMesh is a PolygonMesh.");
	static_assert(!DcelMeshConcept<EdgeMesh>, "The EdgeMesh is a DcelMesh.");

	// mesh views
	meshViewsStaticAsserts<EdgeMesh>();
}

#endif // EDGEMESH_H
