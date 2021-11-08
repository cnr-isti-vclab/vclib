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

template<typename MeshType>
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

	m.clear();
	for (ply::Element el : header) {
		switch (el.type) {
		case ply::VERTEX: ply::loadVertices(file, header, m); break;
		case ply::FACE: ply::loadFaces(file, header, m); break;
		default: break;
		}
	}
	file.close();
}

} // namespace mgp::io
