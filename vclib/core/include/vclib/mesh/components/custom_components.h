// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.
//

#ifndef VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
#define VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H

#include "base/component.h"
#include "base/component_predicates.h"

#include <string>
#include <typeindex>
#include <vector>

namespace vcl::comp {

/**
 * @brief The CustomComponents class is an Element component that manages the
 * access to runtime-defined custom components (e.g. per-vertex or per-face
 * dynamic attributes).
 *
 * CustomComponents are components that can be added at runtime. Each custom
 * component is composed of:
 * - a name, which is an std::string
 * - a type, which needs to be known at compile time when accessed
 *
 * E.g., to access a CustomComponent of type 'int' called "myCustomComponent"
 * from an element proxy `el`:
 *
 * @code{.cpp}
 * el.customComponent<int>("myCustomComponent");
 * @endcode
 *
 * Since the element proxy is a flyweight view over the container's Structure of
 * Arrays (SoA), this component delegates the actual storage and lookup to the
 * underlying ElementContainer.
 *
 * @ingroup components
 */
class CustomComponents : public Component<CompId::CUSTOM_COMPONENTS, void>
{
public:
    /* Constructors */

    /**
     * @brief Initializes the CustomComponents component.
     */
    CustomComponents() = default;

    /* Member functions */

    /**
     * @brief Returns true if the element has a custom component with the given
     * name, false otherwise. The type of the custom component is not checked.
     *
     * @param[in] compName: The name of the custom component.
     * @return true if the element has a custom component with the given name,
     * false otherwise.
     */
    bool hasCustomComponent(this const auto& self, const std::string& compName)
    {
        return self.parentContainer().hasElemCustomComponent(compName);
    }

    /**
     * @brief Returns true if the custom component of the given name is of the
     * type given as template argument, false otherwise.
     *
     * @tparam CompType: The type of the custom component to check.
     * @param[in] compName: The name of the custom component.
     * @return true if the custom component of the given name is of the type
     * CompType, false otherwise.
     */
    template<typename CompType>
    bool isCustomComponentOfType(
        this const auto&   self,
        const std::string& compName)
    {
        return self.parentContainer().template isElemCustomComponentOfType<CompType>(
            compName);
    }

    /**
     * @brief Returns the `std::type_index` of the custom component of the given
     * name.
     *
     * @param[in] compName: The name of the custom component.
     * @return The `std::type_index` of the custom component of the given name.
     */
    std::type_index customComponentType(
        this const auto&   self,
        const std::string& compName)
    {
        return self.parentContainer().elemCustomComponentType(compName);
    }

    /**
     * @brief Returns a vector of strings containing the names of all the custom
     * components associated to the element.
     *
     * @return A vector of strings containing the names of all custom components.
     */
    std::vector<std::string> customComponentNames(this const auto& self)
    {
        return self.parentContainer().elemCustomComponentNames();
    }

    /**
     * @brief Returns a vector of strings containing the names of the custom
     * components of the type given as template argument.
     *
     * @tparam CompType: The type of the custom components to retrieve.
     * @return A vector of strings containing the names of the custom
     * components of the type given as template argument.
     */
    template<typename CompType>
    std::vector<std::string> customComponentNamesOfType(this const auto& self)
    {
        return self.parentContainer()
            .template elemCustomComponentNamesOfType<CompType>();
    }

    /**
     * @brief Returns a reference to the custom component of the given name
     * having the type given as template argument.
     * Propagates constness automatically from the caller (proxy).
     *
     * @throws std::out_of_range if the custom component does not exist.
     * @throws vcl::BadCustomComponentTypeException if the custom component
     * is not of type CompType.
     *
     * @tparam CompType: The type of the custom component to retrieve.
     * @param[in] compName: The name of the custom component.
     * @return A reference to the custom component of the given name.
     */
    template<typename CompType>
    decltype(auto) customComponent(
        this auto&&        self,
        const std::string& compName)
    {
        return self.parentContainer().template elemCustomComponent<CompType>(
            compName, self.index());
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * CustomComponents component (COMP_ID = CompId::CUSTOM_COMPONENTS).
 *
 * @tparam T: The type to be tested for conformity to HasCustomComponents.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasCustomComponents = HasComponent<CompId::CUSTOM_COMPONENTS, T>;

/* Implementation of member functions */

template<typename OtherElement>
void CustomComponents::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool)
{
    if constexpr (HasCustomComponents<OtherElement>) {
        auto&       thisCC  = self.parentContainer().mCustomComponents;
        const auto& otherCC = other.parentContainer().mCustomComponents;
        for (const auto& name : thisCC.allComponentNames()) {
            thisCC.importSameCustomComponentFrom(
                self.index(), other.index(), name, otherCC);
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
