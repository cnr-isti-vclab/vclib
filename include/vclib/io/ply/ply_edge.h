/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_IO_PLY_EDGE_H
#define VCL_IO_PLY_EDGE_H

#include <fstream>
#include <vclib/misc/tokenizer.h>

#include "ply_header.h"
#include "../internal/io_write.h"

namespace vcl::io::ply {

template<EdgeMeshConcept MeshType>
void saveEdges(
	std::ofstream&   file,
	const PlyHeader& header,
	const MeshType&  mesh)
{
	using EdgeType = MeshType::EdgeType;
	bool bin = header.format() == ply::BINARY;

	// indices of vertices that do not consider deleted vertices
	std::vector<int> vIndices = mesh.vertexCompactIndices();

	for (const EdgeType& e : mesh.edges()) {
		for (const ply::Property& p : header.edgeProperties()) {
			bool hasBeenWritten = false;
			if (p.name == ply::vertex1) {
				io::internal::writeProperty(
					file, vIndices[mesh.index(e.vertex(0))], p.type, bin);
				hasBeenWritten = true;
			}
			if (p.name == ply::vertex2) {
				io::internal::writeProperty(
					file, vIndices[mesh.index(e.vertex(1))], p.type, bin);
				hasBeenWritten = true;
			}
			if (!hasBeenWritten) {
				// be sure to write something if the header declares some
				// property that is not in the mesh
				io::internal::writeProperty(file, 0, p.type, bin);
			}
		}
	}
}

} // namespace vcl::ply

#endif // VCL_IO_PLY_EDGE_H
