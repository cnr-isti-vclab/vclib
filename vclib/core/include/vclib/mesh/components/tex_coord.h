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

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_TEX_COORD_H

#include "base/component.h"
#include "concepts/tex_coord.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The TexCoord class represents a component that stores a texture
 * coordinate.
 *
 * It exposes a vcl::TexCoordIndexed object, that stores a texture coordinate
 * and an id of the texture to use.
 *
 * For example, if you have a Vertex Element `v` with the TexCoord component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto texcoord = v.texCoord();
 * @endcode
 *
 * @tparam Scalar: The scalar type of the texture coordinate values.
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, typename ParentElemType = void, bool OPT = false>
class TexCoord :
        public Component<
            TexCoord<Scalar, ParentElemType, OPT>,
            CompId::TEX_COORD,
            vcl::TexCoordIndexed<Scalar>,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        TexCoord<Scalar, ParentElemType, OPT>,
        CompId::TEX_COORD,
        vcl::TexCoordIndexed<Scalar>,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Expose the type of the TexCoord.
     */
    using TexCoordType = vcl::TexCoordIndexed<Scalar>;

    /* Constructors */

    /**
     * @brief Initilizes the Texture Coordinate to (0, 0).
     */
    TexCoord() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the tex coord of the element.
     * @return a const reference of the tex coord of the element.
     */
    const TexCoordType& texCoord() const { return Base::data(); }

    /**
     * @brief Returns a reference of the tex coord of the element.
     * @return a reference of the tex coord of the element.
     */
    TexCoordType& texCoord() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        if constexpr (HasTexCoord<Element>) {
            if (isTexCoordAvailableOn(e)) {
                texCoord() = e.texCoord().template cast<Scalar>();
            }
        }
    }

    void serialize(std::ostream& os) const { texCoord().serialize(os); }

    void deserialize(std::istream& is) { texCoord().deserialize(is); }
};

/* Detector function to check if a class has TexCoord available */

/**
 * @brief Checks if the given Element has TexCoord component
 * available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has TexCoord component available,
 * `false` otherwise.
 */
bool isTexCoordAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::TEX_COORD>(element);
}

/* Specialization Aliases */

/**
 * The TexCoordf class is an alias of the TexCoord component that uses float as
 * scalar type.
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
using TexCoordf = TexCoord<float, ElementType, OPT>;

/**
 * The TexCoordd class is an alias of the TexCoord component that uses double as
 * scalar type.
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
using TexCoordd = TexCoord<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEX_COORD_H
