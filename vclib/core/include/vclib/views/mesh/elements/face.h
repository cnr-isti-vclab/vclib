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

#ifndef VCL_VIEWS_MESH_ELEMENTS_FACE_H
#define VCL_VIEWS_MESH_ELEMENTS_FACE_H

#include <vclib/concepts.h>

namespace vcl::views {
namespace detail {

struct FacesView
{
    constexpr FacesView() = default;

    template<FaceMeshConcept R>
    friend constexpr auto operator|(R&& r, FacesView)
    {
        return r.faces();
    }
};

} // namespace detail

/**
 * @brief A view that allows to iterate overt the Face elements of an object.
 *
 * This view can be applied to objects having type that satisfies the
 * FaceMeshConcept.
 *
 * @ingroup views
 */
inline constexpr detail::FacesView faces;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_ELEMENTS_FACE_H
