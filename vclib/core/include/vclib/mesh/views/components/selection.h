// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
#define VCL_MESH_VIEWS_COMPONENTS_SELECTION_H

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept SelectableConcept = requires(T&& t) {
    { t.selected() };
};

template<typename R>
concept SelectableRangeConcept =
    std::ranges::range<R> && SelectableConcept<std::ranges::range_reference_t<R>>;

namespace views {

namespace detail {

inline constexpr auto isSelected = [](auto&& e) -> bool {
    return static_cast<bool>(e.selected());
};

inline constexpr auto isNotSelected = [](auto&& e) -> bool {
    return !static_cast<bool>(e.selected());
};

inline constexpr auto getSelection = [](auto&& e) -> decltype(auto) {
    return e.selected();
};

struct SelectedView : std::ranges::range_adaptor_closure<SelectedView>
{
    template<SelectableRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::filter(isSelected);
    }
};

struct NotSelectedView : std::ranges::range_adaptor_closure<NotSelectedView>
{
    template<SelectableRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::filter(isNotSelected);
    }
};

struct SelectionView : std::ranges::range_adaptor_closure<SelectionView>
{
    template<SelectableRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::transform(getSelection);
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of boolean-like
 * selection values (e.selected()).
 *
 * Can be used with pipe syntax (`range | views::selection`) or function call syntax
 * (`views::selection(range)`).
 *
 * @ingroup views
 */
inline constexpr detail::SelectionView selection;

/**
 * @brief View adaptor that filters a range of elements, yielding only those that are selected.
 *
 * Can be used with pipe syntax (`range | views::selected`) or function call syntax
 * (`views::selected(range)`).
 *
 * @ingroup views
 */
inline constexpr detail::SelectedView selected;

/**
 * @brief View adaptor that filters a range of elements, yielding only those that are not selected.
 *
 * Can be used with pipe syntax (`range | views::notSelected`) or function call syntax
 * (`views::notSelected(range)`).
 *
 * @ingroup views
 */
inline constexpr detail::NotSelectedView notSelected;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
