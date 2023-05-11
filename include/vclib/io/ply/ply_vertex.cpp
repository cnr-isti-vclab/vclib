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

#include "ply_vertex.h"

#include <vclib/misc/tokenizer.h>
#include <vclib/mesh/requirements.h>
#include <vclib/exceptions/io_exceptions.h>

#include "../internal/io_read.h"
#include "../internal/io_write.h"
#include "vclib/concepts/mesh/per_vertex.h"
#include "vclib/io/ply/ply.h"

namespace vcl::io::ply {

namespace internal {

template<MeshConcept MeshType, VertexConcept VertexType, typename Stream>
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
		if constexpr (vcl::HasPerVertexNormal<MeshType>) {
			if (vcl::isPerVertexNormalEnabled(mesh)) {
				using Scalar = typename VertexType::NormalType::ScalarType;
				int a = p.name - ply::nx;
				v.normal()[a] = io::internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name >= ply::red && p.name <= ply::alpha) {
		if constexpr (vcl::HasPerVertexColor<MeshType>) {
			if (vcl::isPerVertexColorEnabled(mesh)) {
				int a = p.name - ply::red;
				v.color()[a] = io::internal::readProperty<unsigned char>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name == ply::quality) {
		if constexpr (vcl::HasPerVertexQuality<MeshType>) {
			using QualityType = typename VertexType::QualityType;
			if (vcl::isPerVertexQualityEnabled(mesh)) {
				v.quality() = io::internal::readProperty<QualityType>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
		if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
			using Scalar = typename VertexType::TexCoordType::ScalarType;
			if (vcl::isPerVertexTexCoordEnabled(mesh)) {
				int a = p.name - ply::texture_u;
				v.texCoord()[a] = io::internal::readProperty<Scalar>(file, p.type);
				hasBeenRead = true;
			}
		}
	}
	if (p.name == ply::unknown) {
		if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
			if (mesh.hasPerVertexCustomComponent(p.unknownPropertyName)){
				io::internal::readCustomComponent(file, v, p.unknownPropertyName, p.type);
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

template<VertexConcept VertexType, MeshConcept MeshType>
void loadVertexTxt(
	std::ifstream& file,
	VertexType& v,
	MeshType& mesh,
	const std::list<ply::Property>& vertexProperties)
{
	vcl::Tokenizer spaceTokenizer = io::internal::nextNonEmptyTokenizedLine(file);
	vcl::Tokenizer::iterator token = spaceTokenizer.begin();
	for (const ply::Property& p : vertexProperties) {
		if (token == spaceTokenizer.end()){
			throw vcl::MalformedFileException("Unexpected end of line.");
		}
		loadVertexProperty(token, mesh, v, p);
	}
}

template<VertexConcept VertexType, MeshConcept MeshType>
void loadVertexBin(
	std::ifstream& file,
	VertexType& v,
	MeshType& mesh,
	const std::list<ply::Property>& vertexProperties)
{
	for (const ply::Property& p : vertexProperties) {
		loadVertexProperty(file, mesh, v, p);
	}
}

} //namespace vcl::ply::internal

template <MeshConcept MeshType>
void saveVertices(
	std::ofstream& file,
	const PlyHeader& header,
	const MeshType& mesh)
{
	using VertexType = typename MeshType::VertexType;

	bool bin = header.format() == ply::BINARY;
	for(const VertexType& v : mesh.vertices()) {
		for (const ply::Property& p : header.vertexProperties()) {
			bool hasBeenWritten = false;
			if (p.name >= ply::x && p.name <= ply::z) {
				io::internal::writeProperty(file, v.coord()[p.name - ply::x], p.type, bin);
				hasBeenWritten = true;
			}
			if (p.name >= ply::nx && p.name <= ply::nz) {
				if constexpr (vcl::HasPerVertexNormal<MeshType>) {
					io::internal::writeProperty(file, v.normal()[p.name - ply::nx], p.type, bin);
					hasBeenWritten = true;
				}
			}
			if (p.name >= ply::red && p.name <= ply::alpha) {
				if constexpr (vcl::HasPerVertexColor<MeshType>) {
					io::internal::writeProperty(file, v.color()[p.name - ply::red], p.type, bin);
					hasBeenWritten = true;
				}
			}
			if (p.name == ply::quality) {
				if constexpr (vcl::HasPerVertexQuality<MeshType>) {
					io::internal::writeProperty(file, v.quality(), p.type, bin);
					hasBeenWritten = true;
				}
			}
			if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
				if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
					const uint a = p.name - ply::texture_u;
					io::internal::writeProperty(file, v.texCoord()[a], p.type, bin);
					hasBeenWritten = true;
				}
			}
			if (p.name == ply::unknown) {
				if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
					if (mesh.hasPerVertexCustomComponent(p.unknownPropertyName)) {
						io::internal::writeCustomComponent(file, v, p.unknownPropertyName, p.type, bin);
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

template <MeshConcept MeshType>
void loadVertices(
	std::ifstream& file,
	const PlyHeader& header,
	MeshType& m)
{
	m.addVertices(header.numberVertices());

	for(uint vid = 0; vid < header.numberVertices(); ++vid) {
		auto& v = m.vertex(vid);
		if(header.format() == ply::ASCII) {
			internal::loadVertexTxt(file, v, m, header.vertexProperties());
		}
		else if(header.format() == ply::BINARY) {
			internal::loadVertexBin(file, v, m, header.vertexProperties());
		}
	}
}

} //namespace vcl::ply
