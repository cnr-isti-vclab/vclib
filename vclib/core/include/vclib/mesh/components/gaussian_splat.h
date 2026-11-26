// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_GAUSSIAN_SPLAT_H
#define VCL_MESH_COMPONENTS_GAUSSIAN_SPLAT_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The GaussianSplat class represents a component that stores a 3D
 * Gaussian Splat (e.g. scale, rotation, opacity, and spherical harmonics) that
 * will be part of an Element (e.g. Vertex).
 *
 * Allows to get and set an object that represents a 3D Gaussian Splat.
 *
 * For example, if you have a Vertex Element `v` with the GaussianSplat
 * component, you'll be able to access this component member functions from `v`:
 *
 * @code{.cpp}
 * auto& gs = v.gaussianSplat();
 * @endcode
 *
 * @tparam Scalar: The scalar type used for the coordinates and parameters of
 * the Gaussian Splat (e.g. float or double).
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<typename Scalar, bool OPT = false>
class GaussianSplat :
        public Component<
            CompId::GAUSSIAN_SPLAT,
            vcl::GaussianSplat<Scalar>,
            OPT>
{
    using Base =
        Component<CompId::GAUSSIAN_SPLAT, vcl::GaussianSplat<Scalar>, OPT>;

public:
    /**
     * @brief Exposes the type of the Gaussian Splat.
     */
    using GaussianSplatType = vcl::GaussianSplat<Scalar>;

    /**
     * @brief Exposes the scalar type used by the Gaussian Splat.
     */
    using ScalarType = Scalar;

    /* Constructors */

    /**
     * @brief Initializes the Gaussian Splat with default values.
     */
    GaussianSplat() = default;

    /* Member functions */

    /**
     * @brief Returns a reference of the Gaussian Splat of the element.
     * @return a reference of the Gaussian Splat of the element.
     */
    decltype(auto) gaussianSplat(this auto&& self)
    {
        return Base::compData(self);
    }

protected:
    // Component interface functions
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& e, bool = true);
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * GaussianSplat component (COMP_ID = CompId::GAUSSIAN_SPLAT).
 *
 * @tparam T: The type to be tested for conformity to HasGaussianSplat.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasGaussianSplat = HasComponent<CompId::GAUSSIAN_SPLAT, T>;

/**
 * @brief A concept that checks whether a type T has the GaussianSplat
 * component and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasOptionalGaussianSplat.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalGaussianSplat =
    HasGaussianSplat<T> &&
    HasOptionalComponent<CompId::GAUSSIAN_SPLAT, T>;

/* importFrom function */

template<typename Scalar, bool OPT>
template<typename OtherElement>
void GaussianSplat<Scalar, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& e,
    bool)
{
    if constexpr (HasGaussianSplat<OtherElement>) {
        if (e.template isComponentAvailable<CompId::GAUSSIAN_SPLAT>()) {
            self.gaussianSplat() = e.gaussianSplat().template cast<Scalar>();
        }
    }
}

/* Specialization Aliases */

/**
 * @brief The GaussianSplatf class is an alias of the GaussianSplat component
 * that uses float precision.
 *
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<bool OPT = false>
using GaussianSplatf = GaussianSplat<float, OPT>;

/**
 * @brief The GaussianSplatd class is an alias of the GaussianSplat component
 * that uses double precision.
 *
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<bool OPT = false>
using GaussianSplatd = GaussianSplat<double, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_GAUSSIAN_SPLAT_H
