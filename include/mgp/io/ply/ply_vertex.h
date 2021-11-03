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

template <typename A, typename B, typename C>
bool loadVerticesTxt(
		std::ifstream& file,
		const PlyHeader& header,
		A vertices[],
		B vertexNormals[],
		io::FileMeshInfo::ColorMode colorMod ,
		C vertexColors[]);

template <typename A, typename B, typename C>
bool loadVerticesBin(
		std::ifstream& file,
		const PlyHeader& header,
		A vertices[],
		B vertexNormals[],
		io::FileMeshInfo::ColorMode colorMod ,
		C vertexColors[]);

} //namespace mgp::ply::internal

template <typename A, typename B, typename C>
void saveVertices(
		std::ofstream& file,
		const PlyHeader& header,
		const A vertices[],
		const B vertexNormals[],
		io::FileMeshInfo::ColorMode colorMod ,
		const C vertexColors[]);

template <typename A, typename B, typename C>
bool loadVertices(
		std::ifstream& file,
		const PlyHeader& header,
		A vertices[],
		B vertexNormals[],
		io::FileMeshInfo::ColorMode colorMod ,
		C vertexColors[]);

} //namespace mgp::ply
} //namespace mgp

#include "ply_vertex.cpp"

#endif // MGP_PLY_VERTEX_H
