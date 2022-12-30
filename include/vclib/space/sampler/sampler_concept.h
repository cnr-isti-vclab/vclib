/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_SPACE_SAMPLER_SAMPLER_CONCEPT_H
#define VCL_SPACE_SAMPLER_SAMPLER_CONCEPT_H

#include <vclib/misc/internal/tmp_meshes.h>

namespace vcl {

template<typename T>
concept SamplerConcept = requires(
	T o,
	const internal::TMPSimplePolyMesh& m,
	const internal::TMPSimplePolyMesh::VertexType& v)
{
	o.samples();

	o.clear();
	o.reserve(uint());
	o.addVertex(v, m);
};

template<typename T>
concept FaceSamplerConcept = SamplerConcept<T> &&
	requires(
		T o,
		const internal::TMPSimplePolyMesh& m,
		const internal::TMPSimplePolyMesh::VertexType::CoordType& p,
		const internal::TMPSimplePolyMesh::FaceType& f)
{
	typename T::ScalarType;
	o.addFace(f, m);
	o.addFace(f, m, p);
};

} // namespace vcl

#endif // VCL_SPACE_SAMPLER_SAMPLER_CONCEPT_H
