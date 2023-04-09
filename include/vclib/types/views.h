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

#ifndef VCL_TYPES_VIEWS_H
#define VCL_TYPES_VIEWS_H

#include <ranges>

namespace vcl::views{

namespace internal {

struct NotNullView : std::ranges::view_base
{
	constexpr NotNullView() = default;

	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, NotNullView)
	{
		return std::views::filter(r, [](auto* p) { return p != nullptr; });
	}
};

struct DereferenceView : std::ranges::view_base
{
	constexpr DereferenceView() = default;

	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, DereferenceView)
	{
		return std::views::transform(r, [](auto p) { return *p; });
	}
};

struct ReferenceView
{
	constexpr ReferenceView() = default;

	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, ReferenceView)
	{
		return std::views::transform(r, [](auto o) { return &o; });
	}
};

} // namespace vcl::views::internal

/**
 * @brief The notNull view allows to filter the pointers of a range. The resulting
 * view will iterate only on the pointers that are not `nullptr`.
 */
inline constexpr internal::NotNullView notNull;

/**
 * @brief The dereference view allow to dereference the pointers of a range. The resulting
 * view will iterate over the objects pointed by the range of pointers.
 *
 * @note: no check on the validity of the pointers is performed. If you know that in your range
 * there is the possibility to have `nullptr` pointers, use first the `isValid` view:
 *
 * auto resView = inputRange | isValid | dereference;
 */
inline constexpr internal::DereferenceView dereference;

/**
 * @brief The reference view allow to reference the objects of a range. The resulting view will
 * iterate over the pointers pointing to the object of the input range.
 */
inline constexpr internal::ReferenceView reference;

} // namespace vcl::views

#endif // VCL_TYPES_VIEWS_H
