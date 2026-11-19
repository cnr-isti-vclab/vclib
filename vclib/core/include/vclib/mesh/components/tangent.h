// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_TANGENT_H
#define VCL_MESH_COMPONENTS_TANGENT_H

#include "base/component_predicates.h"
#include "normal.h"

#include "../exceptions.h"

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <utility>

namespace vcl::comp {

/**
 * @brief The Tangent class represents a component that stores a tangent vector
 * and a boolean indicating handedness (right-handed if true) of a mesh element.
 *
 * For example, if you have a Vertex Element `v` with the Tangent component,
 * you'll be able to access this component member functions from `v`:
 *
 * @code{.cpp}
 * auto& tn = v.tangent();
 * bool rh = v.tangentRightHanded();
 * auto btn = v.bitangent(); // requires Normal component
 * @endcode
 *
 * @tparam P: The type of the tangent vector. This template argument must satisfy
 * the PointConcept.
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<PointConcept P, bool OPT = false>
class Tangent : public Component<CompId::TANGENT, std::pair<P, bool>, OPT>
{
    using Base = Component<CompId::TANGENT, std::pair<P, bool>, OPT>;

public:
    /**
     * @brief Expose the type of the Tangent vector.
     */
    using TangentType = P;

    /* Constructors */

    /**
     * @brief Default constructor.
     */
    Tangent() = default;

    /* Default value */

    /**
     * @brief Returns the default value for the Tangent component:
     * zero vector and right-handed (true).
     */
    static std::pair<P, bool> defaultValue()
    {
        return {P{}, true};
    }

    /* Member functions */

    /**
     * @brief Returns a reference to the tangent vector of the element.
     * Propagates constness automatically from the caller (proxy).
     * @return a reference to the tangent vector of the element.
     */
    decltype(auto) tangent(this auto&& self)
    {
        return (Base::compData(self).first);
    }

    /**
     * @brief Returns a reference to the boolean indicating if the tangent
     * is right-handed.
     * Propagates constness automatically from the caller (proxy).
     * @return a reference to the boolean indicating if the tangent is right-handed.
     */
    decltype(auto) tangentRightHanded(this auto&& self)
    {
        return (Base::compData(self).second);
    }

    /**
     * @brief Computes and returns the bitangent vector using the tangent and
     * normal vectors.
     *
     * The bitangent is computed as the cross product between the tangent and
     * the normal vectors. The direction of the bitangent depends on the
     * handedness of the tangent.
     *
     * @throws vcl::MissingComponentException if the Normal component is not
     * available or not enabled on the element.
     *
     * @return The computed bitangent vector.
     */
    P bitangent(this auto&& self)
    {
        if constexpr (HasNormal<decltype(self)>) {
            if (self.template isComponentAvailable<CompId::NORMAL>()) {
                const auto& n = self.normal();
                if constexpr (std::is_same_v<
                                  typename P::ScalarType,
                                  typename std::remove_cvref_t<
                                      decltype(n)>::ScalarType>) {
                    return self.tangentRightHanded() ? n.cross(self.tangent()) :
                                                       self.tangent().cross(n);
                }
                else {
                    auto nCast = n.template cast<typename P::ScalarType>();
                    return self.tangentRightHanded() ?
                               nCast.cross(self.tangent()) :
                               self.tangent().cross(nCast);
                }
            }
        }
        throw MissingComponentException(
            "Normal Component is not available on this element.");
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
 * Tangent component (COMP_ID = CompId::TANGENT).
 *
 * @tparam T: The type to be tested for conformity to HasTangent.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTangent = HasComponent<CompId::TANGENT, T>;

/**
 * @brief A concept that checks whether a type T has the Tangent component
 * and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasOptionalTangent.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalTangent =
    HasTangent<T> && HasOptionalComponent<CompId::TANGENT, T>;

/* importFrom function */

template<PointConcept P, bool OPT>
template<typename OtherElement>
void Tangent<P, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& e,
    bool)
{
    using ScalarType = typename TangentType::ScalarType;
    if constexpr (HasTangent<OtherElement>) {
        if (e.template isComponentAvailable<CompId::TANGENT>()) {
            self.tangent()            = e.tangent().template cast<ScalarType>();
            self.tangentRightHanded() = e.tangentRightHanded();
        }
    }
}

/* Specialization Aliases */

/**
 * @brief The Tangent3 class is an alias of the Tangent component that uses 3
 * dimensional Points.
 *
 * @tparam Scalar: The scalar type of the point.
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<typename Scalar, bool OPT = false>
using Tangent3 = Tangent<Point3<Scalar>, OPT>;

/**
 * @brief The Tangent3f class is an alias of the Tangent component that uses
 * 3 dimensional Points with float precision.
 *
 * @ingroup components
 */
using Tangent3f = Tangent3<float, false>;

/**
 * @brief The Tangent3d class is an alias of the Tangent component that uses
 * 3 dimensional Points with double precision.
 *
 * @ingroup components
 */
using Tangent3d = Tangent3<double, false>;

template<typename Scalar>
using OptionalTangent3 = Tangent3<Scalar, true>;

/**
 * @brief The OptionalTangent3f class is an alias of the Tangent component that uses
 * 3 dimensional Points with float precision and is optional.
 *
 * @ingroup components
 */
using OptionalTangent3f = Tangent3<float, true>;

/**
 * @brief The OptionalTangent3d class is an alias of the Tangent component that uses
 * 3 dimensional Points with double precision and is optional.
 *
 * @ingroup components
 */
using OptionalTangent3d = Tangent3<double, true>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TANGENT_H
