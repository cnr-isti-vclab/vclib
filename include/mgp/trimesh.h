/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_TRIMESH_H
#define MGP_TRIMESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace mgp::trimesh {

class Vertex;
class Face;

class Vertex :
		public mgp::Vertex<
			mgp::vert::BitFlags,                        // 4b
			mgp::vert::Coordinate3d,                    // 24b
			mgp::vert::Normal3d,                        // 24b
			mgp::vert::Color,                           // 4b
			mgp::vert::Scalard,                         // 8b
			mgp::vert::OptionalMutableBitFlags<Vertex>, // 0b
			mgp::vert::AdjacentFaces<Face>,             // N*8b
			mgp::vert::CustomComponents<Vertex>>        // 0b
{
};

class Face :
		public mgp::Face<
			mgp::face::TriangleBitFlags,           // 4b
			mgp::face::TriangleVertexRefs<Vertex>, // 24b
			mgp::face::Normal3d,                   // 24b
			mgp::face::OptionalScalard<Face>,      // 0b
			mgp::face::OptionalColor<Face>,        // 0b
			mgp::face::CustomComponents<Face>>     // 0b
{
};

} // namespace mgp::trimesh

namespace mgp {

class TriMesh : public mgp::Mesh<
		trimesh::Vertex,
		trimesh::Face,
		mesh::BoundingBox3d>
{
public:
	using Vertex = trimesh::Vertex;
	using Face   = trimesh::Face;
};

} // namespace mgp

#endif // MGP_TRIMESH_H
