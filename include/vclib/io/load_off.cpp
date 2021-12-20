/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

#include "load_off.h"

#include <vclib/exception/io_exception.h>
#include <vclib/misc/tokenizer.h>

#include "off/off.h"

namespace vcl::io {

namespace internal {

template <typename MeshType>
void loadOffVertices(
	MeshType& mesh,
	std::ifstream& file,
	const FileMeshInfo& fileInfo,
	uint nv)
{
	using VertexType = typename MeshType::VertexType;

	const uint nTexCoords = fileInfo.hasVertexTexCoords() ? 2 : 0;

	mesh.addVertices(nv);
	for (uint i = 0; i < nv; i++) {
		VertexType& v = mesh.vertex(i);

		vcl::Tokenizer           tokens = internal::nextNonEmptyTokenizedLine(file);
		vcl::Tokenizer::iterator token  = tokens.begin();

		// Read 3 vertex coordinates
		for (unsigned int j = 0; j < 3; j++) {
			// Read vertex coordinate
			v.coord()[j] = io::internal::readDouble<double>(token);
		}

		if constexpr(vcl::hasPerVertexNormal<MeshType>()) {
			if (vcl::isPerVertexNormalEnabled(mesh) && fileInfo.hasVertexNormals()) {
				// Read 3 normal coordinates
				for (unsigned int j = 0; j < 3; j++) {
					v.normal()[j] = io::internal::readDouble<double>(token);
				}
			}
		}
		else if (fileInfo.hasVertexNormals()){ // need to read and throw away data
			for (unsigned int j = 0; j < 3; j++) {
				io::internal::readDouble<double>(token);
			}
		}

		const uint nReadComponents = token - tokens.begin();
		const int nColorComponents = (int)tokens.size() - nReadComponents - nTexCoords;

		if constexpr(vcl::hasPerVertexColor<MeshType>()) {
			if (vcl::isPerVertexColorEnabled(mesh) && fileInfo.hasVertexColors()) {
				if (nColorComponents != 1 && nColorComponents != 3 && nColorComponents != 4)
					throw MalformedFileException("Wrong number of components in line.");
				v.color() = off::loadColor(token, nColorComponents);
			}
		}
		else if (fileInfo.hasVertexColors()){ // need to read and throw away data
			if (nColorComponents != 1 && nColorComponents != 3 && nColorComponents != 4)
				throw MalformedFileException("Wrong number of components in line.");
			off::loadColor(token, nColorComponents);
		}

		if constexpr(vcl::hasPerVertexTexCoord<MeshType>()) {
			if (vcl::isPerVertexTexCoordEnabled(mesh) && fileInfo.hasVertexTexCoords()) {
				// Read 2 tex coordinates
				for (unsigned int j = 0; j < 2; j++) {
					v.texCoord()[j] = io::internal::readDouble<double>(token);
				}
			}
		}
		else if (fileInfo.hasVertexTexCoords()){ // need to read and throw away data
			for (unsigned int j = 0; j < 2; j++) {
				io::internal::readDouble<double>(token);
			}
		}
	}
}

template <typename MeshType>
void loadOffFaces(MeshType& mesh, std::ifstream& file, FileMeshInfo& loadedInfo, uint nf)
{
}

} // namespace internal

template<typename MeshType>
MeshType loadOff(const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	return loadOff<MeshType>(filename, loadedInfo, enableOptionalComponents);
}

template<typename MeshType>
MeshType
loadOff(const std::string& filename, FileMeshInfo& loadedInfo, bool enableOptionalComponents)
{
	MeshType m;
	loadOff(m, filename, loadedInfo, enableOptionalComponents);
	return m;
}

template<typename MeshType>
void loadOff(MeshType& m, const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	loadOff(m, filename, loadedInfo, enableOptionalComponents);
}

template<typename MeshType>
void loadOff(
	MeshType&          m,
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	bool               enableOptionalComponents)
{
	std::ifstream file = internal::loadFileStream(filename);
	uint nVertices, nFaces, nEdges;
	FileMeshInfo fileInfo; // data that needs to be read from the file
	off::loadOffHeader(file, fileInfo, nVertices, nFaces, nEdges);
	loadedInfo = fileInfo; // data that will be stored in the mesh!
	if (enableOptionalComponents)
		internal::enableOptionalComponents(loadedInfo, m);

	internal::loadOffVertices(m, file, fileInfo, nVertices);
	internal::loadOffFaces(m, file, fileInfo, nFaces);
	// internal::loadOffEdges(m, file, loadedInfo, nEdges);
}

} // namespace vcl::io
