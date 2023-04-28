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

#ifndef VCL_VIEWS_VIEWS_H
#define VCL_VIEWS_VIEWS_H

#include <ranges>

namespace vcl::views{

namespace internal {

struct NotNullView
{
	constexpr NotNullView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, NotNullView)
	{
		return std::views::filter(r, [](auto* p) { return p != nullptr; });
	}
#endif
};

struct DereferenceView
{
	constexpr DereferenceView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, DereferenceView)
	{
		return std::views::transform(r, [](auto p) { return *p; });
	}
#endif
};

struct AddressOfView
{
	constexpr AddressOfView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, AddressOfView)
	{
		return std::views::transform(r, [](auto& o) { return &o; });
	}
#endif
};

struct ConstAddressOfView
{
	constexpr ConstAddressOfView() = default;

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, ConstAddressOfView)
	{
		return std::views::transform(r, [](const auto& o) { return &o; });
	}
#endif
};

} // namespace vcl::views::internal

/**
 * @brief The notNull view allows to filter the pointers of a range. The resulting
 * view will iterate only on the pointers that are not `nullptr`.
 */
inline constexpr internal::NotNullView notNull;

/**
 * @brief The deref view the dereference operator `*` on the input view.
 *
 * It allows to dereference the pointers of a range. The resulting view will iterate over the
 * objects pointed by the range of pointers.
 *
 * @note: no check on the validity of the pointers is performed. If you know that in your range
 * there is the possibility to have `nullptr` pointers, use first the `notNull` view:
 *
 * auto resView = inputRange | notNull | dereference;
 */
inline constexpr internal::DereferenceView deref;

/**
 * @brief The addrOf view applies the address-of operator `&` on the input view.
 *
 * It allows to get the pointers of the objects of a range. The resulting view will iterate over
 * the pointers pointing to the object of the input range.
 */
inline constexpr internal::AddressOfView addrOf;

/**
 * @brief The constAddrOf view applies the address-of operator `&` on the input view.
 *
 * It allows to get the const pointers of the objects of a range. The resulting view will iterate
 * over the const pointers pointing to the object of the input range (that may be also not const).
 */
inline constexpr internal::ConstAddressOfView constAddrOf;

} // namespace vcl::views

#endif // VCL_VIEWS_VIEWS_H
