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

#include "ply_face.h"
#include <vclib/mesh/requirements.h>
#include <vclib/exception/io_exception.h>

namespace vcl {
namespace ply {

namespace internal {

template<typename MeshType, typename FaceType>
void saveFaceIndices(
	std::ofstream&  file,
	Property        p,
	const MeshType& m,
	const std::vector<int>& vIndices,
	const FaceType& f,
	bool            bin)
{
	using VertexType = typename MeshType::Vertex;

	unsigned int fsize = f.vertexNumber();
	internal::writeProperty(file, fsize, p.listSizeType, bin);
	for (const VertexType* v : f.vertices()){
		internal::writeProperty(file, vIndices[m.index(v)], p.type, bin);
	}
}

// load

template<typename MeshType>
void loadFacesTxt(
	std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	using FaceType = typename MeshType::Face;
	bool error     = false;
	vcl::Tokenizer spaceTokenizer;

	error = !internal::nextLine(file, spaceTokenizer);

	vcl::Tokenizer::iterator token = spaceTokenizer.begin();

	mesh.reserveFaces(header.numberFaces());
	for (unsigned int fid = 0; fid < header.numberFaces(); ++fid) {
		mesh.addFace();
		FaceType& f = mesh.face(mesh.faceNumber() -1);
		for (ply::Property p : header.faceProperties()) {
			if (token == spaceTokenizer.end()) {
				error = !nextLine(file, spaceTokenizer);
				token = spaceTokenizer.begin();
			}
			if (error)
				throw std::runtime_error("Malformed file");
			bool hasBeenRead = false;
			if (p.name == ply::vertex_indices) {
				unsigned int fSize = internal::readProperty<unsigned int>(token, p.listSizeType);
				bool splitFace = false;
				if constexpr(FaceType::VERTEX_NUMBER < 0){
					f.resizeVertices(fSize);
				}
				else if (FaceType::VERTEX_NUMBER != fSize)
					splitFace = true;
				if (!splitFace) {
					for (unsigned int i = 0; i < fSize; ++i) {
						assert(token != spaceTokenizer.end());
						int vid = internal::readProperty<size_t>(token, p.type);
						if (vid < 0 || vid >= mesh.vertexNumber()) {
							throw vcl::MalformedFileException(
								"Bad vertex index for face " + std::to_string(i));
						}
						else
							f.vertex(i) = &mesh.vertex(vid);
					}
					hasBeenRead = true;
				}
				else { // TODO: split face and then load properly n faces
					for (unsigned int i = 0; i < fSize; ++i) {
						if (i < FaceType::VERTEX_NUMBER) {
							assert(token != spaceTokenizer.end());
							int vid = internal::readProperty<size_t>(token, p.type);
							if (vid < 0)
								f.vertex(i) = nullptr;
							else
								f.vertex(i) = &mesh.vertex(vid);
						}
						else {
							internal::readProperty<size_t>(token, p.type);
						}
					}
					hasBeenRead = true;
				}
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (vcl::hasPerFaceNormal<MeshType>()) {
					if (vcl::isPerFaceNormalEnabled(mesh)) {
						using Scalar = typename FaceType::NormalType::ScalarType;
						int a = p.name - ply::nx;
						f.normal()[a] = internal::readProperty<Scalar>(token, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (vcl::hasPerFaceColor<MeshType>()) {
					if (vcl::isPerFaceColorEnabled(mesh)) {
						int a = p.name - ply::red;
						f.color()[a] = internal::readProperty<unsigned char>(token, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name == ply::scalar) {
				if constexpr (vcl::hasPerFaceScalar<MeshType>()) {
					using Scalar = typename FaceType::ScalarType;
					if (vcl::isPerFaceScalarEnabled(mesh)) {
						f.scalar() = internal::readProperty<Scalar>(token, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (!hasBeenRead) {
				if (p.list) {
					unsigned int s = internal::readProperty<int>(token, p.listSizeType);
					for (unsigned int i = 0; i < s; ++i)
						++token;
				}
				else {
					++token;
				}
			}
		}
	}
}

template<typename MeshType>
void loadFacesBin(
	std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	using FaceType = typename MeshType::Face;
	mesh.reserveFaces(header.numberFaces());
	for (unsigned int fid = 0; fid < header.numberFaces(); ++fid) {
		mesh.addFace();
		FaceType& f = mesh.face(mesh.faceNumber() -1);
		for (ply::Property p : header.faceProperties()) {
			bool hasBeenRead = false;
			if (p.name == ply::vertex_indices) {
				unsigned int fSize = internal::readProperty<unsigned int>(file, p.listSizeType);
				bool splitFace = false;
				if constexpr(FaceType::VERTEX_NUMBER < 0){
					f.resizeVertices(fSize);
				}
				else if (FaceType::VERTEX_NUMBER != fSize)
					splitFace = true;
				if (!splitFace) {
					for (unsigned int i = 0; i < fSize; ++i) {
						int vid = internal::readProperty<size_t>(file, p.type);
						if (vid < 0)
							f.vertex(i) = nullptr;
						else
							f.vertex(i) = &mesh.vertex(vid);
					}
					hasBeenRead = true;
				}
				else { // TODO: split face and then load properly n faces
					for (unsigned int i = 0; i < fSize; ++i) {
						if (i < FaceType::VERTEX_NUMBER) {
							int vid = internal::readProperty<size_t>(file, p.type);
							if (vid < 0)
								f.vertex(i) = nullptr;
							else
								f.vertex(i) = &mesh.vertex(vid);
						}
						else {
							internal::readProperty<size_t>(file, p.type);
						}
					}
					hasBeenRead = true;
				}
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (vcl::hasPerFaceNormal<MeshType>()) {
					if (vcl::isPerFaceNormalEnabled(mesh)) {
						using Scalar = typename FaceType::NormalType::ScalarType;
						int a = p.name - ply::nx;
						f.normal()[a] = internal::readProperty<Scalar>(file, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (vcl::hasPerFaceColor<MeshType>()) {
					if (vcl::isPerFaceColorEnabled(mesh)) {
						int a = p.name - ply::red;
						f.color()[a] = internal::readProperty<unsigned char>(file, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name == ply::scalar) {
				if constexpr (vcl::hasPerFaceScalar<MeshType>()) {
					using Scalar = typename FaceType::ScalarType;
					if (vcl::isPerFaceScalarEnabled(mesh)) {
						f.scalar() = internal::readProperty<Scalar>(file, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (!hasBeenRead) {
				if (p.list) {
					unsigned int s = internal::readProperty<int>(file, p.listSizeType);
					for (unsigned int i = 0; i < s; ++i)
						internal::readProperty<int>(file, p.type);
				}
				else {
					internal::readProperty<int>(file, p.type);
				}
			}
		}
	}
}

} // namespace internal

template<typename MeshType>
void saveFaces(std::ofstream& file, const PlyHeader& header, const MeshType mesh)
{
	using FaceType = typename MeshType::Face;

	bool bin = header.format() == ply::BINARY;

	// indices of vertices that do not consider deleted vertices
	std::vector<int> vIndices = mesh.vertexCompactIndices();

	for (const FaceType& f : mesh.faces()){
		for (ply::Property p : header.faceProperties()) {
			bool hasBeenWritten = false;
			if (p.name == ply::vertex_indices){
				internal::saveFaceIndices(file, p, mesh, vIndices, f, bin);
				hasBeenWritten = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (vcl::hasPerFaceNormal<MeshType>()) {
					if (vcl::isPerFaceNormalEnabled(mesh)) {
						internal::writeProperty(file, f.normal()[p.name - ply::nx], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (vcl::hasPerFaceColor<MeshType>()) {
					if (vcl::isPerFaceColorEnabled(mesh)) {
						internal::writeProperty(file, f.color()[p.name - ply::red], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name == ply::scalar) {
				if constexpr (vcl::hasPerFaceScalar<MeshType>()) {
					if (vcl::isPerFaceScalarEnabled(mesh)) {
						internal::writeProperty(file, f.scalar(), p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (!hasBeenWritten){
				// be sure to write something if the header declares some property that is not
				// in the mesh
				internal::writeProperty(file, 0, p.type, bin);
			}
		}
		if (!bin)
			file << std::endl;
	}
}

template<typename MeshType>
void loadFaces(std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	if (header.format() == ply::ASCII) {
		internal::loadFacesTxt(file, header, mesh);
	}
	else {
		internal::loadFacesBin(file, header, mesh);
	}
}

} // namespace ply
} // namespace vcl
