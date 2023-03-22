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

#include "load.h"

#include <vclib/algorithm/polygon.h>
#include <vclib/exception/io_exception.h>
#include <vclib/misc/file_info.h>
#include <vclib/misc/tokenizer.h>

#include "utils.h"

namespace vcl::io {

namespace internal {

template <MeshConcept MeshType>
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

		if constexpr(vcl::HasPerVertexNormal<MeshType>) {
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

		if constexpr(vcl::HasPerVertexColor<MeshType>) {
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

		if constexpr(vcl::HasPerVertexTexCoord<MeshType>) {
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

template<FaceMeshConcept MeshType>
void loadOffFaces(
	MeshType&      mesh,
	std::ifstream& file,
	FileMeshInfo&  loadedInfo,
	uint           nf,
	bool           enableOptionalComponents)
{
	if constexpr (HasFaces<MeshType>) {
		using FaceType = typename MeshType::FaceType;

		mesh.reserveFaces(nf);
		for (uint fid = 0; fid < nf; ++fid) {
			vcl::Tokenizer tokens = vcl::io::internal::nextNonEmptyTokenizedLine(file);
			vcl::Tokenizer::iterator token = tokens.begin();
			mesh.addFace();
			FaceType& f = mesh.face(mesh.faceNumber() - 1);

			// read vertex indices
			uint fSize = io::internal::readUInt<uint>(token);
			std::vector<uint> vids; // contains the vertex ids of the actual face
			vids.resize(fSize);
			for (uint i = 0; i < fSize; ++i) {
				vids[i] = io::internal::readUInt<uint>(token);
				bool splitFace = false;
				// we have a polygonal mesh
				if constexpr (FaceType::VERTEX_NUMBER < 0) {
					f.resizeVertices(vids.size()); // need to resize to the right number of verts
				}
				else if (FaceType::VERTEX_NUMBER != vids.size()) {
					// we have faces with static sizes (triangles), but we are loading faces with
					// number of verts > 3. Need to split the face we are loading in n faces!
					splitFace = true;
				}
				if (!splitFace) { // classic load, no split needed
					for (uint i = 0; i < vids.size(); ++i) {
						if (vids[i] >= mesh.vertexNumber()) {
							throw vcl::MalformedFileException(
								"Bad vertex index for face " + std::to_string(i));
						}
						f.vertex(i) = &mesh.vertex(vids[i]);
					}
				}
				else { // split needed
					addTriangleFacesFromPolygon(mesh, f, vids);
				}
			}

			// read face color
			if (token != tokens.end()) { // there are colors to read
				if constexpr (HasPerFaceColor<MeshType>) {
					if (isPerFaceColorEnabled(mesh) ||
						(enableOptionalComponents && enableIfPerFaceColorOptional(mesh))){
						loadedInfo.setFaceColors();
						f.color() = off::loadColor(token, tokens.size() - (token - tokens.begin()));
						// in case the loaded polygon has been triangulated in the last n triangles
						for (uint ff = mesh.index(f); ff < mesh.faceNumber(); ++ff) {
							mesh.face(ff).color() = f.color();
						}
					}
				}
			}
		}
	}
	else { // mesh does not have face, read nf lines and throw them away
		for (uint i = 0; i < nf; ++i)
			vcl::io::internal::nextNonEmptyTokenizedLine(file);
	}
}

} // namespace internal

template<MeshConcept MeshType, LoggerConcept LogType>
MeshType loadOff(const std::string& filename, LogType& log, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	return loadOff<MeshType>(filename, loadedInfo, log, enableOptionalComponents);
}

template<MeshConcept MeshType, LoggerConcept LogType>
MeshType loadOff(
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	LogType&           log,
	bool               enableOptionalComponents)
{
	MeshType m;
	loadOff(m, filename, loadedInfo, log, enableOptionalComponents);
	return m;
}

template<MeshConcept MeshType, LoggerConcept LogType>
void loadOff(MeshType& m, const std::string& filename, LogType& log, bool enableOptionalComponents)
{
	FileMeshInfo loadedInfo;
	loadOff(m, filename, loadedInfo, log, enableOptionalComponents);
}

template<MeshConcept MeshType, LoggerConcept LogType>
void loadOff(
	MeshType&          m,
	const std::string& filename,
	FileMeshInfo&      loadedInfo,
	LogType&           log,
	bool               enableOptionalComponents)
{
	std::ifstream file = internal::loadFileStream(filename);
	uint nVertices, nFaces, nEdges;

	if constexpr (HasName<MeshType>) {
		m.name() = FileInfo::filenameWithoutExtension(filename);
	}

	FileMeshInfo fileInfo; // data that needs to be read from the file

	off::loadOffHeader(file, fileInfo, nVertices, nFaces, nEdges);
	loadedInfo = fileInfo; // data that will be stored in the mesh!
	if (enableOptionalComponents)
		internal::enableOptionalComponents(loadedInfo, m);

	internal::loadOffVertices(m, file, fileInfo, nVertices);
	internal::loadOffFaces(m, file, fileInfo, nFaces, enableOptionalComponents);
	if (enableOptionalComponents)
		loadedInfo = fileInfo;
	// internal::loadOffEdges(m, file, loadedInfo, nEdges);
}

} // namespace vcl::io
