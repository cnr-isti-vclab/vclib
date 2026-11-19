// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_COLOR_H
#define VCL_MESH_COMPONENTS_COLOR_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The Color class represents a RGBA color that will be part of an
 * Element (e.g. Vertex, Face...).
 *
 * Allows to get and set an object of type Color.
 *
 * For example, if you have a Vertex Element `v` with the Color component,
 * you'll be able to access this component's member functions from `v`:
 *
 * @code{.cpp}
 * auto c = v.color();
 * @endcode
 *
 * @ingroup components
 */
template<bool OPT = false>
class Color : public Component<CompId::COLOR, vcl::Color, OPT>
{
    using Base = Component<CompId::COLOR, vcl::Color, OPT>;

public:
    /**
     * @brief Expose the type of the Color.
     */
    using ColorType = vcl::Color;

    /* Constructors */

    /**
     * @brief Initializes the Color component.
     */
    Color() = default;

    /* Member functions */

    /**
     * @brief Returns a reference to the color of the element.
     * Propagates constness automatically from the caller (proxy).
     * @return a reference to the color of the element.
     */
    decltype(auto) color(this auto&& self) { return Base::compData(self); }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Aliases */

using OptionalColor = Color<true>;

/* Concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the Color
 * component (COMP_ID = CompId::COLOR).
 *
 * @tparam T: The type to be tested for conformity to HasColor.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasColor = HasComponent<CompId::COLOR, T>;

/**
 * @brief A concept that checks whether a type T has the Color component and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasOptionalColor.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalColor =
    HasColor<T> && HasOptionalComponent<CompId::COLOR, T>;

/* Implementation of member functions */

template<bool OPT>
template<typename OtherElement>
void Color<OPT>::importFrom(this auto&& self, const OtherElement& other, bool)
{
    if constexpr (HasColor<OtherElement>) {
        if (other.template isComponentAvailable<CompId::COLOR>()) {
            self.color() = other.color();
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_COLOR_H
