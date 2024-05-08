/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_MESH_CONTAINERS_CUSTOM_COMPONENTS_VECTOR_MAP_H
#define VCL_MESH_CONTAINERS_CUSTOM_COMPONENTS_VECTOR_MAP_H

#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <vclib/exceptions/mesh_exceptions.h>
#include <vclib/misc/compactness.h>
#include <vclib/types.h>

namespace vcl::mesh {

/**
 * @brief The CustomComponentsVectorMap class stores a map of vectors of custom
 * components.
 *
 * Each vector of custom components has two main properties:
 * - component name: an unique std::string that identifies the vector of
 *   components;
 * - component type: the type of the component, that is a template parameter
 *   and must be know at compile time.
 *
 * The class allows to access to the vectors of custom components trough their
 * name and type.
 *
 * For each custom component, the class stores a vector of std::any, that
 * allows to store any type of data. The actual type of the data stored in the
 * vectors is required to access to the vector data.
 *
 * @note This class is templated over a boolean value that enables the
 * functionalities of the class. If a CustomComponentsVectorMap<false> is
 * instantiated, no memory will be used by the instance of the class, and no
 * member functions will be available. The class can be used only when a
 * CustomComponentsVectorMap<true> is instantiated.
 *
 * @tparam HasCustomComponent: A boolean value that enables the functionalities
 * of the class.
 */
template<bool HasCustomComponent>
class CustomComponentsVectorMap
{
};

template<>
class CustomComponentsVectorMap<true>
{
    // the actual map containing, for each name of a custom component, the
    // vector of values (a value for each element(vertex/face...) of the mesh)
    std::unordered_map<std::string, std::vector<std::any>> mMap;
    // map that stores if some custom components need to be initialized.
    // this happens when we resize the container of an element: we need to
    // resize also the vectors of the custom components, but we cannot
    // initialize them with the right type (because we don't know it in resize
    // phase). The initialization will be made when the type is known (get of
    // the custom component vector).
    mutable std::unordered_map<std::string, bool> mNeedToInitialize;
    // types used for each custom component
    std::unordered_map<std::string, std::type_index> mCompType;

public:
    /**
     * @brief Removes all the custom component vectors stored in the mMap.
     */
    void clear()
    {
        mMap.clear();
        mNeedToInitialize.clear();
        mCompType.clear();
    }

    /**
     * @brief For each custom component vector, it reserves the given size.
     * @param[in] size: the size to reserve for each custom component vector.
     */
    void reserve(uint size)
    {
        for (auto& p : mMap) {
            p.second.reserve(size);
        }
    }

    /**
     * @brief For each custom component vector, it resizes the vector to the
     * given size.
     * @param[in] size: the size to reserve for each custom component vector.
     */
    void resize(uint size)
    {
        for (auto& p : mMap) {
            // At this call, we don't know statically the types of each custom
            // component, therefore we cannot initialize them (std::any of each
            // vector will be in an invalid state). Therefore, we mark all the
            // resized custom components as 'needToInitialize'. Initiaization
            // will be made just on the uninitialized values of the vectors at
            // the first access of each custom component.
            if (p.second.size() < size)
                mNeedToInitialize.at(p.first) = true;
            p.second.resize(size);
        }
    }

    /**
     * @brief Compacts each custom component vector according to the given
     * indices.
     *
     * All the indices  of the newIndices vector with value UINT_NULL are
     * considered as deleted, and the corresponding elements in the custom
     * component vectors are removed. The remaining indices are compacted, and
     * the custom component vectors are resized to the new size.
     *
     * @param[in] newIndices: a vector that tells, for each element index, the
     * new index of the element, and UINT_NULL if the element has been deleted.
     */
    void compact(const std::vector<uint>& newIndices)
    {
        for (auto& p : mMap) {
            vcl::compactVector(p.second, newIndices);
        }
    }

    /**
     * @brief Adds a new vector of custom components having the given size, the
     * given name and with the template argumet CompType.
     *
     * Data is initialized with the empty constructor of CompType.
     *
     * @note If a custom component with the same name already exists, it is
     * replaced, no matter of the old type.
     *
     * @tparam CompType: the type of the newly added custom component.
     * @param[in] name: the unique name of the custom component.
     * @param[in] size: the initial size of the custom component vector.
     */
    template<typename CompType>
    void addNewComponent(const std::string& name, uint size)
    {
        std::vector<std::any>& v = mMap[name];
        v.resize(size, CompType());
        mNeedToInitialize[name] = false;
        mCompType.emplace(name, typeid(CompType));
    }

    /**
     * @brief Deletes the custom component vector with the given name.
     * It does nothing if the element does not exist.
     * @param[in] name: the name of the custom component vector to delete.
     */
    void deleteComponent(const std::string& name)
    {
        mMap.erase(name);
        mNeedToInitialize.erase(name);
        mCompType.erase(name);
    }

    /**
     * @brief Asserts that the compName component exists.
     *
     * @param[in] compName: the name of the custom component to check.
     */
    void assertComponentExists(const std::string& compName) const
    {
        (void) (compName);
        assert(mMap.find(compName) != mMap.end());
    }

    /**
     * @brief Returns true if the compName exists.
     * @param[in] compName: the name of the custom component to check.
     * @return true if the compName exists.
     */
    bool componentExists(const std::string& compName) const
    {
        return (mMap.find(compName) != mMap.end());
    }

    /**
     * @brief Returns a vector of std::string containing all the custom
     * components, regardless the types of the custom components.
     * @return a vector of std::string containing all the custom components.
     */
    std::vector<std::string> allComponentNames() const
    {
        std::vector<std::string> names;
        names.reserve(mMap.size());
        for (const auto& p : mMap)
            names.push_back(p.first);
        return names;
    }

    /**
     * @brief Returns true if the type associated to the compName custom
     * component is the same of the given template argument CompType.
     *
     * @tparam CompType: the type to check.
     * @param[in] compName: the name of the custom component to check.
     * @return true if the type associated to the compName custom component
     * is the same of the given template argument CompType.
     */
    template<typename CompType>
    bool isComponentOfType(const std::string& compName) const
    {
        std::type_index t(typeid(CompType));

        return t == mCompType.at(compName);
    }

    /**
     * @brief Returns the std::type_index of the type of the CustomComponent
     * having the input name.
     *
     * @throws std::out_of_range if the compName does not exist.
     * @param[in] compName: the name of the custom component.
     * @return the std::type_index of the type of the CustomComponent having
     * the input name.
     */
    std::type_index componentType(const std::string& compName) const
    {
        return mCompType.at(compName);
    }

    /**
     * @brief Returns a vector of strings of all the custom components having as
     * type the given template argument CompType.
     *
     * @tparam CompType: the type of the custom components to return.
     * @return a vector of strings of all the custom components having as
     * type the given template argument CompType.
     */
    template<typename CompType>
    std::vector<std::string> allComponentNamesOfType() const
    {
        std::vector<std::string> names;
        std::type_index          t(typeid(CompType));
        for (const auto& p : mCompType) {
            if (p.second == t)
                names.push_back(p.first);
        }
        return names;
    }

    /**
     * @brief Returns a const reference of std::vector<std::any> of the custom
     * component with the given name and the given template argument CompType.
     *
     * If the CompType does not mach with the type associated with compName,
     * thows a vcl::BadCustomComponentTypeException.
     *
     * @tparam CompType: the type of the custom component to return.
     * @param[in] compName: the name of the custom component to return.
     * @return a const reference of std::vector<std::any> of the custom
     * component with the given name and the given template argument CompType.
     */
    template<typename CompType>
    const std::vector<std::any>& componentVector(
        const std::string& compName) const
    {
        checkComponentType<CompType>(compName);
        std::vector<std::any>& v =
            const_cast<std::vector<std::any>&>(mMap.at(compName));
        // if the component needs to be initialized (e.g. we made a resize)
        if (mNeedToInitialize.at(compName)) {
            for (std::any& a : v) {
                if (!a.has_value())
                    a = CompType();
            }
            mNeedToInitialize.at(compName) = false;
        }
        return v;
    }

    /**
     * @brief Returns a reference of std::vector<std::any> of the custom
     * component with the given name and the given template argument CompType.
     *
     * If the CompType does not mach with the type associated with compName,
     * thows a vcl::BadCustomComponentTypeException.
     *
     * This function will initialize all the uninitialized values of the
     * accessed custom component.
     *
     * @tparam CompType: the type of the custom component to return.
     * @param[in] compName: the name of the custom component to return.
     * @return a reference of std::vector<std::any> of the custom component
     * with the given name and the given template argument CompType.
     */
    template<typename CompType>
    std::vector<std::any>& componentVector(const std::string& compName)
    {
        checkComponentType<CompType>(compName);
        std::vector<std::any>& v = mMap.at(compName);
        // if the component needs to be initialized (e.g. we made a resize)
        if (mNeedToInitialize.at(compName)) {
            for (std::any& a : v) {
                if (!a.has_value())
                    a = CompType();
            }
            mNeedToInitialize.at(compName) = false;
        }
        return v;
    }

    void importSameCustomComponentFrom(
        uint thisPos,
        uint otherPos,
        const std::string& compName,
        const CustomComponentsVectorMap<true>& other)
    {
        if (other.componentExists(compName) && componentExists(compName)) {
            if (other.componentType(compName) == componentType(compName)) {
                mMap.at(compName)[thisPos] =
                    other.mMap.at(compName)[otherPos];
            }
        }
    }

private:
    template<typename CompType>
    void checkComponentType(const std::string& compName) const
    {
        std::type_index t(typeid(CompType));
        if (t != mCompType.at(compName)) {
            throw BadCustomComponentTypeException(
                "Expected type " + std::string(mCompType.at(compName).name()) +
                " for " + compName + ", but was " + std::string(t.name()) +
                ".");
        }
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_CUSTOM_COMPONENTS_VECTOR_MAP_H
