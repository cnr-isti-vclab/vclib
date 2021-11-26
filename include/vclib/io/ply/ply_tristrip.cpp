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

#include "ply_tristrip.h"

namespace vcl::ply {

namespace internal {

template<typename MeshType>
void facesFromTriStrip(MeshType& m, const std::vector<int>& tristrip)
{
	using FaceType   = typename MeshType::FaceType;
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
			for (uint i = 0; i < 3; ++i)
				f.vertex(i) = &m.vertex(tristrip[k + i]);
			if (k % 2 == 0 != firstOddPos)
				std::swap(f.vertex(0), f.vertex(1));
		}
	}
}

template<typename MeshType>
void loadTriStripsTxt(std::ifstream& file, const PlyHeader& header, MeshType& m)
{
	bool           error = false;
	vcl::Tokenizer spaceTokenizer;

	error = !internal::nextLine(file, spaceTokenizer);

	vcl::Tokenizer::iterator token = spaceTokenizer.begin();

	for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
		for (ply::Property p : header.triStripsProperties()) {
			if (token == spaceTokenizer.end()) {
				error = !nextLine(file, spaceTokenizer);
				token = spaceTokenizer.begin();
			}
			if (error)
				throw std::runtime_error("Malformed file");
			bool hasBeenRead = false;
			if (p.name == ply::vertex_indices) {
				uint             tSize = internal::readProperty<uint>(token, p.listSizeType);
				std::vector<int> tristrip(tSize);
				for (uint i = 0; i < tSize; ++i)
					tristrip[i] = internal::readProperty<size_t>(token, p.type);
				hasBeenRead = true;
				facesFromTriStrip(m, tristrip);
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
void loadTriStripsBin(std::ifstream& file, const PlyHeader& header, MeshType& m)
{
	for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
		for (ply::Property p : header.triStripsProperties()) {
			bool hasBeenRead = false;
			if (p.name == ply::vertex_indices) {
				uint             tSize = internal::readProperty<uint>(file, p.listSizeType);
				std::vector<int> tristrip(tSize);
				for (uint i = 0; i < tSize; ++i)
					tristrip[i] = internal::readProperty<int>(file, p.type);
				hasBeenRead = true;
				facesFromTriStrip(m, tristrip);
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
