/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "save_ply.h"

namespace mgp::io {

template<typename MeshType>
void savePly(const MeshType& m, const std::string& filename, bool binary)
{
	mgp::io::FileMeshInfo info(m);
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
	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

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

	fp.open(plyfilename);
	if (!fp) {
		throw mgp::CannotOpenFileException(plyfilename);
	}
	fp << header.toString();

	if (mgp::hasVertices(m))
		ply::saveVertices(fp, header, m);

	if (mgp::hasFaces(m))
		ply::saveFaces(fp, header, m);

	fp.close();
}

} // namespace mgp::io
