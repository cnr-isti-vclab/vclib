// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_QUALITY_H
#define VCL_MESH_COMPONENTS_QUALITY_H

#include "base/component_predicates.h"

#include <vclib/base.h>

namespace vcl::comp {

/**
 * @brief The Quality class represents a component that stores the quality of a
 * mesh element, that is represented by a scalar.
 *
 * This scalar is actually used for a lot of different purposes, depending on
 * the applications.
 *
 * For example, if you have a Vertex Element `v` with the Quality component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto q = v.quality();
 * @endcode
 *
 * @tparam Scalar: The scalar type of the Quality value.
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<typename Scalar, bool OPT = false>
class Quality : public Component<CompId::QUALITY, Scalar, OPT>
{
    using Base = Component<CompId::QUALITY, Scalar, OPT>;

public:
    /**
     * @brief Exposes the scalar used as Quality type.
     */
    using QualityType = Scalar;

    /* Constructors */

    /**
     * @brief Initializes the Quality value to 0.
     */
    Quality() = default;

    /* Member functions */

    /**
     * @brief Returns a reference of the quality of the element.
     * @return a reference of the quality of the element.
     */
    decltype(auto) quality(this auto&& self) { return Base::compData(self); }

protected:
    // Component interface functions
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& e, bool = true);
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * Quality component (COMP_ID = CompId::QUALITY).
 *
 * @tparam T: The type to be tested for conformity to HasQuality.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasQuality = HasComponent<CompId::QUALITY, T>;

/**
 * @brief A concept that checks whether a type T has the Quality component
 * and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasOptionalQuality.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalQuality =
    HasQuality<T> && HasOptionalComponent<CompId::QUALITY, T>;

/* Detector function to check if a class has Quality available */

/* importFrom function */

template<typename Scalar, bool OPT>
template<typename OtherElement>
void Quality<Scalar, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& e,
    bool)
{
    if constexpr (HasQuality<OtherElement>) {
        if (e.template isComponentAvailable<CompId::QUALITY>()) {
            self.quality() = static_cast<Scalar>(e.quality());
        }
    }
}

/* Specialization Aliases */

/**
 * The Qualityf class is an alias of the Quality component that uses float as
 * scalar type.
 *
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<bool OPT = false>
using Qualityf = Quality<float, OPT>;

/**
 * The Qualityd class is an alias of the Quality component that uses double as
 * scalar type.
 *
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<bool OPT = false>
using Qualityd = Quality<double, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_QUALITY_H
