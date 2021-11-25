/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "save_ply.h"

namespace vcl::io {

template<typename MeshType>
void savePly(const MeshType& m, const std::string& filename, bool binary)
{
	vcl::io::FileMeshInfo info(m);
	savePly(m, filename, info, binary);
}

template<typename MeshType>
void savePly(const MeshType& m, const std::string& filename, const FileMeshInfo& info, bool binary)
{
	ply::PlyHeader header(binary ? ply::BINARY : ply::ASCII, info);
	header.setNumberVertices(m.vertexNumber());
	header.setNumberFaces(m.faceNumber());

	savePly(m, filename, header);
}

template<typename MeshType>
void savePly(const MeshType& m, const std::string& filename, const ply::PlyHeader& header)
{
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	if (!header.isValid())
		throw std::runtime_error("Ply Header not valid.");

	std::string plyfilename;
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	std::ofstream fp;
	size_t        lastindex = filename.find_last_of(".");
	if (lastindex != filename.size())
		plyfilename = filename;
	else
		plyfilename = filename + ".ply";

	fp.open(plyfilename, std::ofstream::binary); // need to set binary of windows will fail
	if (!fp) {
		throw vcl::CannotOpenFileException(plyfilename);
	}
	fp << header.toString();

	if (vcl::hasVertices<MeshType>())
		ply::saveVertices(fp, header, m);

	if (vcl::hasFaces<MeshType>())
		ply::saveFaces(fp, header, m);

	fp.close();
}

} // namespace vcl::io
