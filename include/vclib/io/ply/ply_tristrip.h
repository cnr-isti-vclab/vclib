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

#ifndef VCL_IO_PLY_TRISTRIP_H
#define VCL_IO_PLY_TRISTRIP_H

#include <fstream>

#include <vclib/misc/tokenizer.h>

#include "ply_header.h"
#include "../internal/io_read.h"

namespace vcl::io::ply {

namespace internal {

template<FaceMeshConcept MeshType>
void facesFromTriStrip(MeshType& m, const std::vector<int>& tristrip)
{
	using FaceType   = MeshType::FaceType;

	bool firstOddPos = false;
	for (uint k = 0; k < tristrip.size() - 2; ++k) {
		if (tristrip[k + 2] < 0) {
			k += 2;
			if (k % 2 == 0)
				firstOddPos = false;
			else
				firstOddPos = true;
		}
		else {
			uint      fid = m.addFace();
			FaceType& f   = m.face(fid);
			if constexpr (FaceType::VERTEX_NUMBER < 0) {
				f.resizeVertices(3);
			}
			uint i = 0;
			for (auto& v : f.vertices()) {
				v = &m.vertex(tristrip[k + i]);
				++i;
			}
			if (k % 2 == 0 != firstOddPos)
				std::swap(f.vertex(0), f.vertex(1));
		}
	}
}

template<FaceMeshConcept MeshType>
void loadTriStripsTxt(std::ifstream& file, const PlyHeader& header, MeshType& m)
{
	for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
		vcl::Tokenizer spaceTokenizer =
			vcl::io::internal::nextNonEmptyTokenizedLine(file);
		vcl::Tokenizer::iterator token = spaceTokenizer.begin();
		for (const ply::Property& p : header.triStripsProperties()) {
			if (token == spaceTokenizer.end()) {
				throw vcl::MalformedFileException("Unexpected end of line.");
			}
			bool hasBeenRead = false;
			if (p.name == ply::vertex_indices) {
				uint tSize =
					io::internal::readProperty<uint>(token, p.listSizeType);
				std::vector<int> tristrip(tSize);
				for (uint i = 0; i < tSize; ++i) {
					tristrip[i] =
						io::internal::readProperty<size_t>(token, p.type);
				}
				hasBeenRead = true;
				facesFromTriStrip(m, tristrip);
			}
			if (!hasBeenRead) {
				if (p.list) {
					uint s =
						io::internal::readProperty<int>(token, p.listSizeType);
					for (uint i = 0; i < s; ++i) {
						++token;
					}
				}
				else {
					++token;
				}
			}
		}
	}
}

template<FaceMeshConcept MeshType>
void loadTriStripsBin(std::ifstream& file, const PlyHeader& header, MeshType& m)
{
	for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
		for (const ply::Property& p : header.triStripsProperties()) {
			bool hasBeenRead = false;
			if (p.name == ply::vertex_indices) {
				uint tSize =
					io::internal::readProperty<uint>(file, p.listSizeType);
				std::vector<int> tristrip(tSize);
				for (uint i = 0; i < tSize; ++i)
					tristrip[i] = io::internal::readProperty<int>(file, p.type);
				hasBeenRead = true;
				facesFromTriStrip(m, tristrip);
			}
			if (!hasBeenRead) {
				if (p.list) {
					uint s =
						io::internal::readProperty<int>(file, p.listSizeType);
					for (uint i = 0; i < s; ++i)
						io::internal::readProperty<int>(file, p.type);
				}
				else {
					io::internal::readProperty<int>(file, p.type);
				}
			}
		}
	}
}

} // namespace internal

template<FaceMeshConcept MeshType>
void loadTriStrips(std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
	if (header.format() == ply::ASCII) {
		internal::loadTriStripsTxt(file, header, mesh);
	}
	else if (header.format() == ply::BINARY) {
		internal::loadTriStripsBin(file, header, mesh);
	}
}

} // namespace vcl::ply

#endif // VCL_IO_PLY_TRISTRIP_H
