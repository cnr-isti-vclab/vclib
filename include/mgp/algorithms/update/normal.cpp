/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#include "normal.h"

#include <mgp/mesh/requirements.h>

namespace mgp {

template<typename Triangle>
double triangleNormal(const Triangle& t)
{
	return (t.v(1).coordinate() - t.v(0).coordinate)
		.cross(t.v(2).coordinate() - t.v(0).coordinate());
}

template<typename Polygon>
double polygonNormal(const Polygon& p)
{
	// todo
}

template<typename MeshType>
void normalizePerFaceNormals(MeshType& m)
{
	mgp::requireFaces<MeshType>();
	mgp::requirePerFaceNormal(m);

	using FaceType = typename MeshType::Face;

	for (FaceType& f : m.faceIterator()) {
		f.normal().normalize();
	}
}

template<typename MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize)
{
	mgp::requireVertices<MeshType>();
	mgp::requireFaces<MeshType>();
	mgp::requirePerFaceNormal(m);

	using FaceType = typename MeshType::Face;
	if constexpr (mgp::hasTriangles<MeshType>()) {
		for (FaceType& f : m.faceIterator()) {
			f.normal() = triangleNormal(f);
		}
	}
	else {
		for (FaceType& f : m.faceIterator()) {
			f.normal() = polygonNormal(f);
		}
	}
	if (normalize)
		normalizePerFaceNormals(m);
}

} // namespace mgp
