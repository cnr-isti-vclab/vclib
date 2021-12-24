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

#include "load_obj.h"

#include <map>

#include "internal/io_read.h"
#include "internal/io_utils.h"
#include "obj/material.h"

namespace vcl::io {

template<typename MeshType>
MeshType loadObj(const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	return loadObj<MeshType>(filename, loadedInfo, enableOptionalComponents);
}

template<typename MeshType>
MeshType
loadObj(const std::string& filename, FileMeshInfo& loadedInfo, bool enableOptionalComponents)
{
	MeshType m;
	loadObj(m, filename, loadedInfo, enableOptionalComponents);
	return m;
}

template<typename MeshType>
void loadObj(MeshType& m, const std::string& filename, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	loadObj(m, filename, loadedInfo, enableOptionalComponents);
}

template<typename MeshType>
void loadObj(
	MeshType&          m,
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	bool               enableOptionalComponents)
{
	std::ifstream file = internal::loadFileStream(filename);
	do {
		uint vn = 0;
		std::map<uint, vcl::Point3d> mapNormalsCache;
		std::vector<vcl::TexCoordd> wedgeTexCoords;

		vcl::Tokenizer tokens = internal::nextNonEmptyTokenizedLineNoThrow(file);
		if (file) {
			vcl::Tokenizer::iterator token = tokens.begin();
			std::string header = *token++;
			if (header == "v") {
				// first, need to set that I'm loading vertices
				if (m.vertexNumber() == 0) {
					loadedInfo.setVertices();
					loadedInfo.setVertexCoords();
				}
				uint vid = m.addVertex();
				for (uint i = 0; i < 3; ++i) {
					m.vertex(vid).coord()[i] = internal::readDouble<double>(token);
				}
			}
			if constexpr(hasPerVertexNormal<MeshType>()) {
				if (header == "vn") {
					// first, need to check if I can store normals in the mesh
					if (vn == 0) {
						if (enableOptionalComponents){
							enableIfPerVertexNormalOptional(m);
							loadedInfo.setVertexNormals();
						}
						else {
							if (isPerVertexNormalEnabled(m))
								loadedInfo.setVertexNormals();
						}
					}
					// I can store the normal in its vertex
					if (m.vertexNumber() > vn) {
						for (uint i = 0; i < 3; ++i) {
							m.vertex(vn).normal()[i] = internal::readDouble<double>(token);
						}
					}
					else {
						Point3d n;
						for (uint i = 0; i < 3; ++i) {
							n[i] = internal::readDouble<double>(token);
						}
						mapNormalsCache[vn] = n;
					}
					vn++;
				}
			}
			if (header == "vt") {
				// save the texcoord for later
				TexCoordd tf;
				for (uint i = 0; i < 2; ++i) {
					tf[i] = internal::readDouble<double>(token);
				}
				wedgeTexCoords.push_back(tf);
			}
			if (header == "f") {

			}
		}
	} while (file);

}

} // namespace vcl::io
