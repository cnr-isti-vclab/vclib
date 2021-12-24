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

#include <vclib/algorithms/polygon.h>

#include "internal/io_read.h"
#include "internal/io_utils.h"
#include "obj/material.h"

namespace vcl::io {

namespace internal {

template<typename MeshType>
void loadVertexNormal(
	MeshType&                    m,
	std::map<uint, vcl::Point3d> mapNormalsCache,
	uint                         vn,
	vcl::Tokenizer::iterator&    token,
	FileMeshInfo&                loadedInfo,
	bool                         enableOptionalComponents)
{
	// first, need to check if I can store normals in the mesh
	if (vn == 0) {
		if (enableOptionalComponents) {
			enableIfPerVertexNormalOptional(m);
			loadedInfo.setVertexNormals();
		}
		else {
			if (isPerVertexNormalEnabled(m))
				loadedInfo.setVertexNormals();
		}
	}
	if (loadedInfo.hasVertexNormals()) {
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
	}
}

template<typename MeshType>
void loadFace(
	MeshType& m,
	const vcl::Tokenizer& tokens,
	const std::vector<vcl::TexCoordd>& wedgeTexCoords,
	FileMeshInfo&         loadedInfo,
	bool                  enableOptionalComponents)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<uint> vids;
	std::vector<uint> wids;

	vcl::Tokenizer::iterator token = tokens.begin();
	++token;
	for (uint i = 0; i < tokens.size()-1; ++i) {
		vcl::Tokenizer subt(*token, '/', false);
		auto t = subt.begin();
		vids[i] = readUInt<uint>(t) - 1;
		if (subt.size() > 1) {
			if (!t->empty()) {
				wids[i] = readUInt<uint>(t) - 1;
			}
		}
	}

	uint fid = m.addFace();
	FaceType& f = m.face(fid);

	bool splitFace = false;
	// we have a polygonal mesh
	if constexpr (FaceType::VERTEX_NUMBER < 0) {
		f.resizeVertices(tokens.size()-1); // need to resize to the right number of verts
	}
	else if (FaceType::VERTEX_NUMBER != tokens.size()-1) {
		// we have faces with static sizes (triangles), but we are loading faces with
		// number of verts > 3. Need to split the face we are loading in n faces!
		splitFace = true;
	}
	if (!splitFace) {
		for (uint i = 0; i < vids.size(); ++i) {
			if (vids[i] >= m.vertexNumber()) {
				throw vcl::MalformedFileException(
					"Bad vertex index for face " + std::to_string(i));
			}
			f.vertex(i) = &m.vertex(vids[i]);
		}
	}
	else { // split needed
		addTriangleFacesFromPolygon(m, f, vids);
	}
}

} // namespace internal

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
	std::map<uint, vcl::Point3d> mapNormalsCache;
	do {
		uint vn = 0;
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
					internal::loadVertexNormal(
						m, mapNormalsCache, vn, token, loadedInfo, enableOptionalComponents);
					vn++;
				}
			}
			if constexpr (
				hasPerVertexTexCoord<MeshType>() || hasPerFaceWedgeTexCoords<MeshType>()) {
				if (header == "vt") {
					// save the texcoord for later
					TexCoordd tf;
					for (uint i = 0; i < 2; ++i) {
						tf[i] = internal::readDouble<double>(token);
					}
					wedgeTexCoords.push_back(tf);
				}
			}
			if constexpr (hasFaces<MeshType>()) {
				if (header == "f") {
					internal::loadFace(m, tokens, loadedInfo, enableOptionalComponents);
				}
			}
		}
	} while (file);

	if constexpr (hasPerVertexNormal<MeshType>()) {
		// set all vertex normals that have not been stored in vertices
		for (const auto& p : mapNormalsCache) {
			if (p.first < m.vertexNumber()) {
				m.vertex(p.first).normal() = p.second;
			}
		}
	}
}

} // namespace vcl::io
