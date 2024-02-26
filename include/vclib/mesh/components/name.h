/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_MESH_COMPONENTS_NAME_H
#define VCL_MESH_COMPONENTS_NAME_H

#include <vclib/concepts/mesh/components/name.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The Name component class represents a simple name stored as string.
 * This class is usually used as a component of a Mesh.
 *
 * The member functions of this class will be available in the instance of any
 * Element or Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the Name component, you'll be able
 * to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * m.name();
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
class Name :
        public Component<
            Name<ElementType, OPT>,
            CompId::NAME,
            std::string,
            ElementType,
            OPT>
{
    using Base = Component<
        Name<ElementType, OPT>,
        CompId::NAME,
        std::string,
        ElementType,
        OPT>;

public:
    /* Constructors */

    /**
     * @brief Initilizes an empty name.
     */
    Name() = default;

    /* Member functions */

    /**
     * @brief Returns the name of this object.
     * @return The name of this object.
     */
    std::string& name() { return Base::data(); }

    /**
     * @brief Returns the name of this object.
     * @return The name of this object.
     */
    const std::string& name() const { return Base::data(); }

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e)
    {
        if constexpr (HasName<Element>) {
            name() = e.name();
        }
    }
};

/* Detector function to check if a class has Name available */

/**
 * @brief Checks if the given Element/Mesh has Name component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has Name component available, `false`
 * otherwise.
 */
bool isNameAvailableOn(const ElementOrMeshConcept auto& element)
{
    return isComponentAvailableOn<CompId::NAME>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_NAME_H
