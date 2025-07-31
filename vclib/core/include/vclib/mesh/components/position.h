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

#ifndef VCL_MESH_COMPONENTS_POSITION_H
#define VCL_MESH_COMPONENTS_POSITION_H

#include "base/component.h"

#include <vclib/mesh/concepts/components/position.h>

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The Position class represents a N-Dimensional point that will be
 * part of an Element (e.g. Vertex...).
 *
 * Allows to get and set an object that represents a position in a space. The
 * type of this object must satisfy the PointConcept.
 *
 * For example, if you have a Vertex Element `v` with the Position component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto pos = v.position();
 * @endcode
 *
 * @tparam P: The type of the position. This template argument must be a type
 * that satisfies the PointConcept.
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<PointConcept P, typename ParentElemType = void, bool OPT = false>
class Position :
        public Component<
            Position<P, ParentElemType, OPT>,
            CompId::POSITION,
            P,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        Position<P, ParentElemType, OPT>,
        CompId::POSITION,
        P,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Expose the type of the Position.
     */
    using PositionType = P;

    /* Constructors */

    /**
     * @brief Initilizes the Position to (0, 0, 0).
     */
    Position() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the position of the element.
     * @return a const reference of the position of the element.
     */
    const P& position() const { return Base::data(); }

    /**
     * @brief Returns a reference of the position of the element.
     * @return a reference of the position of the element.
     */
    P& position() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& v, bool = true)
    {
        using ScalarType = PositionType::ScalarType;
        if constexpr (HasPosition<Element>) {
            if (isPositionAvailableOn(v)) {
                position() = v.position().template cast<ScalarType>();
            }
        }
    }

    void serialize(std::ostream& os) const { position().serialize(os); }

    void deserialize(std::istream& is) { position().deserialize(is); }
};

/* Detector function to check if a class has Position available */

/**
 * @brief Checks if the given Element has Position component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has Position component available, `false`
 * otherwise.
 */
bool isPositionAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::POSITION>(element);
}

/* Specialization Aliases */

/**
 * @brief The Position3 class is an alias of the Position component that
 * uses 3 dimensional Points.
 *
 * @tparam Scalar: The scalar type of the point.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool OPT = false>
using Position3 = Position<Point3<Scalar>, ElementType, OPT>;

/**
 * @brief The Position3f class is an alias of the Position component that
 * uses 3 dimensional Points with float precision.
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
using Position3f = Position3<float, ElementType, OPT>;

/**
 * @brief The Position3d class is an alias of the Position component that
 * uses 3 dimensional Points with double precision.
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
using Position3d = Position3<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_POSITION_H
