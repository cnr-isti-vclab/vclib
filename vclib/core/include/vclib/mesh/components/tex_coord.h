// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_TEX_COORD_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The TexCoord class represents a component that stores a texture
 * coordinate.
 *
 * It exposes a vcl::TexCoord object, that stores a texture coordinate.
 *
 * For example, if you have a Vertex Element `v` with the TexCoord component,
 * you'll be able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * auto texcoord = v.texCoord();
 * @endcode
 *
 * @tparam Scalar: The scalar type of the texture coordinate values.
 * @tparam OPT: If true, the component will be optional.
 *
 * @ingroup components
 */
template<typename Scalar, bool OPT = false>
class TexCoord : public Component<CompId::TEX_COORD, vcl::TexCoord<Scalar>, OPT>
{
    using Base = Component<CompId::TEX_COORD, vcl::TexCoord<Scalar>, OPT>;

public:
    /**
     * @brief Expose the type of the TexCoord.
     */
    using TexCoordType = vcl::TexCoord<Scalar>;

    /* Constructors */

    /**
     * @brief Initilizes the Texture Coordinate to (0, 0).
     */
    TexCoord() = default;

    /* Member functions */

    /**
     * @brief Returns a reference of the tex coord of the element.
     * @return a reference of the tex coord of the element.
     */
    decltype(auto) texCoord(this auto&& self) { return Base::compData(self); }

protected:
    // Component interface functions
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the TexCoord
 * component (COMP_ID = CompId::TEX_COORD).
 *
 * @tparam T: The type to be tested for conformity to HasTexCoord.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTexCoord = HasComponent<CompId::TEX_COORD, T>;

/**
 * @brief A concept that checks whether a type T has the TexCoord component and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasOptionalTexCoord.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalTexCoord =
    HasTexCoord<T> && HasOptionalComponent<CompId::TEX_COORD, T>;

/* Detector function to check if a class has TexCoord available */

/* Implementation of member functions */

template<typename Scalar, bool OPT>
template<typename OtherElement>
void TexCoord<Scalar, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool)
{
    if constexpr (HasTexCoord<OtherElement>) {
        if (other.template isComponentAvailable<CompId::TEX_COORD>()) {
            self.texCoord() = other.texCoord().template cast<Scalar>();
        }
    }
}

/* Specialization Aliases */

/**
 * The TexCoordf class is an alias of the TexCoord component that uses float as
 * scalar type.
 *
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<bool OPT = false>
using TexCoordf = TexCoord<float, OPT>;

/**
 * The TexCoordd class is an alias of the TexCoord component that uses double as
 * scalar type.
 *
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<bool OPT = false>
using TexCoordd = TexCoord<double, OPT>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEX_COORD_H
