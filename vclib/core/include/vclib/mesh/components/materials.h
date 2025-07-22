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

#include "bases/component.h"

#include <vclib/concepts/mesh/components/materials.h>
#include <vclib/space/core/material.h>

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

protected:
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
