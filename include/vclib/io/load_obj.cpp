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
#include <vclib/misc/file_info.h>

#include "internal/io_read.h"
#include "internal/io_utils.h"
#include "obj/material.h"

namespace vcl::io {

namespace internal {

template<typename MeshType>
void loadMaterials(
	std::map<std::string, obj::Material>& materialMap,
	MeshType& mesh,
	const std::string& mtllib)
{
	std::ifstream file = loadFileStream(mtllib);
	std::string matName;
	obj::Material mat;

	do {
		vcl::Tokenizer tokens = nextNonEmptyTokenizedLineNoThrow(file);
		if (file) {
			uint nt = 0; // counter for texture images, used when mesh has no texture files
			vcl::Tokenizer::iterator token = tokens.begin();
			std::string header = *token++;
			if (header == "newmtl"){
				if (!matName.empty())
					materialMap[matName] = mat;
				mat = obj::Material();
				matName = *token;
			}
			if (header == "Ka") {
				if (tokens.size() >= 4) {
					if (*token != "spectral" && *token != "xyz") {
						mat.Ka.x() = readFloat<float>(token);
						mat.Ka.y() = readFloat<float>(token);
						mat.Ka.z() = readFloat<float>(token);
					}
				}
			}
			if (header == "Kd") {
				if (tokens.size() >= 4) {
					if (*token != "spectral" && *token != "xyz") {
						mat.Kd.x() = readFloat<float>(token);
						mat.Kd.y() = readFloat<float>(token);
						mat.Kd.z() = readFloat<float>(token);
						mat.hasColor = true;
					}
				}
			}
			if (header == "Ks") {
				if (tokens.size() >= 4) {
					if (*token != "spectral" && *token != "xyz") {
						mat.Ks.x() = readFloat<float>(token);
						mat.Ks.y() = readFloat<float>(token);
						mat.Ks.z() = readFloat<float>(token);
					}
				}
			}
			if (header == "d" || header == "Tr") {
				if ((*token)[0] == '-') token++;
				mat.d = readFloat<float>(token);
			}
			if (header == "Ns") {
				mat.Ns = readFloat<float>(token);
			}
			if (header == "illum") {
				mat.illum = readFloat<int>(token);
			}
			if (header == "map_Kd") {
				// need to manage args
				while ((*token)[0] == '-') {
					if (*token == "-o" || *token == "-s" || *token == "-t"){
						// ignore the argument and the three values
						++token; ++token; ++token; ++token;
					}
					if (*token == "-mm"){
						// ignore the argument and the two values
						++token; ++token; ++token;
					}
					if (*token == "-blendu" || *token == "-blendv" || *token == "-cc" ||
						*token == "-clamp" || *token == "-texres") {
						// ignore the argument and the value
						++token; ++token;
					}
				}
				mat.map_Kd = *token;
				mat.hasTexture = true;
				if constexpr (hasTexFileNames<MeshType>()) {
					mat.mapId = mesh.textureNumber();
					mesh.pushTexture(mat.map_Kd);
				}
				else {
					mat.mapId = nt++;
				}
			}
		}
	} while(file);
	if (!matName.empty())
		materialMap[matName] = mat;
}

template<typename MeshType>
void loadVertexCoord(
	MeshType&                    m,
	vcl::Tokenizer::iterator&    token,
	FileMeshInfo&                loadedInfo,
	const vcl::Tokenizer&        tokens,
	const obj::Material&         currentMaterial,
	bool                         enableOptionalComponents)
{
	// first, need to set that I'm loading vertices
	if (m.vertexNumber() == 0) {
		loadedInfo.setVertices();
		loadedInfo.setVertexCoords();
	}
	uint vid = m.addVertex();
	for (uint i = 0; i < 3; ++i) {
		m.vertex(vid).coord()[i] = internal::readDouble<double>(token);
	}
	if constexpr (hasPerVertexColor<MeshType>()){
		if (vid == 0) {
			// if the current material has a valid color, of the file stores the vertex color in the
			// non-standard way (color values after the coordinates)
			if (currentMaterial.hasColor || tokens.size() > 6) {
				if (enableOptionalComponents) {
					enableIfPerVertexColorOptional(m);
					loadedInfo.setVertexColors();
				}
				else {
					if (isPerVertexColorEnabled(m))
						loadedInfo.setVertexColors();
				}
			}
		}
		if (loadedInfo.hasVertexColors()) {
			// the file has the nonstandard way to store vertex colors, after the coords...
			if (tokens.size() > 6) {
				m.vertex(vid).color().setRedF(internal::readFloat<float>(token));
				m.vertex(vid).color().setGreenF(internal::readFloat<float>(token));
				m.vertex(vid).color().setBlueF(internal::readFloat<float>(token));
			}
			else if (currentMaterial.hasColor) {
				m.vertex(vid).color() = currentMaterial.color();
			}
		}
	}
}


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
		// read the normal
		Point3d n;
		for (uint i = 0; i < 3; ++i) {
			n[i] = internal::readDouble<double>(token);
		}
		// I can store the normal in its vertex
		if (m.vertexNumber() > vn) {
			m.vertex(vn).normal() = n;
		}
		// read the normal and save it in the cache map, because we still don't have read the
		// vertex corresponding to the current normal
		else {
			mapNormalsCache[vn] = n;
		}
	}
}

template<typename MeshType>
void loadFace(
	MeshType& m,
	FileMeshInfo&         loadedInfo,
	const vcl::Tokenizer& tokens,
	const std::vector<vcl::TexCoordd>& wedgeTexCoords,
	const obj::Material&         currentMaterial,
	bool                  enableOptionalComponents)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<uint> vids;
	std::vector<uint> wids;

	// actual read - load vertex indices and texcoords indices, if present
	vcl::Tokenizer::iterator token = tokens.begin();
	++token;
	vids.resize(tokens.size()-1);
	wids.reserve(tokens.size()-1);
	for (uint i = 0; i < tokens.size()-1; ++i) {
		vcl::Tokenizer subt(*token, '/', false);
		auto t = subt.begin();
		vids[i] = readUInt<uint>(t) - 1;
		if (subt.size() > 1) {
			if (!t->empty()) {
				wids.push_back(readUInt<uint>(t) - 1);
			}
		}
		++token;
	}

	// add the face
	uint fid = m.addFace();
	FaceType& f = m.face(fid);

	// check if we need to split the face we read into triangles
	bool splitFace = false;
	// we have a polygonal mesh, no need to split
	if constexpr (FaceType::VERTEX_NUMBER < 0) {
		f.resizeVertices(tokens.size()-1); // need to resize to the right number of verts
	}
	else if (FaceType::VERTEX_NUMBER != tokens.size()-1) {
		// we have faces with static sizes (triangles), but we are loading faces with
		// number of verts > 3. Need to split the face we are loading in n faces!
		splitFace = true;
	}

	// create the face in the mesh, for now we manage only vertex indices
	if (!splitFace) { // no need to split face case
		for (uint i = 0; i < vids.size(); ++i) {
			if (vids[i] >= m.vertexNumber()) {
				throw vcl::MalformedFileException(
					"Bad vertex index for face " + std::to_string(fid));
			}
			f.vertex(i) = &m.vertex(vids[i]);
		}
	}
	else { // split needed
		addTriangleFacesFromPolygon(m, f, vids);
	}

	// color
	if (hasPerFaceColor<MeshType>()){
		if (fid == 0) { // if the first face, we need to check if I can store colors
			// if the current material has no color, we assume that the file has no face color
			if (currentMaterial.hasColor) {
				if (enableOptionalComponents) {
					enableIfPerFaceColorOptional(m);
					loadedInfo.setFaceColors();
				}
				else {
					if (isPerFaceColorEnabled(m))
						loadedInfo.setFaceColors();
				}
			}
		}
		if (loadedInfo.hasFaceColors()) {
			if (currentMaterial.hasColor) {
				// in case the loaded polygon has been triangulated in the last n triangles of mesh
				for (uint ff = fid; ff < m.faceNumber(); ++ff) {
					m.face(ff).color() = currentMaterial.color();
				}
			}
		}
	}

	// wedge coords
	if constexpr(hasPerFaceWedgeTexCoords<MeshType>()) {
		// first, need to check if I can store wedge texcoords in the mesh
		if (fid == 0) {
			// if the current face has the right number of wedge texcoords, we assume that we can
			// load wedge texcoords
			if (wids.size() == vids.size()) {
				if (enableOptionalComponents) {
					enableIfPerFaceWedgeTexCoordsOptional(m);
					loadedInfo.setFaceWedgeTexCoords();
				}
				else {
					if (isPerFaceWedgeTexCoordsEnabled(m))
						loadedInfo.setFaceWedgeTexCoords();
				}
			}
		}
		if (loadedInfo.hasFaceWedgeTexCoords()) {
			if (wids.size() == vids.size()) {
				if (!splitFace) { // there wasn't a triangulation of the face
					// it is safe to assign each wedge texcoord to its position in the face
					for (uint i = 0; i < wids.size(); ++i) {
						if (wids[i] >= wedgeTexCoords.size()) {
							throw vcl::MalformedFileException(
								"Bad texcoord index for face " + std::to_string(fid));
						}
						f.wedgeTexCoord(i) = wedgeTexCoords[wids[i]];
						if (currentMaterial.hasTexture){
							f.wedgeTexCoord(i).nTexture() = currentMaterial.mapId;
						}
					}
				}
				else {
					// take read texcoords and map them in the triangulated faces
					// for each face of the triangulation of the polygon
					for (uint ff = fid; ff < m.faceNumber(); ++ff) {
						FaceType& f = m.face(ff);
						// for each vertex of the face
						for (uint i = 0; i < f.vertexNumber(); ++i){
							uint vid = m.index(f.vertex(i));
							// find the position of the vertex in the vids array
							auto it = std::find(vids.begin(), vids.end(), vid);
							assert(it != vids.end());
							uint pos = it - vids.begin();
							// check that the texcoord id is valid
							if (wids[pos] >= wedgeTexCoords.size()) {
								throw vcl::MalformedFileException(
									"Bad texcoord index for face " + std::to_string(fid));
							}
							// set the wedge texcoord in the same position of the vertex
							f.wedgeTexCoord(i) = wedgeTexCoords[wids[pos]];
							if (currentMaterial.hasTexture){
								f.wedgeTexCoord(i).nTexture() = currentMaterial.mapId;
							}
						}
					}
				}
			}
		}
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
	// save normals if they can't be stored directly into vertices
	std::map<uint, vcl::Point3d> mapNormalsCache;
	uint vn = 0; // number of vertex normals read
	// save array of texcoords, that are stored later (into wedges when loading faces or into
	// vertices as a fallback)
	std::vector<vcl::TexCoordd> texCoords;

	// map of materials loaded
	std::map<std::string, obj::Material> materialMap;
	obj::Material currentMaterial; // the current material, set by 'usemtl'

	// some obj files do not declare the material file name with mtllib, but they assume that
	// material file has the same name of the obj file. Therefore, we first load this file if it
	// exists.
	std::string stdmtlfile = fileInfo::pathWithoutFilename(filename) +
							 fileInfo::filenameWithoutExtension(filename) + ".mtl";
	try {
		internal::loadMaterials(materialMap, m, stdmtlfile);
	}
	catch(vcl::CannotOpenFileException){
		// nothing to do, this file was missing but was a fallback for some type of files...
	}

	// cycle that reads line by line
	do {
		vcl::Tokenizer tokens = internal::nextNonEmptyTokenizedLineNoThrow(file);
		if (file) {
			vcl::Tokenizer::iterator token = tokens.begin();
			std::string header = *token++;
			if (header == "mtllib") { // material file
				std::string mtlfile = fileInfo::pathWithoutFilename(filename) + *token;
				internal::loadMaterials(materialMap, m, mtlfile);
			}
			if (header == "usemtl") { // use a new material - change currentMaterial
				std::string matname = *token;
				auto it = materialMap.find(matname);
				if (it != materialMap.end()) {
					currentMaterial = it->second;
				}
			}
			// read vertex (and for some non-standard obj files, also vertex color)
			if (header == "v") {
				internal::loadVertexCoord(
					m, token, loadedInfo, tokens, currentMaterial, enableOptionalComponents);
			}
			// read vertex normal (and save in vn how many normals we read)
			if constexpr(hasPerVertexNormal<MeshType>()) {
				if (header == "vn") {
					internal::loadVertexNormal(
						m, mapNormalsCache, vn, token, loadedInfo, enableOptionalComponents);
					vn++;
				}
			}
			// read texcoords and save them in the vector of texcoords, we will store them in the
			// mesh later
			if constexpr (
				hasPerVertexTexCoord<MeshType>() || hasPerFaceWedgeTexCoords<MeshType>()) {
				if (header == "vt") {
					// save the texcoord for later
					TexCoordd tf;
					for (uint i = 0; i < 2; ++i) {
						tf[i] = internal::readDouble<double>(token);
					}
					// store also the texture id, if present in the currentMaterial
					if (currentMaterial.hasTexture)
						tf.nTexture() = currentMaterial.mapId;
					texCoords.push_back(tf);
				}
			}
			// read faces and manage:
			// - color
			// - eventual texcoords
			// - possibility to split polygonal face into several triangles
			if constexpr (hasFaces<MeshType>()) {
				if (header == "f") {
					internal::loadFace(
						m,
						loadedInfo,
						tokens,
						texCoords,
						currentMaterial,
						enableOptionalComponents);
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
	if constexpr (hasPerVertexTexCoord<MeshType>()) {
		using VertexType = typename MeshType::VertexType;
		if (!loadedInfo.hasFaceWedgeTexCoords()) {
			// we can set the loaded texCoords to vertices, also if they are not supported in obj
			if (texCoords.size() == m.vertexNumber()) {
				if (enableOptionalComponents) {
					enableIfPerVertexTexCoordOptional(m);
					loadedInfo.setVertexTexCoords();
				}
				else {
					if (isPerVertexTexCoordEnabled(m))
						loadedInfo.setVertexTexCoords();
				}
				if (loadedInfo.hasVertexTexCoords()) {
					uint i = 0;
					for (VertexType& v : m.vertices()) {
						v.texCoord() = texCoords[i++];
					}
				}
			}
		}
	}
}

} // namespace vcl::io
