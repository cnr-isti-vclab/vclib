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

#include "vertex_index_sampler.h"

namespace vcl {

inline const std::vector<uint> VertexIndexSampler::samples() const
{
	return samplesVec;
}

inline void VertexIndexSampler::clear()
{
	samplesVec.clear();
}

inline void VertexIndexSampler::reserve(uint n)
{
	samplesVec.reserve(n);
}

inline void VertexIndexSampler::resize(uint n)
{
	samplesVec.resize(n);
}

template<MeshConcept MeshType>
void VertexIndexSampler::addVertex(const typename MeshType::VertexType &v, const MeshType &m)
{
	samplesVec.push_back(m.index(v));
}

template<MeshConcept MeshType>
void VertexIndexSampler::setVertex(
	uint                                 i,
	const typename MeshType::VertexType& v,
	const MeshType&                      m)
{
	samplesVec[i] = m.index(v);
}

} // namespace vcl
