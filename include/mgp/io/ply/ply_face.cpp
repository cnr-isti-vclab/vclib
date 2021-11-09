/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "ply_face.h"
#include <mgp/mesh/requirements.h>

namespace mgp {
namespace ply {

namespace internal {

template<typename MeshType, typename FaceType>
void saveFaceIndices(
	std::ofstream&  file,
	Property        p,
	const MeshType& m,
	const FaceType& f,
	bool            bin)
{
	using VertexType = typename MeshType::Vertex;

	unsigned int fsize = f.vertexNumber();
	internal::writeProperty(file, fsize, p.listSizeType, bin);
	for (const VertexType* v : f.vertexIterator()){
		internal::writeProperty(file, m.vertexIdIfCompact(v->id()), p.type, bin);
	}
}

// load

template<typename MeshType>
void loadFacesTxt(
	std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	using FaceType = typename MeshType::Face;
	bool error     = false;
	mgp::Tokenizer spaceTokenizer;

	error = !internal::nextLine(file, spaceTokenizer);

	mgp::Tokenizer::iterator token = spaceTokenizer.begin();

	mesh.reserveFaces(header.numberFaces());
	for (uint fid = 0; fid < header.numberFaces(); ++fid) {
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
				uint fSize = internal::readProperty<uint>(token, p.listSizeType);
				bool splitFace = false;
				if constexpr(FaceType::VERTEX_NUMBER < 0){
					f.resizeVertices(fSize);
				}
				else if (FaceType::VERTEX_NUMBER != fSize)
					splitFace = true;
				if (!splitFace) {
					for (uint i = 0; i < fSize; ++i) {
						assert(token != spaceTokenizer.end());
						int vid = internal::readProperty<size_t>(token, p.type);
						if (vid < 0)
							f.v(i) = nullptr;
						else
							f.v(i) = &mesh.vertex(vid);
					}
					hasBeenRead = true;
				}
				else { // TODO: split face and then load properly n faces
					for (uint i = 0; i < fSize; ++i) {
						if (i < FaceType::VERTEX_NUMBER) {
							assert(token != spaceTokenizer.end());
							int vid = internal::readProperty<size_t>(token, p.type);
							if (vid < 0)
								f.v(i) = nullptr;
							else
								f.v(i) = &mesh.vertex(vid);
						}
						else {
							internal::readProperty<size_t>(token, p.type);
						}
					}
					hasBeenRead = true;
				}
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (mgp::hasPerFaceNormal(mesh)) {
					if (mgp::isPerFaceNormalEnabled(mesh)) {
						using Scalar = typename FaceType::NormalType::ScalarType;
						int a = p.name - ply::nx;
						f.normal()[a] = internal::readProperty<Scalar>(token, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (mgp::hasPerFaceColor(mesh)) {
					if (mgp::isPerFaceColorEnabled(mesh)) {
						int a = p.name - ply::red;
						f.color()[a] = internal::readProperty<unsigned char>(token, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (!hasBeenRead) {
				if (p.list) {
					uint s = internal::readProperty<int>(token, p.listSizeType);
					for (uint i = 0; i < s; ++i)
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
	for (uint fid = 0; fid < header.numberFaces(); ++fid) {
		mesh.addFace();
		FaceType& f = mesh.face(mesh.faceNumber() -1);
		for (ply::Property p : header.faceProperties()) {
			bool hasBeenRead = false;
			if (p.name == ply::vertex_indices) {
				uint fSize = internal::readProperty<uint>(file, p.listSizeType);
				bool splitFace = false;
				if constexpr(FaceType::VERTEX_NUMBER < 0){
					f.resizeVertices(fSize);
				}
				else if (FaceType::VERTEX_NUMBER != fSize)
					splitFace = true;
				if (!splitFace) {
					for (uint i = 0; i < fSize; ++i) {
						int vid = internal::readProperty<size_t>(file, p.type);
						if (vid < 0)
							f.v(i) = nullptr;
						else
							f.v(i) = &mesh.vertex(vid);
					}
					hasBeenRead = true;
				}
				else { // TODO: split face and then load properly n faces
					for (uint i = 0; i < fSize; ++i) {
						if (i < FaceType::VERTEX_NUMBER) {
							int vid = internal::readProperty<size_t>(file, p.type);
							if (vid < 0)
								f.v(i) = nullptr;
							else
								f.v(i) = &mesh.vertex(vid);
						}
						else {
							internal::readProperty<size_t>(file, p.type);
						}
					}
					hasBeenRead = true;
				}
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (mgp::hasPerFaceNormal(mesh)) {
					if (mgp::isPerFaceNormalEnabled(mesh)) {
						using Scalar = typename FaceType::NormalType::ScalarType;
						int a = p.name - ply::nx;
						f.normal()[a] = internal::readProperty<Scalar>(file, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (mgp::hasPerFaceColor(mesh)) {
					if (mgp::isPerFaceColorEnabled(mesh)) {
						int a = p.name - ply::red;
						f.color()[a] = internal::readProperty<unsigned char>(file, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (!hasBeenRead) {
				if (p.list) {
					uint s = internal::readProperty<int>(file, p.listSizeType);
					for (uint i = 0; i < s; ++i)
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
	for (const FaceType& f : mesh.faceIterator()){
		for (ply::Property p : header.faceProperties()) {
			bool hasBeenWritten = false;
			if (p.name == ply::vertex_indices){
				internal::saveFaceIndices(file, p, mesh, f, bin);
				hasBeenWritten = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (mgp::hasPerFaceNormal(mesh)) {
					if (mgp::isPerFaceNormalEnabled(mesh)) {
						internal::writeProperty(file, f.normal()[p.name - ply::nx], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (mgp::hasPerFaceColor(mesh)) {
					if (mgp::isPerFaceColorEnabled(mesh)) {
						internal::writeProperty(file, f.color()[p.name - ply::red], p.type, bin);
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
} // namespace mgp
