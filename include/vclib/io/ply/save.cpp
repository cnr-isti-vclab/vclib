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

#include "save.h"

#include "../internal/io_utils.h"

namespace vcl::io {

template<MeshConcept MeshType, LoggerConcept LogType>
void savePly(const MeshType& m, const std::string& filename, LogType& log, bool binary)
{
	MeshInfo info(m);
	savePly(m, filename, info, log, binary);
}

template<MeshConcept MeshType, LoggerConcept LogType>
void savePly(
	const MeshType&    m,
	const std::string& filename,
	const MeshInfo&    info,
	LogType&           log,
	bool               binary)
{
	MeshInfo meshInfo(m);

	// make sure that the given info contains only components that are actually available in the
	// mesh. meshInfo will contain the intersection between the components that the user wants to
	// save and the components that are available in the mesh.
	meshInfo = info.intersect(meshInfo);

	ply::PlyHeader header(binary ? ply::BINARY : ply::ASCII, meshInfo);
	header.setNumberVertices(m.vertexNumber());

	if constexpr (vcl::HasFaces<MeshType>) {
		if (header.hasFaces()) {
			header.setNumberFaces(m.faceNumber());
		}
	}
	if constexpr (vcl::HasEdges<MeshType>) {
		if (header.hasEdges()) {
			header.setNumberEdges(m.edgeNumber());
		}
	}
	ply::saveTextures(header, m);

	if (!header.isValid())
		throw std::runtime_error("Ply Header not valid.");

	std::ofstream fp = internal::saveFileStream(filename, "ply");

	fp << header.toString();

	ply::saveVertices(fp, header, m);

	if constexpr (vcl::HasFaces<MeshType>) {
		if (header.hasFaces()) {
			ply::saveFaces(fp, header, m);
		}
	}

	if constexpr (vcl::HasEdges<MeshType>) {
		if (header.hasEdges()) {
			ply::saveEdges(fp, header, m);
		}
	}

	fp.close();
}

} // namespace vcl::io
