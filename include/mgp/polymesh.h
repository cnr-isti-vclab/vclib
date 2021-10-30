/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POLYMESH_H
#define MGP_POLYMESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace mgp::polymesh {

class Vertex;
class Face;

class Vertex : public mgp::Vertex<mgp::vert::BitFlags, mgp::vert::Coordinate3d>
{
};

class Face :
		public mgp::Face<
			mgp::face::BitFlags, // 4b
			mgp::face::VertexReferences<Vertex, -1>>
{
};

} // namespace mgp::polymesh

namespace mgp {

class PolyMesh : public mgp::Mesh<polymesh::Vertex, polymesh::Face>
{
public:
	using Vertex = polymesh::Vertex;
	using Face   = polymesh::Face;
};

} // namespace mgp

#endif // MGP_POLYMESH_H
