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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_COLORS_H

#include "component.h"

#include <vclib/space/color.h>

namespace vcl::comp {

/**
 * @brief HasWedgeColors concept is satisfied only if a Element class provides
 * the types and member functions specified in this concept. These types and
 * member functions allow to access to an WedgeColors component of a given
 * element.
 *
 * Note that this concept does not discriminate between the Horizontal
 * WedgeColors component and the vertical OptionalWedgeColors component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides WedgeColors component at runtime (it is guaranteed only
 * that the proper member functions are available at compile time).
 *
 * To be completely sure that WedgeColors is available at runtime, you need to
 * call the member function `isWedgeColorsEnabled()`.
 *
 * @ingroup components_concepts
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

	{ o.wedgeColor(uint()) } -> std::same_as<vcl::Color&>;
	{ co.wedgeColor(uint()) } -> std::same_as<const vcl::Color&>;
	{ o.wedgeColorMod(int()) } -> std::same_as<vcl::Color&>;
	{ co.wedgeColorMod(int()) } -> std::same_as<const vcl::Color&>;
	{ o.setWedgeColor(c, uint()) } -> std::same_as<void>;
	{ o.setWedgeColors(v) } -> std::same_as<void>;
	{ co.isWedgeColorsEnabled() } -> std::same_as<bool>;

	{ o.wedgeColorBegin() } -> std::same_as<typename T::WedgeColorsIterator>;
	{ o.wedgeColorEnd() } -> std::same_as<typename T::WedgeColorsIterator>;
	{ co.wedgeColorBegin() } ->
		std::same_as<typename T::ConstWedgeColorsIterator>;
	{ co.wedgeColorEnd() } ->
		std::same_as<typename T::ConstWedgeColorsIterator>;
	o.wedgeColors();
	co.wedgeColors();
};

/**
 * @brief HasOptionalWedgeColors concept is satisfied only if a class satisfies
 * the HasWedgeColors concept and has the static boolean constant IS_OPTIONAL is
 * set to true.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalWedgeColors =
	HasWedgeColors<T> && IsOptionalComponent<typename T::WedgeColors>;

/**
 * @private
 * @brief HasRightNumberOfWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge colors must be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge colors is the same
 * of the static number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeColors = T::VERTEX_NUMBER == T::WEDGE_COLOR_NUMBER;

/**
 * @private
 * @brief SanityCheckWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of
 * wedge colors must be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge colors;
 * - in case it has wedge colors, they have the same number of vertices of the
 * face.
 */
template<typename T>
concept SanityCheckWedgeColors =
	!HasWedgeColors<T> || HasRightNumberOfWedgeColors<T>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_WEDGE_COLORS_H
