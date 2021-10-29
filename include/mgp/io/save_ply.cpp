/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "save_ply.h"

namespace mgp::io {

template<typename MeshType>
void savePly(const MeshType& m, const std::string& filename)
{
	using VertexType = typename MeshType::Vertex;
	using FaceType = typename MeshType::Face;
	mgp::io::FileMeshInfo info;

	bool binary = false;
	std::string plyfilename;
	std::setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator
	std::ofstream fp;
	size_t lastindex = filename.find_last_of(".");
	if (lastindex != filename.size())
		plyfilename = filename;
	else
		plyfilename = filename + ".ply";

	ply::PlyHeader header;
	header.setInfo(info, binary);
	header.setNumberVertices(m.vertexNumber());
	header.setNumberFaces(m.faceNumber());
	fp.open (plyfilename);
	if(!fp) {
		return;
	}
	fp << header.toString();

	unsigned int i = 0;
	std::vector<double> vertices(m.vertexNumber()*3);
	std::vector<double> normals, colors;
	for (const VertexType& v : m.vertexIterator()){
		vertices[i++] = v.coordinate().x();
		vertices[i++] = v.coordinate().y();
		vertices[i++] = v.coordinate().z();
	}

	ply::saveVertices(fp, header, vertices.data(), normals.data(), FileMeshInfo::RGB, colors.data());

	std::vector<unsigned int> faces(m.faceNumber()*3);
	std::vector<unsigned int> pSize(m.faceNumber());

	i = 0;
	for (const FaceType& f : m.faceIterator()){
		faces[i*3+0] = f.v(0)->id();
		faces[i*3+1] = f.v(1)->id();
		faces[i*3+2] = f.v(2)->id();
		pSize[i++] = 3;
	}


	ply::saveFaces(fp, header, faces.data(), info, normals.data(), FileMeshInfo::RGB, colors.data(), pSize.data());
	fp.close();
}

}
