// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H

#include <vclib/mesh/components/adjacent_faces.h>

#include <vclib/base.h>

namespace vcl::views {

namespace detail {

struct AdjFacesView
{
    constexpr AdjFacesView() = default;

    template<comp::HasAdjacentFaces R>
    friend constexpr auto operator|(R&& r, AdjFacesView)
    {
        if constexpr (IsPointer<R>)
            return r->adjFaces();
        else
            return r.adjFaces();
    }
};

} // namespace detail

/**
 * @brief The adjFaces view allows to obtain a view that access to the adjacent
 * faces of the object that has been piped. Every object having type that
 * satisfies the HasAdjacentFaces concept can be applied to this view.
 *
 * Resulting adjacent faces will be pointers to Faces, that may be `nullptr`.
 * If you are interested only on the not-null pointers, you can use the
 * `notNull` view:
 *
 * @code{.cpp}
 * for (auto* af: f | views::adjFaces | views::notNull) { ... }
 * @endcode
 *
 * @ingroup views
 */
inline constexpr detail::AdjFacesView adjFaces;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H
