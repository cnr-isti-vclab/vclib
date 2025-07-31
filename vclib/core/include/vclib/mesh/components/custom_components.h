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

#ifndef VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
#define VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H

#include "base/base.h"
#include "detail/custom_components_data.h"
#include "concepts/custom_components.h"

#include <vclib/serialization.h>

#include <any>
#include <string>
#include <vector>

namespace vcl::comp {

/**
 * @brief The CustomComponents Component is a container of additional components
 * associated to an Element (e.g. Vertex, Face).
 *
 * CustomComponents are components that can be added at runtime. Each custom
 * component is composed of:
 * - a name, which is an std::string
 * - a type, which needs to be known at compile time
 *
 * E.g., to access to a CustomComponent of type 'int' called "myCustomComponent"
 * from an element el:
 *
 * @code{.cpp}
 * el.customComponent<int>("myCustomComponent");
 * @endcode
 *
 * CustomComponents can be stored horizontally or vertically.
 *
 * If the CustomComponents component is horizontal (this happens mostly on Mesh
 * data structures), a custom component can be added or removed from the object
 * itself.
 *
 * For example, having a Mesh m of type 'MyMesh' that has horizontal
 * CustomComponents:
 *
 * @code{.cpp}
 * m.addCustomComponent<int>("myCustomComponent");
 * @endcode
 *
 * Otherwise, if the CustomComponents component is vertical (this happens mostly
 * on Element types stored in a container, like Vertex, Face...), the
 * addition/deletion of a custom component cannot be performed by the object,
 * because its storage its managed by the Container of objects (that must
 * provide a proper member function to do that).
 *
 * For example, having a Mesh m of type MyMesh that has a VertexContainer where
 * its Vertex has (vertical) CustomComponents:
 *
 * @code{.cpp}
 * m.addPerVertexCustomComponent<int>("myCustomComponent");
 * @endcode
 *
 * After this call, all the Vertices of the VertexContainer will have their
 * custom component of type int called "myCustomComponent". The member function
 * addPerVertexCustomComponent is provided by the VertexContainer and can be
 * accessed directly from the Mesh.
 *
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 *
 * @ingroup components
 */
template<typename ParentElemType = void>
class CustomComponents
{
    static const bool IS_VERTICAL = !std::is_same_v<ParentElemType, void>;

    detail::CustomComponentsData<ParentElemType, IS_VERTICAL> mData;

public:
    /**
     * @brief The ID of component.
     */
    static const uint COMPONENT_ID = CompId::CUSTOM_COMPONENTS;

    /* Constructors */

    /**
     * @brief Initilizes an empty container of custom components.
     */
    CustomComponents() = default;

    /* Member functions */

    /**
     * @brief Returns true if the element has a custom component with the given
     * name, false otherwise. The type of the custom component is not checked.
     *
     * @param[in] compName: the name of the custom component.
     * @return true if the element has a custom component with the given name,
     * false otherwise.
     */
    bool hasCustomComponent(const std::string& compName) const
    {
        return mData.componentExists(
            compName, static_cast<const ParentElemType*>(this));
    }

    /**
     * @brief Returns true if the custom component of the given name is of the
     * type given as template argument, false otherwise.
     *
     * @note The element must have a custom component with the given name,
     * otherwise a std::out_of_range exception is thrown. If you don't know if
     * the element has a custom component with the given name, use the
     * CustomComponents::hasCustomComponent() member function.
     *
     * @throws std::out_of_range if the element does not have a custom component
     * with the given name.
     *
     * @tparam CompType: the type of the custom component to check.
     * @param[in] compName: the name of the custom component.
     * @return true if the custom component of the given name is of the type
     * CompType, false otherwise.
     */
    template<typename CompType>
    bool isCustomComponentOfType(const std::string& compName) const
    {
        return mData.template isComponentOfType<CompType>(
            compName, static_cast<const ParentElemType*>(this));
    }

    /**
     * @brief Returns the `std::type_index` of the custom component of the given
     * name.
     *
     * @note The element must have a custom component with the given name,
     * otherwise a std::out_of_range exception is thrown. If you don't know if
     * the element has a custom component with the given name, use the
     * CustomComponents::hasCustomComponent() member function.
     *
     * @throws std::out_of_range if the element does not have a custom component
     * with the given name.
     *
     * @param[in] compName: the name of the custom component.
     * @return the `std::type_index` of the custom component of the given name.
     */
    std::type_index customComponentType(const std::string& compName) const
    {
        return mData.componentType(
            compName, static_cast<const ParentElemType*>(this));
    }

    /**
     * @brief Returns a std::vector of std::strings containing the names of the
     * custom components of the type given as template argument.
     *
     * @tparam CompType: the type of the custom components to retrieve.
     * @return a std::vector of std::strings containing the names of the custom
     * components of the type given as template argument.
     */
    template<typename CompType>
    std::vector<std::string> customComponentNamesOfType() const
    {
        return mData.template componentNamesOfType<CompType>(
            static_cast<const ParentElemType*>(this));
    }

    /**
     * @brief Returns the const reference to the custom component of the given
     * name having the type given as template argument.
     *
     * @note The element must have a custom component with the given name,
     * otherwise a std::out_of_range exception is thrown. If you don't know if
     * the element has a custom component with the given name, use the
     * CustomComponents::hasCustomComponent() member function.
     *
     * @note The custom component must be of the type given as template
     * argument, otherwise a std::bad_any_cast exception is thrown. The type of
     * the custom component must be known at compile time.
     *
     * @throws std::out_of_range if the element does not have a custom component
     * with the given name.
     * @throws std::bad_any_cast if the custom component is not of the type
     * given as template argument.
     *
     * @tparam CompType: the type of the custom component to retrieve.
     * @param[in] compName: the name of the custom component.
     * @return the const reference to the custom component of the given name
     * having the type given as template argument.
     */
    template<typename CompType>
    const CompType& customComponent(const std::string& compName) const
    {
        return mData.template get<CompType>(
            compName, static_cast<const ParentElemType*>(this));
    }

    /**
     * @brief Returns the reference to the custom component of the given
     * name having the type given as template argument.
     *
     * @note The element must have a custom component with the given name,
     * otherwise a std::out_of_range exception is thrown. If you don't know if
     * the element has a custom component with the given name, use the
     * CustomComponents::hasCustomComponent() member function.
     *
     * @note The custom component must be of the type given as template
     * argument, otherwise a std::bad_any_cast exception is thrown. The type of
     * the custom component must be known at compile time.
     *
     * @throws std::out_of_range if the element does not have a custom component
     * with the given name.
     * @throws std::bad_any_cast if the custom component is not of the type
     * given as template argument.
     *
     * @tparam CompType: the type of the custom component to retrieve.
     * @param[in] compName: the name of the custom component.
     * @return the reference to the custom component of the given name having
     * the type given as template argument.
     */
    template<typename CompType>
    CompType& customComponent(const std::string& compName)
    {
        return mData.template get<CompType>(
            compName, static_cast<ParentElemType*>(this));
    }

    template<typename CompType>
    void addCustomComponent(
        const std::string& compName,
        const CompType&    value = CompType()) requires (!IS_VERTICAL)
    {
        return mData.template addCustomComponent<CompType>(compName, value);
    }

    void deleteCustomComponent(const std::string& compName)
        requires (!IS_VERTICAL)
    {
        return mData.deleteCustomComponent(compName);
    }

    template<typename CompType>
    void serializeCustomComponentsOfType(std::ostream& os) const
        requires (!IS_VERTICAL)
    {
        std::vector<std::string> compNames =
            customComponentNamesOfType<CompType>();
        vcl::serialize(os, compNames);
        for (const auto& name : compNames) {
            const CompType& c = customComponent<CompType>(name);
            vcl::serialize(os, c);
        }
    }

    template<typename CompType>
    void deserializeCustomComponentsOfType(std::istream& is)
        requires (!IS_VERTICAL)
    {
        std::vector<std::string> compNames;
        vcl::deserialize(is, compNames);
        for (const auto& name : compNames) {
            CompType c;
            vcl::deserialize(is, c);
            addCustomComponent<CompType>(name, c);
        }
    }

protected:
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        // if the component is vertical, the import is managed by the container.
        // if is horizontal, it must be managed by the component itself.
        if constexpr (!IS_VERTICAL && HasCustomComponents<Element>) {
            mData = e.CustomComponents::mData;
        }
    }

    void serialize(std::ostream& os) const
    {
        // todo
    }

    void deserialize(std::istream& is)
    {
        // todo
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
