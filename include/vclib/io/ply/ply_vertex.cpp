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

#include "ply_vertex.h"

#include <vclib/misc/tokenizer.h>
#include <vclib/mesh/requirements.h>
#include <vclib/exception/io_exception.h>

#include "../internal/io_read.h"
#include "../internal/io_write.h"

namespace vcl::io::ply {

namespace internal {

template<typename MeshType, typename VertexType, typename Stream>
void loadVertexProperty(Stream& file, MeshType& mesh, VertexType& v, ply::Property p)
{
	bool hasBeenRead = false;
	if (p.name >= ply::x && p.name <= ply::z) {
		using Scalar = typename VertexType::CoordType::ScalarType;
		int a = p.name - ply::x;
		v.coord()[a] = io::internal::readProperty<Scalar>(file, p.type);
		hasBeenRead = true;
	}
	if (p.name >= ply::nx && p.name <= ply::nz) {
		if constexpr (vcl::hasPerVertexNormal<MeshType>()) {
			if (vcl::isPerVertexNormalEnabled(mesh)) {
				using Scalar = typename VertexType::NormalType::ScalarType;
				int a = p.name - ply::nx;
				v.normal()[a] = io::internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name >= ply::red && p.name <= ply::alpha) {
		if constexpr (vcl::hasPerVertexColor<MeshType>()) {
			if (vcl::isPerVertexColorEnabled(mesh)) {
				int a = p.name - ply::red;
				v.color()[a] = io::internal::readProperty<unsigned char>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name == ply::scalar) {
		if constexpr (vcl::hasPerVertexScalar<MeshType>()) {
			using Scalar = typename VertexType::ScalarType;
			if (vcl::isPerVertexScalarEnabled(mesh)) {
				v.scalar() = io::internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
		if constexpr (vcl::hasPerVertexTexCoord<MeshType>()) {
			using Scalar = typename VertexType::TexCoordType::ScalarType;
			if (vcl::isPerVertexTexCoordEnabled(mesh)) {
				int a = p.name - ply::texture_u;
				v.texCoord()[a] = io::internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name == ply::texnumber) {
		if constexpr (vcl::hasPerVertexTexCoord<MeshType>()) {
			if (vcl::isPerVertexTexCoordEnabled(mesh)) {
				v.texCoord().nTexture() = io::internal::readProperty<uint>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (!hasBeenRead) {
		if (p.list) {
			uint s = io::internal::readProperty<int>(file, p.listSizeType);
			for (uint i = 0; i < s; ++i)
				io::internal::readProperty<int>(file, p.type);
		}
		else {
			io::internal::readProperty<int>(file, p.type);
		}
	}
}

template <typename MeshType>
void loadVerticesTxt(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& mesh)
{
	using VertexType = typename MeshType::VertexType;

	mesh.addVertices(header.numberVertices());
	for(uint vid = 0; vid < header.numberVertices(); ++vid) {
		vcl::Tokenizer spaceTokenizer = io::internal::nextNonEmptyTokenizedLine(file);
		vcl::Tokenizer::iterator token = spaceTokenizer.begin();
		VertexType& v = mesh.vertex(vid);
		for (ply::Property p : header.vertexProperties()) {
			if (token == spaceTokenizer.end()){
				throw vcl::MalformedFileException("Unexpected end of line.");
			}
			loadVertexProperty(token, mesh, v, p);
		}
	}
}

template<typename MeshType>
void loadVerticesBin(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& mesh)
{
	using VertexType = typename MeshType::VertexType;
	mesh.addVertices(header.numberVertices());
	for(uint vid = 0; vid < header.numberVertices(); ++vid) {
		VertexType& v = mesh.vertex(vid);
		for (ply::Property p : header.vertexProperties()) {
			loadVertexProperty(file, mesh, v, p);
		}
	}
}

} //namespace vcl::ply::internal

template <typename MeshType>
void saveVertices(
	std::ofstream& file,
	const PlyHeader& header,
	const MeshType& mesh)
{
	using VertexType = typename MeshType::VertexType;

	bool bin = header.format() == ply::BINARY;
	for(const VertexType& v : mesh.vertices()) {
		for (ply::Property p : header.vertexProperties()) {
			bool hasBeenWritten = false;
			if (p.name >= ply::x && p.name <= ply::z) {
				int a = p.name - ply::x;
				io::internal::writeProperty(file, v.coord()[a], p.type, bin);
				hasBeenWritten = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (vcl::hasPerVertexNormal<MeshType>()) {
					if (vcl::isPerVertexNormalEnabled(mesh)) {
						io::internal::writeProperty(file, v.normal()[p.name - ply::nx], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (vcl::hasPerVertexColor<MeshType>()) {
					if (vcl::isPerVertexColorEnabled(mesh)) {
						io::internal::writeProperty(file, v.color()[p.name - ply::red], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name == ply::scalar) {
				if constexpr (vcl::hasPerVertexScalar<MeshType>()) {
					if (vcl::isPerVertexScalarEnabled(mesh)) {
						io::internal::writeProperty(file, v.scalar(), p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
				if constexpr (vcl::hasPerVertexTexCoord<MeshType>()) {
					if (vcl::isPerVertexTexCoordEnabled(mesh)) {
						int a = p.name - ply::texture_u;
						io::internal::writeProperty(file, v.texCoord()[a], p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (p.name == ply::texnumber) {
				if constexpr (vcl::hasPerVertexTexCoord<MeshType>()) {
					if (vcl::isPerVertexTexCoordEnabled(mesh)) {
						io::internal::writeProperty(file, v.texCoord().nTexture(), p.type, bin);
						hasBeenWritten = true;
					}
				}
			}
			if (!hasBeenWritten){
				// be sure to write something if the header declares some property that is not
				// in the mesh
				io::internal::writeProperty(file, 0, p.type, bin);
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

} //namespace vcl::ply
