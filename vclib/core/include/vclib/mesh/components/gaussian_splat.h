// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_GAUSSIAN_SPLAT_H
#define VCL_MESH_COMPONENTS_GAUSSIAN_SPLAT_H

#include "base/component.h"
#include "base/predicates.h"

#include <vclib/space/core.h>

namespace vcl {

// PlaceHolder
// TODO: remove this...
template<typename Scalar>
class GaussianSplat
{
};

} // namespace vcl

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
class GaussianSplat :
        public Component<
            GaussianSplat<Scalar, ParentElemType, OPT>,
            CompId::GAUSSIAN_SPLAT,
            vcl::GaussianSplat<Scalar>,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        GaussianSplat<Scalar, ParentElemType, OPT>,
        CompId::GAUSSIAN_SPLAT,
        vcl::GaussianSplat<Scalar>,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /**
     * @brief Exposes the type of the Gaussian Splat.
     */
    using GaussianSplatType = vcl::GaussianSplat<Scalar>;

    /* Constructors */

    /**
     * @brief Initializes the Gaussian Splat with default values.
     */
    GaussianSplat() = default;

    /* Member functions */

    /**
     * @brief Returns a const reference of the Gaussian Splat of the element.
     * @return a const reference of the Gaussian Splat of the element.
     */
    const GaussianSplatType& gaussianSplat() const { return Base::data(); }

    /**
     * @brief Returns a reference of the Gaussian Splat of the element.
     * @return a reference of the Gaussian Splat of the element.
     */
    GaussianSplatType& gaussianSplat() { return Base::data(); }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const
    {
        // TODO: Uncomment when GaussianSplat serialization is implemented
        // gaussianSplat().serialize(os);
    }

    void deserialize(std::istream& is)
    {
        // TODO: Uncomment when GaussianSplat deserialization is implemented
        // gaussianSplat().deserialize(is);
    }
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (that should be an Element) has
 * the GaussianSplat component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::GaussianSplat, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * GaussianSplat component and the vertical OptionalGaussianSplat component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides the GaussianSplat component at runtime (it is
 * guaranteed only that the proper member functions are available at compile
 * time).
 *
 * @tparam T: The type to be tested for conformity to the HasGaussianSplat.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasGaussianSplat =
    TTB::IsDerivedFromSpecializationOfV<T, GaussianSplat>;

/**
 * @brief A concept that checks whether a type T (that should be an Element) has
 * the GaussianSplat component (inherits from it), and that the component is
 * optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalGaussianSplat.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalGaussianSplat =
    HasGaussianSplat<T> &&
    IsOptionalComponent<typename RemoveRef<T>::GaussianSplat>;

/* importFrom function */

template<typename Scalar, typename ParentElemType, bool OPT>
template<typename Element>
void GaussianSplat<Scalar, ParentElemType, OPT>::importFrom(
    const Element& e,
    bool)
{
    if constexpr (HasGaussianSplat<Element>) {
        if (isGaussianSplatAvailableOn(e)) {
            gaussianSplat() = e.gaussianSplat().template cast<Scalar>();
        }
    }
}

/* Detector function to check if a class has GaussianSplat available */

/**
 * @brief Checks if the given Element has GaussianSplat component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has GaussianSplat component available, `false`
 * otherwise.
 */
bool isGaussianSplatAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::GAUSSIAN_SPLAT>(element);
}

/* Specialization Aliases */

/**
 * @brief The GaussianSplatf class is an alias of the GaussianSplat component
 * that uses float precision.
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
using GaussianSplatf = GaussianSplat<float, ElementType, OPT>;

/**
 * @brief The GaussianSplatd class is an alias of the GaussianSplat component
 * that uses double precision.
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
using GaussianSplatd = GaussianSplat<double, ElementType, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_GAUSSIAN_SPLAT_H
