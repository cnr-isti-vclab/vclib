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

#include "face_requirements.h"

#include <vclib/exception/mesh_exception.h>

#include "../containers/face_container.h"

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its Face Container compact.
 * @param[in] m: input mesh on which test if the its Face Container is compact.
 * @return `true` if the Face Container of the mesh is compact.
 */
template<FaceMeshConcept MeshType>
bool isFaceContainerCompact(const MeshType& m)
{
	return (m.faceNumber() == m.faceContainerSize());
}

template<typename MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalAdjacentFaces<typename MeshType::FaceType>) {
			return m.isPerFaceAdjacentFacesEnabled();
		}
		else {
			return vcl::face::HasAdjacentFaces<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType& m)
{
	if constexpr (HasPerFaceAdjacentFaces<MeshType>) {
		if constexpr(vcl::face::HasOptionalAdjacentFaces<typename MeshType::FaceType>) {
			m.enablePerFaceAdjacentFaces();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceColorEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalColor<typename MeshType::FaceType>) {
			return m.isPerFaceColorEnabled();
		}
		else {
			return vcl::face::HasColor<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceColorOptional(MeshType& m)
{
	if constexpr (HasPerFaceColor<MeshType>) {
		if constexpr(vcl::face::HasOptionalColor<typename MeshType::FaceType>) {
			m.enablePerFaceColor();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceMarkEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalMark<typename MeshType::FaceType>) {
			return m.isPerFaceMarkEnabled();
		}
		else {
			return vcl::face::HasMark<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceMarkOptional(MeshType& m)
{
	if constexpr (HasPerFaceMark<MeshType>) {
		if constexpr(vcl::face::HasOptionalMark<typename MeshType::FaceType>) {
			m.enablePerFaceMark();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceNormalEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalNormal<typename MeshType::FaceType>) {
			return m.isPerFaceNormalEnabled();
		}
		else {
			return vcl::face::HasNormal<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceNormalOptional(MeshType& m)
{
	if constexpr (HasPerFaceNormal<MeshType>) {
		if constexpr(vcl::face::HasOptionalNormal<typename MeshType::FaceType>) {
			m.enablePerFaceNormal();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFacePrincipalCurvatureEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalPrincipalCurvature<typename MeshType::FaceType>) {
			return m.isPerFacePrincipalCurvatureEnabled();
		}
		else {
			return vcl::face::HasPrincipalCurvature<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType& m)
{
	if constexpr (HasPerFacePrincipalCurvature<MeshType>) {
		if constexpr(vcl::face::HasOptionalPrincipalCurvature<typename MeshType::FaceType>) {
			m.enablePerFacePrincipalCurvature();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceScalarEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalScalar<typename MeshType::FaceType>) {
			return m.isPerFaceScalarEnabled();
		}
		else {
			return vcl::face::HasScalar<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceScalarOptional(MeshType& m)
{
	if constexpr (HasPerFaceScalar<MeshType>) {
		if constexpr(vcl::face::HasOptionalScalar<typename MeshType::FaceType>) {
			m.enablePerFaceScalar();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceWedgeColorsEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalWedgeColors<typename MeshType::FaceType>) {
			return m.isPerFaceWedgeColorsEnabled();
		}
		else {
			return vcl::face::HasWedgeColors<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType& m)
{
	if constexpr (HasPerFaceWedgeColors<MeshType>) {
		if constexpr(vcl::face::HasOptionalWedgeColors<typename MeshType::FaceType>) {
			m.enablePerFaceWedgeColors();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceWedgeTexCoordsEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalWedgeTexCoords<typename MeshType::FaceType>) {
			return m.isPerFaceWedgeTexCoordsEnabled();
		}
		else {
			return vcl::face::HasWedgeTexCoords<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType& m)
{
	if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
		if constexpr(vcl::face::HasOptionalWedgeTexCoords<typename MeshType::FaceType>) {
			m.enablePerFaceWedgeTexCoords();
		}
		return true;
	}
	else {
		return false;
	}
}

/*********************
 * require functions *
 *********************/

template<FaceMeshConcept MeshType>
void requireFaceContainerCompactness(const MeshType& m)
{
	if (!isFaceContainerCompact(m))
		throw vcl::MissingCompactnessException("Face Container of the Mesh is not compact.");
}

template<FaceMeshConcept MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m)
	requires HasPerFaceAdjacentFaces<MeshType>
{
	if (!isPerFaceAdjacentFacesEnabled(m))
		throw vcl::MissingComponentException("Per face adjacent faces not enabled.");
}

template<FaceMeshConcept MeshType>
void requirePerFaceColor(const MeshType& m)
	requires HasPerFaceColor<MeshType>
{
	if (!isPerFaceColorEnabled(m))
		throw vcl::MissingComponentException("Face colors not enabled.");
}

template<FaceMeshConcept MeshType>
void requirePerFaceMark(const MeshType& m)
	requires HasPerFaceMark<MeshType>
{
	if (!isPerFaceMarkEnabled(m))
		throw vcl::MissingComponentException("Face mark not enabled.");
}

template<FaceMeshConcept MeshType>
void requirePerFaceNormal(const MeshType& m)
	requires HasPerFaceNormal<MeshType>
{
	if (!isPerFaceNormalEnabled(m))
		throw vcl::MissingComponentException("Face normals not enabled.");
}

template<FaceMeshConcept MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m)
	requires HasPerFacePrincipalCurvature<MeshType>
{
	if (!isPerFacePrincipalCurvatureEnabled(m))
		throw vcl::MissingComponentException("Face principal curvature not enabled.");
}

template<FaceMeshConcept MeshType>
void requirePerFaceScalar(const MeshType& m)
	requires HasPerFaceScalar<MeshType>
{
	if (!isPerFaceScalarEnabled(m))
		throw vcl::MissingComponentException("Face scalars not enabled.");
}

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeColors(const MeshType& m)
	requires HasPerFaceWedgeColors<MeshType>
{
	if (!isPerFaceWedgeColorsEnabled(m))
		throw vcl::MissingComponentException("Face wedge colors not enabled.");
}

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m)
	requires HasPerFaceWedgeTexCoords<MeshType>
{
	if (!isPerFaceWedgeTexCoordsEnabled(m))
		throw vcl::MissingComponentException("Face wedge texcoords not enabled.");
}

} // namespace vcl
