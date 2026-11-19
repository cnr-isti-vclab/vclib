// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

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
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<typename Scalar, bool OPT = false>
class PrincipalCurvature :
        public Component<
            CompId::PRINCIPAL_CURVATURE,
            vcl::PrincipalCurvature<Scalar>,
            OPT>
{
    using Base = Component<
        CompId::PRINCIPAL_CURVATURE,
        vcl::PrincipalCurvature<Scalar>,
        OPT>;

public:
    /**
     * @brief Expose the type of the principal curvature object.
     */
    using PrincipalCurvatureType = vcl::PrincipalCurvature<Scalar>;

    /* Constructors */

    /**
     * @brief Initializes the PrincipalCurvature values to 0.
     */
    PrincipalCurvature() = default;

    /* Member functions */

    /**
     * @brief Returns a reference of the principal curvature of the element.
     * @return a reference of the principal curvature of the element.
     */
    decltype(auto) principalCurvature(this auto&& self)
    {
        return Base::compData(self);
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& e, bool = true);
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the PrincipalCurvature
 * component (COMP_ID = CompId::PRINCIPAL_CURVATURE).
 *
 * @tparam T: The type to be tested for conformity to HasPrincipalCurvature.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasPrincipalCurvature = HasComponent<CompId::PRINCIPAL_CURVATURE, T>;

/**
 * @brief A concept that checks whether a type T has the PrincipalCurvature
 * component and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasPrincipalCurvature.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalPrincipalCurvature =
    HasPrincipalCurvature<T> && HasOptionalComponent<CompId::PRINCIPAL_CURVATURE, T>;

/* Detector function to check if a class has PrincipalCurvature available */

/* importFrom function */

template<typename Scalar, bool OPT>
template<typename OtherElement>
void PrincipalCurvature<Scalar, OPT>::importFrom(
    this auto&& self,
    const OtherElement& e,
    bool)
{
    if constexpr (HasPrincipalCurvature<OtherElement>) {
        if (e.template isComponentAvailable<CompId::PRINCIPAL_CURVATURE>()) {
            self.principalCurvature() =
                e.principalCurvature().template cast<Scalar>();
        }
    }
}

/* Specialization Aliases */

/**
 * The PrincipalCurvaturef class is an alias of the PrincipalCurvature component
 * that uses float as scalar type.
 *
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<bool OPT = false>
using PrincipalCurvaturef = PrincipalCurvature<float, OPT>;

/**
 * The PrincipalCurvatured class is an alias of the PrincipalCurvature component
 * that uses double as scalar type.
 *
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<bool OPT = false>
using PrincipalCurvatured = PrincipalCurvature<double, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_H
