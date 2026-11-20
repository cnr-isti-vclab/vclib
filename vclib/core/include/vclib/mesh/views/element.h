// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENT_H
#define VCL_MESH_VIEWS_ELEMENT_H

#include <vclib/base.h>
#include <vclib/mesh/elements/base/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasIndexMember = requires (T&& t) {
    { t.index() } -> std::unsigned_integral;
};

template<typename R>
concept IndexRangeConcept =
    std::ranges::range<R> && HasIndexMember<std::ranges::range_reference_t<R>>;

namespace views {

namespace detail {

struct UnsupportedElementView
{
};

inline constexpr auto index = [](auto&& p) { return p.index(); };

struct IndicesView : std::ranges::range_adaptor_closure<IndicesView>
{
    template<IndexRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::transform(index);
    }
};

} // namespace detail

/**
 * @brief Generic element view adaptor template variable for a given element ID.
 *
 * Specializations for supported element types:
 * - elements<ElemId::VERTEX> = vertices
 * - elements<ElemId::FACE>   = faces
 * - elements<ElemId::EDGE>   = edges
 */
template<ElemId ELEM_ID>
inline constexpr auto elements = detail::UnsupportedElementView {};

/**
 * @brief The indices view transforms a range of elements into a range of their
 * indices.
 *
 * Example:
 * @code{.cpp}
 * for (auto id : mesh.vertices() | views::indices) {
 *     // ...
 * }
 * @endcode
 */
inline constexpr detail::IndicesView indices;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENT_H
