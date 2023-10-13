/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_MESH_COMPONENTS_NORMAL_H
#define VCL_MESH_COMPONENTS_NORMAL_H

#include <vclib/concepts/mesh/components/normal.h>
#include <vclib/space/point.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The Normal class represents a N-Dimensional normal vector that will be
 * part of an Element (e.g. Vertex, Face...).
 *
 * Allows to get and set an object that represents a normal. The type of this
 * object must satisfy the PointConcept.
 *
 * For example, if you have a Vertex Element `v` with the Normal component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto n = v.normal();
 * @endcode
 *
 * @tparam P: The type of the normal. This template argument must be a type that
 * satisfies the PointConcept.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<PointConcept P, typename ElementType = void, bool OPT = false>
class Normal :
        public Component<
            Normal<P, ElementType, OPT>,
            NORMAL,
            P,
            ElementType,
            OPT>
{
    using Base =
        Component<Normal<P, ElementType, OPT>, NORMAL, P, ElementType, OPT>;

public:
    /**
     * @brief Expose the type of the Normal.
     */
    using NormalType = P;

    /* Constructors */

    /**
     * @brief Initilizes the Normal to (0, 0, 0).
     */
    Normal() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the normal of the element.
     * @return a const reference of the normal of the element.
     */
    const P& normal() const { return Base::data(); }

    /**
     * @brief Returns a reference of the normal of the element.
     * @return a reference of the normal of the element.
     */
    P& normal() { return Base::data(); }

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e)
    {
        if constexpr (HasNormal<Element>) {
            if (isNormalAvailableOn(e)) {
                normal() =
                    e.normal().template cast<typename NormalType::ScalarType>();
            }
        }
    }
};

/* Detector function to check if a class has Normal available */

/**
 * @brief Checks if the given Element has Normal component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementConcept.
 * @return `true` if the element has Normal component available, `false`
 * otherwise.
 */
bool isNormalAvailableOn(const ElementConcept auto& element)
{
    return isComponentAvailableOn<NORMAL>(element);
}

/* Specialization Aliases */

/**
 * @brief The Normal3 class is an alias of the Normal component that uses 3
 * dimensional Points.
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
using Normal3 = Normal<Point3<Scalar>, ElementType, OPT>;

/**
 * @brief The Normal3f class is an alias of the Normal component that uses
 * 3 dimensional Points with float precision.
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
using Normal3f = Normal3<float, ElementType, OPT>;

/**
 * @brief The Normal3d class is an alias of the Normal component that uses
 * 3 dimensional Points with double precision.
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
using Normal3d = Normal3<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_NORMAL_H
