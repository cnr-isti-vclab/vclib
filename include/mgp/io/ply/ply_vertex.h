/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_PLY_VERTEX_H
#define MGP_PLY_VERTEX_H

#include "ply_header.h"
#include "../file_mesh_info.h"
#include <fstream>

namespace mgp {
namespace ply {

namespace internal {

template <typename MeshType>
void loadVerticesTxt(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& mesh);

template <typename MeshType>
void loadVerticesBin(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& mesh);

} //namespace mgp::ply::internal

template <typename MeshType>
void saveVertices(
	std::ofstream& file,
	const PlyHeader& header,
	const MeshType& mesh);

template <typename MeshType>
void loadVertices(
		std::ifstream& file,
		const PlyHeader& header,
		MeshType& mesh);

} //namespace mgp::ply
} //namespace mgp

#include "ply_vertex.cpp"

#endif // MGP_PLY_VERTEX_H
