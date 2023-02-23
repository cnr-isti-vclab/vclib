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

#include "geometry.h"

namespace vcl {

template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceNormal(const FaceType& f)
{
	if constexpr (TriangleFaceConcept<FaceType>) {
		return triangleNormal(f);
	}
	else {
		return polygonNormal(f);
	}
}

template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceBarycenter(const FaceType& f)
{
	if constexpr (TriangleFaceConcept<FaceType>) {
		return triangleBarycenter(f);
	}
	else {
		return polygonBarycenter(f);
	}
}

template<FaceConcept FaceType, typename ScalarType>
ScalarType faceArea(const FaceType& f)
{
	if constexpr (TriangleFaceConcept<FaceType>) {
		return triangleArea<FaceType, ScalarType>(f);
	}
	else {
		return polygonArea<FaceType, ScalarType>(f);
	}
}

} // namespace vcl
