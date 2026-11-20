// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENTS_FACE_H
#define VCL_MESH_VIEWS_ELEMENTS_FACE_H

#include "../element.h"

#include <vclib/mesh/elements/face.h>

#include <ranges>

namespace vcl {

/**
 * @brief Concept that checks whether Rng is a valid range of faces.
 *
 * @ingroup element_concepts
 */
template<typename Rng>
concept FaceRangeConcept =
    std::ranges::range<Rng> &&
    FaceConcept<typename std::ranges::iterator_t<Rng>::value_type>;

namespace views {

namespace detail {

struct FacesView : std::ranges::range_adaptor_closure<FacesView>
{
    constexpr FacesView() = default;

    template<typename R>
    requires requires (R&& r) {
        { r.faces() } -> std::ranges::range;
    }
    constexpr auto operator()(R&& r) const
    {
        return r.faces();
    }

    template<typename R>
    requires (!std::ranges::range<R>) && requires (R&& r) {
        { r.faces() } -> std::ranges::range;
    }
    friend constexpr auto operator|(R&& r, FacesView)
    {
        return r.faces();
    }
};

} // namespace detail

/**
 * @brief A view adaptor that allows to iterate over the Face elements of an
 * object.
 *
 * Can be applied to Meshes: `mesh | views::faces`
 *
 * Supports both pipe syntax (`mesh | views::faces`) and function call syntax
 * (`views::faces(mesh)`).
 *
 * @ingroup views
 */
inline constexpr detail::FacesView faces;

template<>
inline constexpr auto elements<ElemId::FACE> = faces;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_FACE_H
