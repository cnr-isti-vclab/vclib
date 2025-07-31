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

#ifndef VCL_MESH_COMPONENTS_MATERIALS_H
#define VCL_MESH_COMPONENTS_MATERIALS_H

#include "base/component.h"
#include "concepts/materials.h"

#include <vclib/space/core.h>

namespace vcl::comp {

namespace detail {

struct MData
{
    std::vector<Material> materials;
    std::string           meshPath;
};

} // namespace detail

class Materials :
        public Component<
            Materials,
            CompId::MATERIALS,
            detail::MData,
            void,
            false,
            false>
{
    using Base = Component<
        Materials,
        CompId::MATERIALS,
        detail::MData,
        void,
        false,
        false>;

public:
    /**
     * @brief Expose the type of the Material.
     */
    using MaterialType = Material;

    // iterators
    using MaterialIterator      = std::vector<Material>::iterator;
    using ConstMaterialIterator = std::vector<Material>::const_iterator;

    Materials() = default;

    /* Member functions */

    /**
     * @brief Returns the number of materials of the mesh.
     */
    uint materialNumber() const { return mats().size(); }

    /**
     * @brief Returns the mesh base path.
     */
    const std::string& meshBasePath() const { return Base::data().meshPath; }

    /**
     * @brief Returns a reference to the mesh base path.
     */
    std::string& meshBasePath() { return Base::data().meshPath; }

    /**
     * @brief Returns the i-th material of the mesh.
     * @param[in] i: The index of the texture.
     */
    const Material& material(uint i) const { return mats()[i]; }

    /**
     * @brief Returns a reference to the i-th material of the mesh.
     * @param[in] i: The index of the material.
     */
    Material& material(uint i) { return mats()[i]; }

    /**
     * @brief Clears the vector of materials.
     */
    void clearMaterials()
    {
        mats().clear();
    }

    /**
     * @brief Adds a material to the vector of materials.
     * @param[in] mat: The material to add.
     */
    void pushMaterial(const Material& mat)
    {
        mats().push_back(mat);
    }

    /**
     * @brief Returns an iterator to the beginning of the vector of materials.
     * @return an iterator to the beginning of the vector of materials.
     */
    MaterialIterator materialBegin() { return mats().begin(); }

    /**
     * @brief Returns an iterator to the end of the vector of materials.
     * @return an iterator to the end of the vector of materials.
     */
    MaterialIterator materialEnd() { return mats().end(); }

    /**
     * @brief Returns a const iterator to the beginning of the vector of materials.
     * @return a const iterator to the beginning of the vector of materials.
     */
    ConstMaterialIterator materialBegin() const { return mats().begin(); }

    /**
     * @brief Returns a const iterator to the end of the vector of materials.
     * @return a const iterator to the end of the vector of materials.
     */
    ConstMaterialIterator materialEnd() const { return mats().end(); }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the vector of materials. The view object exposes the iterators
     * trough the `begin()` and `end()` member functions, and therefore the
     * returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (auto& mat : m.materials()) {
     *     // Do something with mat
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of materials.
     */
    View<MaterialIterator> materials()
    {
        return View(materialBegin(), materialEnd());
    }

    /**
     * @brief Returns a lightweight const view object that stores the begin and end
     * iterators of the vector of materials. The view object exposes the iterators
     * trough the `begin()` and `end()` member functions, and therefore the
     * returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (const auto& mat : m.materials()) {
     *     // Do something with mat
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of materials.
     */
    View<ConstMaterialIterator> materials() const
    {
        return View(materialBegin(), materialEnd());
    }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        if constexpr (HasMaterials<Element>) {
            mats().clear();
            for (const auto& mat : e.materials()) {
                mats().push_back(mat.path());
            }
            meshBasePath() = e.meshBasePath();
        }
    }

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, mats());
        vcl::serialize(os, meshBasePath());
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, mats());
        vcl::deserialize(is, meshBasePath());
    }

private:
    // members that allow to access the data
    std::vector<Material>& mats() { return Base::data().materials; }

    const std::vector<Material>& mats() const { return Base::data().materials; }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_MATERIALS_H
