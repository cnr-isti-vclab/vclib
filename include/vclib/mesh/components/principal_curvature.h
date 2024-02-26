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

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H

#include <vclib/concepts/mesh/components/principal_curvature.h>
#include <vclib/space/principal_curvature.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The PrincipalCurvature class represents a component that stores
 * the principal curvature directions and values at a point on a surface.
 * This component could be part of Vertices or Faces.
 *
 * It exposes a vcl::PrincipalCurvature object, that stores the two principal
 * curvature directions (`maxDir()` and `minDir()`, also note as k1 and k2
 * respectively) and the maximum and minimum values of the curvature
 * (`maxValue()` and `minValue()`).
 *
 * For example, if you have a Vertex Element `v` with the PrincipalCurvature
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * auto k1 = v.principalCurvature().maxDir();
 * auto maxv = v.principalCurvature().maxValue();
 * @endcode
 *
 * @tparam Scalar: The scalar type of the principal curvature values.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool OPT = false>
class PrincipalCurvature :
        public Component<
            PrincipalCurvature<Scalar, ElementType, OPT>,
            CompId::PRINCIPAL_CURVATURE,
            vcl::PrincipalCurvature<Scalar>,
            ElementType,
            OPT>
{
    using Base = Component<
        PrincipalCurvature<Scalar, ElementType, OPT>,
        CompId::PRINCIPAL_CURVATURE,
        vcl::PrincipalCurvature<Scalar>,
        ElementType,
        OPT>;

public:
    /**
     * @brief Expose the type of the principal curvature object.
     */
    using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

    /* Constructors */

    /**
     * @brief Initilizes the PrincipalCurvature values to 0.
     */
    PrincipalCurvature() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the principal curvature of the
     * element.
     * @return a const reference of the principal curvature of the element.
     */
    const PrincipalCurvatureType& principalCurvature() const
    {
        return Base::data();
    }

    /**
     * @brief Returns a reference of the principal curvature of the element.
     * @return a reference of the principal curvature of the element.
     */
    PrincipalCurvatureType& principalCurvature() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e)
    {
        if constexpr (HasPrincipalCurvature<Element>) {
            if (isPrincipalCurvatureAvailableOn(e)) {
                principalCurvature() =
                    e.principalCurvature().template cast<Scalar>();
            }
        }
    }
};

/* Detector function to check if a class has PrincipalCurvature available */

/**
 * @brief Checks if the given Element has PrincipalCurvature component
 * available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementConcept.
 * @return `true` if the element has PrincipalCurvature component available,
 * `false` otherwise.
 */
bool isPrincipalCurvatureAvailableOn(const ElementConcept auto& element)
{
    return isComponentAvailableOn<CompId::PRINCIPAL_CURVATURE>(element);
}

/* Specialization Aliases */

/**
 * The PrincipalCurvaturef class is an alias of the PrincipalCurvature component
 * that uses float as scalar type.
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
using PrincipalCurvaturef = PrincipalCurvature<float, ElementType, OPT>;

/**
 * The PrincipalCurvatured class is an alias of the PrincipalCurvature component
 * that uses double as scalar type.
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
using PrincipalCurvatured = PrincipalCurvature<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
