/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_COLOR_H
#define VCL_MESH_COMPONENTS_COLOR_H

#include <vclib/concepts/mesh/components/color.h>
#include <vclib/space/color.h>

#include "bases/component.h"

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
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
class Color :
        public Component<
            Color<ElementType, OPT>,
            COLOR,
            vcl::Color,
            ElementType,
            OPT>
{
    using Base =
        Component<Color<ElementType, OPT>, COLOR, vcl::Color, ElementType, OPT>;

public:
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
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e)
    {
        if constexpr (HasColor<Element>) {
            if (isColorAvailableOn(e)) {
                color() = e.color();
            }
        }
    }
};

/* Detector function to check if a class has Color available */

/**
 * @brief Checks if the given Element/Mesh has Color component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has Color component available, `false`
 * otherwise.
 */
bool isColorAvailableOn(const ElementOrMeshConcept auto& element)
{
    return isComponentAvailableOn<COLOR>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_COLOR_H
