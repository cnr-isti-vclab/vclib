/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "load_ply.h"

namespace mgp::io {

template<typename MeshType>
void loadPly(MeshType& m, const std::string& filename)
{
	FileMeshInfo loadedInfo;
	loadPly(m, filename, loadedInfo);
}

template <typename MeshType>
void loadPly(MeshType& m, const std::string& filename, mgp::io::FileMeshInfo& loadedInfo)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw mgp::CannotOpenFileException(filename);
	}
	mgp::ply::PlyHeader header(file);
	if (header.errorWhileLoading())
		return; // todo make exceptions

	loadedInfo = header.getInfo();
	io::FileMeshInfo::MeshType meshType;

	uint                          nV = header.numberVertices();
	uint                          nF = header.numberFaces();
	uint                          nE = header.numberEdges();
	std::vector<uint>             vc, fc, ec; // v and f colors
	std::vector<double>           fn;         // f normals
	std::vector<double>           coords(nV * 3);
	std::vector<int>              edges(nE * 2);
	std::vector<int> faces;
	std::vector<double>           vertexNormals(nV * 3);
	vc.resize(nV * 4); // also alpha
	fc.resize(nF * 4); // also alpha
	ec.resize(nE * 4); // also alpha
	fn.resize(nF * 3);

	std::vector<int> faceSizes(nF);

	bool loadOk = true;
	for (ply::Element el : header) {
		switch (el.type) {
		case ply::VERTEX:
			loadOk = ply::loadVertices(
				file,
				header,
				coords.data(),
				vertexNormals.data(),
				io::FileMeshInfo::RGBA,
				vc.data());
			break;
		case ply::FACE:
			loadOk = ply::loadFaces(
				file,
				header,
				faces,
				meshType,
				fn.data(),
				io::FileMeshInfo::RGBA,
				fc.data(),
				faceSizes.data());
			loadedInfo.setMeshType(meshType);
			break;
		default: break;
		}
		if (!loadOk)
			return;
	}
	file.close();

	m.reserveVertices(nV);
	m.reserveFaces(nF);

	for (unsigned int i = 0; i < nV*3; i += 3) {
		int id                    = m.addVertex();
		m.vertex(id).coordinate() = Point3d(coords[i], coords[i + 1], coords[i + 2]);
	}
	for (unsigned int i = 0; i < nF*3; i += 3) {
		int id = m.addFace();
		m.face(id).v0() = &m.vertex(faces[i]);
		m.face(id).v1() = &m.vertex(faces[i+1]);
		m.face(id).v2() = &m.vertex(faces[i+2]);
	}
}

} // namespace mgp::io
