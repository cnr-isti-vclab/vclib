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

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H

#include <vclib/concepts.h>
#include <vclib/types.h>

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
