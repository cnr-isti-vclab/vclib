// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_MATERIAL_INDEX_H
#define VCL_MESH_COMPONENTS_MATERIAL_INDEX_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::comp {

/**
 * @brief The MaterialIndex class represents an unsigned short index that
 * refers to a Material in the Mesh Materials component.
 *
 * It is typically part of an Element (e.g. Vertex, Face).
 *
 * For example, if you have a Face Element `f` with the MaterialIndex component,
 * you can access and modify its material index:
 *
 * @code{.cpp}
 * f.materialIndex() = 0;
 * ushort mi = f.materialIndex();
 * @endcode
 *
 * @tparam OPT: If true, the component is optional (runtime enable/disable).
 *
 * @ingroup components
 */
template<bool OPT = false>
class MaterialIndex : public Component<CompId::MATERIAL_INDEX, ushort, OPT>
{
    using Base = Component<CompId::MATERIAL_INDEX, ushort, OPT>;

public:
    /* Constructors */

    /**
     * @brief Default constructor.
     */
    MaterialIndex() = default;

    /**
     * @brief Returns the default initial value for MaterialIndex
     * (USHORT_NULL).
     * @return USHORT_NULL.
     */
    static constexpr ushort defaultValue() { return USHORT_NULL; }

    /* Member functions */

    /**
     * @brief Returns a reference to the material index of the element.
     * Propagates constness automatically from the caller (proxy).
     * @return A reference to the material index.
     */
    decltype(auto) materialIndex(this auto&& self)
    {
        return Base::compData(self);
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Aliases */

using OptionalMaterialIndex = MaterialIndex<true>;

/* Concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * MaterialIndex component (COMP_ID = CompId::MATERIAL_INDEX).
 *
 * @tparam T: The type to be tested for conformity to HasMaterialIndex.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasMaterialIndex = HasComponent<CompId::MATERIAL_INDEX, T>;

/**
 * @brief A concept that checks whether a type T has the MaterialIndex component
 * and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasOptionalMaterialIndex.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalMaterialIndex =
    HasMaterialIndex<T> && HasOptionalComponent<CompId::MATERIAL_INDEX, T>;

/* Implementation of member functions */

template<bool OPT>
template<typename OtherElement>
void MaterialIndex<OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool)
{
    if constexpr (HasMaterialIndex<OtherElement>) {
        if (other.template isComponentAvailable<CompId::MATERIAL_INDEX>()) {
            self.materialIndex() = other.materialIndex();
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MATERIAL_INDEX_H
