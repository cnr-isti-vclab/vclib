/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "ply_vertex.h"
#include <mgp/misc/tokenizer.h>
#include <mgp/mesh/requirements.h>

namespace mgp {
namespace ply {

namespace internal {

template <typename MeshType>
void loadVerticesTxt(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& mesh)
{
	using VertexType = typename MeshType::Vertex;
	bool error = false;
	mgp::Tokenizer spaceTokenizer;
	error = !internal::nextLine(file, spaceTokenizer);
	mgp::Tokenizer::iterator token = spaceTokenizer.begin();
	mesh.addVertices(header.numberVertices());
	for(uint vid = 0; vid < header.numberVertices(); ++vid) {
		VertexType& v = mesh.vertex(vid);
		for (ply::Property p : header.vertexProperties()) {
			if (token == spaceTokenizer.end()){
				error = !nextLine(file, spaceTokenizer);
				token = spaceTokenizer.begin();
			}
			if (error)
				throw std::runtime_error("Malformed file");
			bool hasBeenRead = false;
			if (p.name >= ply::x && p.name <= ply::z) {
				using Scalar = typename VertexType::CoordinateType::ScalarType;
				int a = p.name - ply::x;
				v.coordinate()[a] = internal::readProperty<Scalar>(token, p.type);
				hasBeenRead = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (mgp::hasPerVertexNormal(mesh)) {
					if (mgp::isPerVertexNormalEnabled(mesh)) {
						using Scalar = typename VertexType::NormalType::ScalarType;
						int a = p.name - ply::nx;
						v.normal()[a] = internal::readProperty<Scalar>(token, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (mgp::hasPerVertexColor(mesh)) {
					if (mgp::isPerVertexColorEnabled(mesh)) {
						int a = p.name - ply::red;
						v.color()[a] = internal::readProperty<unsigned char>(token, p.type);
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
void loadVerticesBin(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& mesh)
{
	using VertexType = typename MeshType::Vertex;
	mesh.addVertices(header.numberVertices());
	for(uint vid = 0; vid < header.numberVertices(); ++vid) {
		VertexType& v = mesh.vertex(vid);
		for (ply::Property p : header.vertexProperties()) {
			bool hasBeenRead = false;
			if (p.name >= ply::x && p.name <= ply::z) {
				using Scalar = typename VertexType::CoordinateType::ScalarType;
				int a = p.name - ply::x;
				v.coordinate()[a] = internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (mgp::hasPerVertexNormal(mesh)) {
					if (mgp::isPerVertexNormalEnabled(mesh)) {
						using Scalar = typename VertexType::NormalType::ScalarType;
						int a = p.name - ply::nx;
						v.normal()[a] = internal::readProperty<Scalar>(file, p.type);
						hasBeenRead = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (mgp::hasPerVertexColor(mesh)) {
					if (mgp::isPerVertexColorEnabled(mesh)) {
						int a = p.name - ply::red;
						v.color()[a] = internal::readProperty<unsigned char>(file, p.type);
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

} //namespace mgp::ply::internal

template <typename MeshType>
void saveVertices(
	std::ofstream& file,
	const PlyHeader& header,
	const MeshType& mesh)
{
	using VertexType = typename MeshType::Vertex;

	bool bin = header.format() == ply::BINARY;
	for(const VertexType& v : mesh.vertexIterator()) {
		for (ply::Property p : header.vertexProperties()) {
			bool hasBeenWritten = false;
			if (p.name >= ply::x && p.name <= ply::z) {
				int a = p.name - ply::x;
				internal::writeProperty(file, v.coordinate()[a], p.type, bin);
				hasBeenWritten = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (mgp::hasPerVertexNormal(mesh)) {
					if (mgp::isPerVertexNormalEnabled(mesh)) {
						internal::writeProperty(file, v.normal()[p.name - ply::nx], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (mgp::hasPerVertexColor(mesh)) {
					if (mgp::isPerVertexColorEnabled(mesh)) {
						internal::writeProperty(file, v.color()[p.name - ply::red], p.type, bin);
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

template <typename MeshType>
void loadVertices(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& m)
{
	if(header.format() == ply::ASCII) {
		internal::loadVerticesTxt(file, header, m);
	}
	else if(header.format() == ply::BINARY) {
		internal::loadVerticesBin(file, header, m);
	}
}

} //namespace mgp::ply
} //namespace mgp
