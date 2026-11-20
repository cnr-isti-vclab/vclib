// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
#define VCL_MESH_MESH_COMPONENTS_CUSTOM_COMPONENTS_H

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/components/custom_components.h>
#include <vclib/mesh/exceptions.h>

#include <vclib/base.h>

#include <any>
#include <istream>
#include <ostream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace vcl::mesh {

/**
 * @brief The CustomComponents class is a Mesh component that manages
 * runtime-defined custom components at the mesh level (horizontal dynamic
 * attributes stored on the mesh).
 *
 * Each mesh-level custom component is identified by a unique name (std::string)
 * and stores an instance of any type (std::any), with its std::type_index
 * tracked.
 *
 * E.g., to add and access a mesh-level custom component from a mesh `m`:
 * @code{.cpp}
 * m.addCustomComponent<int>("iteration", 42);
 * int iter = m.customComponent<int>("iteration");
 * @endcode
 *
 * @ingroup mesh_components
 */
class CustomComponents : public comp::Component<CompId::CUSTOM_COMPONENTS, void>
{
    std::unordered_map<std::string, std::any>        mMap;
    std::unordered_map<std::string, std::type_index> mCompType;

public:
    /**
     * @brief Default constructor. Initializes an empty container of custom
     * components.
     */
    CustomComponents() = default;

    /**
     * @brief Checks if the mesh has a custom component with the given name.
     *
     * @param[in] compName: The name of the custom component to check.
     * @return true if the custom component exists, false otherwise.
     */
    bool hasCustomComponent(this const auto& self, const std::string& compName)
    {
        return self.mMap.find(compName) != self.mMap.end();
    }

    /**
     * @brief Checks if the custom component of the given name is of type
     * CompType.
     *
     * @tparam CompType: The type of the custom component to check.
     * @param[in] compName: The name of the custom component.
     * @throws std::out_of_range if the custom component does not exist.
     * @return true if the custom component is of type CompType, false
     * otherwise.
     */
    template<typename CompType>
    bool isCustomComponentOfType(
        this const auto&   self,
        const std::string& compName)
    {
        std::type_index t(typeid(CompType));
        return t == self.mCompType.at(compName);
    }

    /**
     * @brief Returns the std::type_index of the custom component of the given
     * name.
     *
     * @param[in] compName: The name of the custom component.
     * @throws std::out_of_range if the custom component does not exist.
     * @return The std::type_index of the custom component.
     */
    std::type_index customComponentType(
        this const auto&   self,
        const std::string& compName)
    {
        return self.mCompType.at(compName);
    }

    /**
     * @brief Returns a vector of strings containing the names of all custom
     * components stored on the mesh.
     *
     * @return A vector of strings containing the names of all custom
     * components.
     */
    std::vector<std::string> customComponentNames(this const auto& self)
    {
        std::vector<std::string> names;
        names.reserve(self.mMap.size());
        for (const auto& [name, _] : self.mMap) {
            names.push_back(name);
        }
        return names;
    }

    /**
     * @brief Returns a vector of strings containing the names of all custom
     * components of type CompType stored on the mesh.
     *
     * @tparam CompType: The type of the custom components to retrieve.
     * @return A vector of strings containing the names of custom components of
     * type CompType.
     */
    template<typename CompType>
    std::vector<std::string> customComponentNamesOfType(this const auto& self)
    {
        std::vector<std::string> names;
        std::type_index          t(typeid(CompType));
        for (const auto& [name, type] : self.mCompType) {
            if (type == t) {
                names.push_back(name);
            }
        }
        return names;
    }

    /**
     * @brief Adds or replaces a custom component of type CompType with the
     * given name.
     *
     * @tparam CompType: The type of the custom component to add.
     * @param[in] compName: The name of the custom component.
     * @param[in] value: The initial value (default: CompType()).
     */
    template<typename CompType>
    void addCustomComponent(
        this auto&         self,
        const std::string& compName,
        const CompType&    value = CompType())
    {
        self.mMap[compName] = value;
        self.mCompType.insert_or_assign(compName, typeid(CompType));
    }

    /**
     * @brief Deletes the custom component with the given name.
     * If the component does not exist, this function does nothing.
     *
     * @param[in] compName: The name of the custom component to delete.
     */
    void deleteCustomComponent(this auto& self, const std::string& compName)
    {
        self.mMap.erase(compName);
        self.mCompType.erase(compName);
    }

    /**
     * @brief Clears all custom components stored on the mesh.
     */
    void clearCustomComponents(this auto& self)
    {
        self.mMap.clear();
        self.mCompType.clear();
    }

    /**
     * @brief Returns a reference to the custom component of type CompType with
     * the given name.
     *
     * @tparam CompType: The type of the custom component to retrieve.
     * @param[in] compName: The name of the custom component.
     * @throws std::out_of_range if the custom component does not exist.
     * @throws vcl::BadCustomComponentTypeException if the custom component is
     * not of type CompType.
     * @return A reference to the custom component value.
     */
    template<typename CompType>
    decltype(auto) customComponent(
        this auto&&        self,
        const std::string& compName)
    {
        self.template checkComponentType<CompType>(compName);
        using ReturnType = std::conditional_t<
            std::is_const_v<std::remove_reference_t<decltype(self)>>,
            const CompType&,
            CompType&>;
        return std::any_cast<ReturnType>(self.mMap.at(compName));
    }

    /**
     * @brief Serializes all custom components of type CompType to the output
     * stream.
     *
     * @tparam CompType: The type of the custom components to serialize.
     * @param[out] os: The output stream.
     */
    template<typename CompType>
    void serializeCustomComponentsOfType(
        this const auto& self,
        std::ostream&    os)
    {
        constexpr uint32_t CCMP_TAG = 0x504D4343; // 'C', 'C', 'M', 'P'
        vcl::serialize(os, CCMP_TAG);
        uint32_t typeSize = static_cast<uint32_t>(sizeof(CompType));
        vcl::serialize(os, typeSize);

        std::vector<std::string> compNames =
            self.template customComponentNamesOfType<CompType>();
        vcl::serialize(os, compNames);
        for (const auto& name : compNames) {
            const CompType& c = self.template customComponent<CompType>(name);
            vcl::serialize(os, c);
        }
    }

    /**
     * @brief Deserializes custom components of type CompType from the input
     * stream.
     *
     * @tparam CompType: The type of the custom components to deserialize.
     * @param[in] is: The input stream.
     */
    template<typename CompType>
    void deserializeCustomComponentsOfType(this auto& self, std::istream& is)
    {
        constexpr uint32_t CCMP_TAG = 0x504D4343;
        uint32_t           tag      = 0;
        vcl::deserialize(is, tag);
        if (tag != CCMP_TAG) {
            throw std::runtime_error(
                "Invalid custom component block magic header");
        }
        uint32_t typeSize = 0;
        vcl::deserialize(is, typeSize);
        if (typeSize != sizeof(CompType)) {
            throw BadCustomComponentTypeException(
                "Custom component type size mismatch during deserialization.");
        }

        std::vector<std::string> compNames;
        vcl::deserialize(is, compNames);
        for (const auto& name : compNames) {
            CompType c;
            vcl::deserialize(is, c);
            self.template addCustomComponent<CompType>(name, c);
        }
    }

protected:
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        if constexpr (comp::HasCustomComponents<OtherMesh>) {
            if constexpr (std::is_base_of_v<CustomComponents, OtherMesh>) {
                const auto& other = static_cast<const CustomComponents&>(m);
                mMap              = other.mMap;
                mCompType         = other.mCompType;
            }
        }
    }

private:
    template<typename CompType>
    void checkComponentType(this const auto& self, const std::string& compName)
    {
        std::type_index t(typeid(CompType));
        if (t != self.mCompType.at(compName)) {
            throw BadCustomComponentTypeException(
                "Expected type " +
                std::string(self.mCompType.at(compName).name()) + " for " +
                compName + ", but was " + std::string(t.name()) + ".");
        }
    }
};

/**
 * @brief Concept that checks whether a type T has the mesh-level
 * CustomComponents component (COMP_ID = CompId::CUSTOM_COMPONENTS).
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;

} // namespace vcl::mesh

#endif // VCL_MESH_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
