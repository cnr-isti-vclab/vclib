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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H

#include "component.h"

#include <vclib/space/color.h>

namespace vcl::comp {

/**
 * @brief HasWedgeColors concept is satisfied only if a Element class provides the types and
 * member functions specified in this concept. These types and member functions allow to access to
 * an WedgeColors component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal WedgeColors component and
 * the vertical OptionalWedgeColors component, therefore it does not guarantee that a template
 * Element type that satisfies this concept provides WedgeColors component at runtime (it is
 * guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that WedgeColors is available at runtime, you need to call the member
 * function `isWedgeColorsEnabled()`.
 */
template<typename T>
concept HasWedgeColors = requires(
	T o,
	const T& co,
	vcl::Color c,
	std::vector<vcl::Color> v)
{
	T::WEDGE_COLOR_NUMBER;
	typename T::WedgeColorsIterator;
	typename T::ConstWedgeColorsIterator;
	typename T::WedgeColorsRangeIterator;
	typename T::ConstWedgeColorsRangeIterator;

	{ o.wedgeColor(uint()) } -> std::same_as<vcl::Color&>;
	{ co.wedgeColor(uint()) } -> std::same_as<const vcl::Color&>;
	{ o.wedgeColorMod(int()) } -> std::same_as<vcl::Color&>;
	{ co.wedgeColorMod(int()) } -> std::same_as<const vcl::Color&>;
	{ o.setWedgeColor(c, uint()) } -> std::same_as<void>;
	{ o.setWedgeColors(v) } -> std::same_as<void>;
	{ co.isWedgeColorsEnabled() } -> std::same_as<bool>;

	{ o.wedgeColorBegin() } -> std::same_as<typename T::WedgeColorsIterator>;
	{ o.wedgeColorEnd() } -> std::same_as<typename T::WedgeColorsIterator>;
	{ co.wedgeColorBegin() } -> std::same_as<typename T::ConstWedgeColorsIterator>;
	{ co.wedgeColorEnd() } -> std::same_as<typename T::ConstWedgeColorsIterator>;
	{ o.wedgeColors() } -> std::same_as<typename T::WedgeColorsRangeIterator>;
	{ co.wedgeColors() } -> std::same_as<typename T::ConstWedgeColorsRangeIterator>;
};

/**
 * @brief HasWedgeColorsComponent concept is used to discriminate between the WedgeColors (or
 * OptionalWedgeColors) component, and the FaceHalfEdgeReferences component, which using half edges
 * allows to access to wedge colors. This concept is intended only for internal use, useful to check
 * that a Face does not have both WedgeColors and FaceHalfEdgeReferences components.
 */
template<typename T>
concept HasWedgeColorsComponent = requires(T o)
{
	requires HasWedgeColors<T>;
	typename T::WedgeColorsComponent;
};

/**
 * @brief HasOptionalWedgeColors concept is satisfied only if a class satisfies the
 * HasWedgeColors concept and has the additional member function '__optionalWedgeColors()', which is
 * the discriminator between the non-optional and optional component.
 */
template<typename T>
concept HasOptionalWedgeColors =
	HasWedgeColorsComponent<T> && IsOptionalComponent<typename T::WedgeColorsComponent>;

/**
 * @brief HasRightNumberOfWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of wedge colors must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge colors is the same of the static
 * number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeColors = T::VERTEX_NUMBER == T::WEDGE_COLOR_NUMBER;

/**
 * @brief SanityCheckWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of wedge colors must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge colors;
 * - in case it has wedge colors, they have the same number of vertices of the face.
 */
template<typename T>
concept SanityCheckWedgeColors = !HasWedgeColors<T> || HasRightNumberOfWedgeColors<T>;

/* Detector function to check if a class has WedgeColors enabled */

template <typename T>
bool isWedgeColorsEnabledOn(const T& element)
{
	if constexpr(HasOptionalWedgeColors<T>) {
		return element.isWedgeColorsEnabled();
	}
	else {
		return HasWedgeColors<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H
