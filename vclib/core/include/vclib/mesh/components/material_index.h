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

#ifndef VCL_MESH_COMPONENTS_MATERIAL_INDEX_H
#define VCL_MESH_COMPONENTS_MATERIAL_INDEX_H

#include "base/component.h"
#include "base/predicates.h"

namespace vcl::comp {

template<typename ParentElemType = void, bool OPT = false>
class MaterialIndex :
        public Component<
            MaterialIndex<ParentElemType, OPT>,
            CompId::MATERIAL_INDEX,
            ushort,
            ParentElemType,
            !std::is_same_v<ParentElemType, void>,
            OPT>
{
    using Base = Component<
        MaterialIndex<ParentElemType, OPT>,
        CompId::MATERIAL_INDEX,
        ushort,
        ParentElemType,
        !std::is_same_v<ParentElemType, void>,
        OPT>;

public:
    /* Constructors */

    /**
     * @brief Constructor that initializes the material index to USHORT_NULL.
     */
    MaterialIndex()
    {
        if constexpr (!Base::IS_VERTICAL) {
            init();
        }
    }

    /**
     * @private
     * @brief Initializes the material index to USHORT_NULL.
     *
     * It is made in the init function since the component could be not
     * available during construction (e.g. if the component is optional and not
     * enabled).
     *
     * This member function is hidden by the element that inherits this class.
     */
    void init() { materialIndex() = USHORT_NULL; }

    /* Member functions */

    /**
     * @brief Returns a reference to the material index.
     * @return a reference to the material index.
     */
    ushort& materialIndex() { return Base::data(); }

    /**
     * @brief Returns the value of the material index.
     * @return the value of the material index.
     */
    ushort materialIndex() const { return Base::data(); }

    /**
     * @brief Returns a reference to the material index.
     *
     * Same as materialIndex(). Left for compatibility reasons.
     *
     * @return a reference to the material index.
     */
    ushort& textureIndex() { return Base::data(); }

    /**
     * @brief Returns the value of the material index.
     *
     * Same as materialIndex(). Left for compatibility reasons.
     *
     * @return the value of the material index.
     */
    ushort textureIndex() const { return Base::data(); }

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e, bool = true);

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, materialIndex());
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, materialIndex());
    }
};

/* concepts */

/**
 * @brief A concept that checks whether a type T (that should be a Element or a
 * Mesh) has the MaterialIndex component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::MaterialIndex, with any template arguments.
 *
 * Note that this concept does not discriminate between the Horizontal
 * MaterialIndex component and the vertical OptionalMaterialIndex component,
 * therefore it does not guarantee that a template Element type that satisfies
 * this concept provides MaterialIndex component at runtime (it is guaranteed
 * only that the proper member functions are available at compile time).
 *
 * @tparam T: The type to be tested for conformity to the HasMaterialIndex.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasMaterialIndex = TB::IsDerivedFromSpecializationOfV<T, MaterialIndex>;

/**
 * @brief A concept that checks whether a type T (that should be a Element or a
 * Mesh) has the MaterialIndex component (inherits from it), and that the
 * component is optional.
 *
 * @tparam T: The type to be tested for conformity to the
 * HasOptionalMaterialIndex.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalMaterialIndex =
    HasMaterialIndex<T> &&
    IsOptionalComponent<typename RemoveRef<T>::MaterialIndex>;

/* importFrom function */

template<typename ParentElemType, bool OPT>
template<typename Element>
void MaterialIndex<ParentElemType, OPT>::importFrom(const Element& e, bool)
{
    if constexpr (HasMaterialIndex<Element>) {
        if (isMaterialIndexAvailableOn(e)) {
            materialIndex() = e.materialIndex();
        }
    }
}

/* Detector function to check if a class has MaterialIndex available */

/**
 * @brief Checks if the given Element has the MaterialIndex component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has the MaterialIndex component available,
 * `false` otherwise.
 */
bool isMaterialIndexAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::MATERIAL_INDEX>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MATERIAL_INDEX_H
