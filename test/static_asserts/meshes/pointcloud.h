#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "mesh_views.h"

#include <vclib/meshes/point_cloud.h>

void pointcloudStaticAsserts()
{
	using namespace vcl;

	// makes sure that the PointCloud satisfies Mesh concepts
	static_assert(MeshConcept<PointCloud>, "The PointCloud is not a valid Mesh.");
	static_assert(!FaceMeshConcept<PointCloud>, "The PointCloud is a valid Mesh with Faces.");
	static_assert(!TriangleMeshConcept<PointCloud>, "The PointCloud is a static Triangle Mesh.");
	static_assert(!QuadMeshConcept<PointCloud>, "The PointCloud is a static Quad Mesh.");
	static_assert(!EdgeMeshConcept<PointCloud>, "The PointCloud is an Edge Mesh.");
	static_assert(!PolygonMeshConcept<PointCloud>, "The PointCloud is a PolygonMesh.");
	static_assert(!DcelMeshConcept<PointCloud>, "The PointCloud is a DcelMesh.");

	static_assert(HasVertices<PointCloud>, "");
	static_assert(!HasFaces<PointCloud>, "");
	static_assert(!HasEdges<PointCloud>, "");
	static_assert(!HasHalfEdges<PointCloud>, "");

	static_assert(!HasTriangles<PointCloud>, "");
	static_assert(!HasQuads<PointCloud>, "");
	static_assert(!HasPolygons<PointCloud>, "");

	// mesh views
	meshViewsStaticAsserts<PointCloud>();
}

#endif // POINTCLOUD_H
