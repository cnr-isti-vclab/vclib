// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASE_COMPONENT_H

#include <vclib/base.h>

#include <vclib/mesh/components/base/base.h>

#include <cassert>

namespace vcl::comp {

/**
 * @brief Concept that checks whether a type Data can be used as the data type
 * of a Component.
 *
 * A valid component data type must be either `void` (for components that do not
 * store per-element data in the container tuple, like CustomComponents) or a
 * semiregular, serializable type.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept ComponentData =
    std::is_void_v<T> || (std::semiregular<T> && Serializable<T>);

/**
 * @brief Base class for all components.
 * It exposes the DataType (used by the container to build the tuple)
 * and the COMPONENT_ID (for metaprogramming checks).
 */
template<CompId COMP_ID, ComponentData Data, bool OPT = false>
class Component
{
public:
    using DataType                       = Data;
    static constexpr CompId COMPONENT_ID = COMP_ID;
    static constexpr bool   IS_OPTIONAL  = OPT;

protected:
    // needs to be static to be accessible from the derived classes
    static decltype(auto) compData(auto&& self)
    {
        assert(self.isValid() && "Element proxy is invalid!");
        if constexpr (OPT) {
            assert(
                self.parentContainer()
                    .template isComponentAvailable<COMP_ID>() &&
                "Component is optional and not currently enabled in the "
                "container!");
        }
        return self.parentContainer().template vector<COMP_ID>()[self.index()];
    }
};

/**
 * @brief Concept that checks whether a type T is a Component (has
 * COMPONENT_ID).
 */
template<typename T>
concept ComponentConcept = requires {
    { std::remove_cvref_t<T>::COMPONENT_ID } -> std::convertible_to<CompId>;
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_COMPONENT_H
