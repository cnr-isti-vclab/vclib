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

// here, type of R is not constrained by any concept.
// We trust that all the closures structs defined in the vcl::views::internal namespace will
// constraint the Range types (which may also not be ranges at all)
template <typename R, typename Closure>
constexpr auto operator | (R& r, Closure const & a)
{
	return a(r);
}

template <typename R, typename Closure>
constexpr auto operator | (const R& r, Closure const & a)
{
	return a(r);
}

struct IsValidClosure
{
	inline static constexpr auto isValid = [](auto* p)
	{
		return p != nullptr;
	};

	constexpr IsValidClosure() {};

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return r | std::views::filter(isValid);
	}
};

struct DereferenceClosure
{
	inline static constexpr auto deref = [](auto p)
	{
		return *p;
	};

	constexpr DereferenceClosure() {};

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return r | std::views::transform(deref);
	}
};

struct ReferenceClosure
{
	inline static constexpr auto ref = [](auto o)
	{
		return &o;
	};

	constexpr ReferenceClosure() {};

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return r | std::views::transform(ref);
	}
};

} // namespace vcl::views::internal

/**
 * @brief The isValid view allows to filter the pointers of a range. The resulting
 * view will iterate only on the pointers that are not `nullptr`.
 */
inline constexpr internal::IsValidClosure isValid;

/**
 * @brief The dereference view allow to dereference the pointers of a range. The resulting
 * view will iterate over the objects pointed by the range of pointers.
 *
 * @note: no check on the validity of the pointers is performed. If you know that in your range
 * there is the possibility to have `nullptr` pointers, use first the `isValid` view:
 *
 * auto resView = inputRange | isValid | dereference;
 */
inline constexpr internal::DereferenceClosure dereference;

/**
 * @brief The reference view allow to reference the objects of a range. The resulting view will
 * iterate over the pointers pointing to the object of the input range.
 */
inline constexpr internal::ReferenceClosure reference;

} // namespace vcl::views

#endif // VCL_TYPES_VIEWS_H
