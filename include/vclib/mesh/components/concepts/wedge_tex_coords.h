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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_TEX_COORDS_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasWedgeTexCoords concept
 *
 * This concept is satisfied only if a class has a member function 'wedgeTexCoord(uint)'.
 */
template<typename T>
concept HasWedgeTexCoords = requires(T o)
{
	typename T::WedgeTexCoordType;
	o.wedgeTexCoord(uint());
};

/**
 * @brief HasWedgeTexCoordsComponent concept
 *
 * This concept is used to discriminate between the WedgeTexCoords (or OptionalWedgeTexCoords)
 * component, and the FaceHalfEdgeReferences component, which using half edges allows to access
 * to wedge tex coords. This concept is intended only for internal use, useful to check that a Face
 * does not have both WedgeTexCoords and FaceHalfEdgeReferences components.
 */
template<typename T>
concept HasWedgeTexCoordsComponent = requires(T o)
{
	HasWedgeTexCoords<T>;
	{ o.__compWedgeTexCoords() } -> std::same_as<void>;
};

/**
 * @brief HasOptionalWedgeColors concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'wedgeTexCoord(uint)'
 * - '__optionalWedgeTexCoords()'
 */
template<typename T>
concept HasOptionalWedgeTexCoords = HasWedgeTexCoords<T> && requires(T o)
{
	{ o.__optionalWedgeTexCoords() } -> std::same_as<void>;
};

/**
 * @brief HasRightNumberOfWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of wedge texcoords
 * must be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge texcoords is the same of the static
 * number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeTexCoords = T::VERTEX_NUMBER == T::WEDGE_TEX_COORD_NUMBER;

/**
 * @brief SanityCheckWedgeTexCoords concept
 *
 * This concept is designed to be used with Face components, where the number of wedge texcoords
 * must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge texcoords;
 * - in case it has wedge texcoords, they have the same number of vertices of the face.
 */
template<typename T>
concept SanityCheckWedgeTexCoords = !HasWedgeTexCoords<T> || HasRightNumberOfWedgeTexCoords<T>;

/* Detector function to check if a class has WedgeTexCoords enabled */

template <typename T>
bool isWedgeTexCoordsEnabledOn(const T& element)
{
	if constexpr (HasOptionalWedgeTexCoords<T>) {
		return element.isWedgeTexCoordsEnabled();
	}
	else {
		return HasWedgeTexCoords<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_TEX_COORDS_H
