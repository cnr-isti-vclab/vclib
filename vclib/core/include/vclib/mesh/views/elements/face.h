// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENTS_FACE_H
#define VCL_MESH_VIEWS_ELEMENTS_FACE_H

#include <vclib/mesh/requirements/face_requirements.h>

namespace vcl {

/**
 * @brief The FaceRangeConcept evaluates to true if Rng is a valid Range
 * on Faces.
 *
 * This means that Rng must be a Range of FaceConcept: the iterated type
 * must satisfy the FaceConcept.
 *
 * @ingroup face_concepts
 */
template<typename Rng>
concept FaceRangeConcept =
    Range<Rng> &&
    FaceConcept<typename std::ranges::iterator_t<Rng>::value_type>;

/**
 * @brief The FacePointerRangeConcept evaluates to true if Rng is a valid
 * Range on Face Pointers.
 *
 * This means that Rng must be a Range of pointers to a type that satisfy the
 * FaceConcept.
 *
 * @ingroup face_concepts
 */
template<typename Rng>
concept FacePointerRangeConcept =
    Range<Rng> &&
    IsPointer<typename std::ranges::iterator_t<Rng>::value_type> &&
    FaceConcept<typename std::decay_t<
        RemovePtr<typename std::ranges::iterator_t<Rng>::value_type>>>;

namespace views {
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

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_FACE_H
