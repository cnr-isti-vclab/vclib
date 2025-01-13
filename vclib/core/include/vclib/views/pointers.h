/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_VIEWS_POINTERS_H
#define VCL_VIEWS_POINTERS_H

#include <ranges>

namespace vcl::views {

namespace detail {

struct NotNullView
{
    constexpr NotNullView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, NotNullView)
    {
        return std::views::filter(r, [](auto* p) {
            return p != nullptr;
        });
    }
};

struct DereferenceView
{
    constexpr DereferenceView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, DereferenceView)
    {
        return std::views::transform(r, [](auto p) {
            return *p;
        });
    }
};

struct AddressOfView
{
    constexpr AddressOfView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, AddressOfView)
    {
        return std::views::transform(r, [](auto& o) {
            return &o;
        });
    }
};

struct ConstAddressOfView
{
    constexpr ConstAddressOfView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, ConstAddressOfView)
    {
        return std::views::transform(r, [](const auto& o) {
            return &o;
        });
    }
};

} // namespace detail

/**
 * @brief The notNull view allows to filter the pointers of a range. The
 * resulting view will iterate only on the pointers that are not `nullptr`.
 *
 * @ingroup views
 */
inline constexpr detail::NotNullView notNull;

/**
 * @brief The deref view the dereference operator `*` on the input view.
 *
 * It allows to dereference the pointers of a range. The resulting view will
 * iterate over the objects pointed by the range of pointers.
 *
 * @note: no check on the validity of the pointers is performed. If you know
 * that in your range there is the possibility to have `nullptr` pointers, use
 * first the `notNull` view:
 *
 * @code{.cpp}
 * auto resView = inputRange | notNull | dereference;
 * @endcode
 *
 * @ingroup views
 */
inline constexpr detail::DereferenceView deref;

/**
 * @brief The addrOf view applies the address-of operator `&` on the input view.
 *
 * It allows to get the pointers of the objects of a range. The resulting view
 * will iterate over the pointers pointing to the object of the input range.
 *
 * @ingroup views
 */
inline constexpr detail::AddressOfView addrOf;

/**
 * @brief The constAddrOf view applies the address-of operator `&` on the input
 * view.
 *
 * It allows to get the const pointers of the objects of a range. The resulting
 * view will iterate over the const pointers pointing to the object of the input
 * range (that may be also not const).
 *
 * @ingroup views
 */
inline constexpr detail::ConstAddressOfView constAddrOf;

} // namespace vcl::views

#endif // VCL_VIEWS_POINTERS_H
