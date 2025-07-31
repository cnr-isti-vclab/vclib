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

#ifndef VCL_MESH_COMPONENTS_COLOR_H
#define VCL_MESH_COMPONENTS_COLOR_H

#include "base/component.h"
#include "base/predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The Color class represents a RGBA color that will be part of an
 * Element (e.g. Vertex, Face, ...) or Mesh.
 *
 * Allows to get and set an object of type Color.
 *
 * For example, if you have a Vertex Element `v` with the Color component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * vcl::Color c = v.color();
 * @endcode
 *
 * @note This component can be both used for Elements and Meshes.
 *
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ParentElemType = void, bool OPT = false>
class Color :
        public Component<
            Color<ParentElemType, OPT>,
            CompId::COLOR,
            vcl::Color,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        Color<ParentElemType, OPT>,
        CompId::COLOR,
        vcl::Color,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Expose the type of the Color.
     */
    using ColorType = vcl::Color;

    /* Constructors */

    /**
     * @brief Initilizes the color to black (with alpha 255).
     */
    Color() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the color of the element.
     * @return a const reference of the color of the element.
     */
    const vcl::Color& color() const { return Base::data(); }

    /**
     * @brief Returns a reference pf the color of the element.
     * @return a reference pf the color of the element.
     */
    vcl::Color& color() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const { color().serialize(os); }

    void deserialize(std::istream& is) { color().deserialize(is); }
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (that should be a Element or a
 * Mesh) has the Color component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from vcl::comp::Color,
 * with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal Color
 * component and the vertical OptionalColor component, therefore it does not
 * guarantee that a template Element type that satisfies this concept provides
 * Color component at runtime (it is guaranteed only that the proper member
 * functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasColor.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasColor = TB::IsDerivedFromSpecializationOfV<T, Color>;

/**
 * @brief A concept that checks whether a type T (that should be a Element or a
 * Mesh) has the Color component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the HasOptionalColor.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalColor =
    HasColor<T> && IsOptionalComponent<typename RemoveRef<T>::Color>;

/* importFrom function */

template<typename ParentElemType, bool OPT>
template<typename Element>
void Color<ParentElemType, OPT>::importFrom(const Element& e, bool)
{
    if constexpr (HasColor<Element>) {
        if (isColorAvailableOn(e)) {
            color() = e.color();
        }
    }
}

/* Detector function to check if a class has Color available */

/**
 * @brief Checks if the given Element has Color component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has Color component available, `false`
 * otherwise.
 */
bool isColorAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::COLOR>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_COLOR_H
