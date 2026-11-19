// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_WEDGES_WEDGE_TRAITS_H
#define VCL_MESH_COMPONENTS_WEDGES_WEDGE_TRAITS_H

#include "../base/base.h"
#include "../base/component.h"
#include "../base/component_predicates.h"

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <concepts>

namespace vcl::comp {

// Forward declarations
template<bool OPT>
class Color;

template<typename Scalar, bool OPT>
class TexCoord;

template<PointConcept P, bool OPT>
class Normal;

/**
 * @brief Traits class mapping a component or raw type to its Wedge representation
 * (COMP_ID and ValueType).
 */
template<typename Comp>
struct WedgeTraits;

// Specialization for Color component:
template<bool OPT>
struct WedgeTraits<Color<OPT>>
{
    static constexpr CompId COMP_ID = CompId::WEDGE_COLORS;
    using ValueType                 = vcl::Color;
};

// Specialization for TexCoord component:
template<typename Scalar, bool OPT>
struct WedgeTraits<TexCoord<Scalar, OPT>>
{
    static constexpr CompId COMP_ID = CompId::WEDGE_TEX_COORDS;
    using ValueType                 = vcl::TexCoord<Scalar>;
};

// Specialization for Normal component:
template<PointConcept P, bool OPT>
struct WedgeTraits<Normal<P, OPT>>
{
    static constexpr CompId COMP_ID = CompId::WEDGE_NORMALS;
    using ValueType                 = P;
};

// Specialization for raw types:
template<>
struct WedgeTraits<vcl::Color> : WedgeTraits<Color<false>> {};

template<typename Scalar>
struct WedgeTraits<vcl::TexCoord<Scalar>> : WedgeTraits<TexCoord<Scalar, false>> {};

template<typename Scalar, uint Dim>
requires (Dim == 3)
struct WedgeTraits<vcl::Point<Scalar, Dim>>
{
    static constexpr CompId COMP_ID = CompId::WEDGE_NORMALS;
    using ValueType                 = vcl::Point<Scalar, Dim>;
};

template<typename Comp>
concept WedgeableComponent = requires {
    typename WedgeTraits<Comp>::ValueType;
    { WedgeTraits<Comp>::COMP_ID } -> std::convertible_to<CompId>;
};

template<typename Comp>
struct IsCompOptional
{
    static constexpr bool value = false;
};

template<typename Comp>
requires ComponentConcept<Comp>
struct IsCompOptional<Comp>
{
    static constexpr bool value = Comp::IS_OPTIONAL;
};

/* Concepts */

template<typename T>
concept HasWedgeColors = HasComponent<CompId::WEDGE_COLORS, T>;

template<typename T>
concept HasOptionalWedgeColors =
    HasWedgeColors<T> && HasOptionalComponent<CompId::WEDGE_COLORS, T>;

template<typename T>
concept HasWedgeTexCoords = HasComponent<CompId::WEDGE_TEX_COORDS, T>;

template<typename T>
concept HasOptionalWedgeTexCoords =
    HasWedgeTexCoords<T> && HasOptionalComponent<CompId::WEDGE_TEX_COORDS, T>;

template<typename T>
concept HasWedgeNormals = HasComponent<CompId::WEDGE_NORMALS, T>;

template<typename T>
concept HasOptionalWedgeNormals =
    HasWedgeNormals<T> && HasOptionalComponent<CompId::WEDGE_NORMALS, T>;

template<typename T>
concept HasWedges =
    HasWedgeColors<T> || HasWedgeTexCoords<T> || HasWedgeNormals<T>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_WEDGES_WEDGE_TRAITS_H
