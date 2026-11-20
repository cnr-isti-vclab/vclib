// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_BASE_CUSTOM_COMPONENTS_VECTOR_MAP_H
#define VCL_MESH_CONTAINERS_BASE_CUSTOM_COMPONENTS_VECTOR_MAP_H

#include <vclib/mesh/exceptions.h>

#include <vclib/base.h>

#include <any>
#include <cstring>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace vcl::mesh::detail {

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
public:
    void clear() {}

    void reserve(std::size_t) {}

    void resize(std::size_t) {}

    template<typename Index>
    void compact(const std::vector<Index>&)
    {
    }

    void swapCustomComponents(std::size_t, std::size_t) {}

    void swap(CustomComponentsVectorMap&) noexcept {}

    friend void swap(
        CustomComponentsVectorMap&,
        CustomComponentsVectorMap&) noexcept
    {
    }

    template<typename Other>
    void append(const Other&, std::size_t, std::size_t)
    {
    }

    template<typename Other>
    void importSameCustomComponentFrom(
        std::size_t,
        std::size_t,
        const std::string&,
        const Other&)
    {
    }

    std::vector<std::string> allComponentNames() const { return {}; }

    template<typename CompType>
    void serializeCustomComponentsOfType(std::ostream&) const
    {
    }

    template<typename CompType>
    void deserializeCustomComponentsOfType(std::istream&)
    {
    }
};

template<>
class CustomComponentsVectorMap<true>
{
    using FactoryFn = std::any (*)();

    // the actual map containing, for each name of a custom component, the
    // vector of values (a value for each element(vertex/face...) of the mesh)
    std::unordered_map<std::string, std::vector<std::any>> mMap;
    // types used for each custom component
    std::unordered_map<std::string, std::type_index> mCompType;
    // default constructor factories for initializing new elements on resize
    std::unordered_map<std::string, FactoryFn> mFactory;

    template<typename CompType>
    static std::any createDefault()
    {
        return CompType();
    }

public:
    /**
     * @brief Removes all the custom component vectors stored in the mMap.
     */
    void clear()
    {
        mMap.clear();
        mCompType.clear();
        mFactory.clear();
    }

    /**
     * @brief Swaps the contents of this CustomComponentsVectorMap with another.
     * @param[in,out] other: The other CustomComponentsVectorMap.
     */
    void swap(CustomComponentsVectorMap& other) noexcept
    {
        using std::swap;
        swap(mMap, other.mMap);
        swap(mCompType, other.mCompType);
        swap(mFactory, other.mFactory);
    }

    friend void swap(
        CustomComponentsVectorMap& a,
        CustomComponentsVectorMap& b) noexcept
    {
        a.swap(b);
    }

    /**
     * @brief For each custom component vector, it reserves the given size.
     * @param[in] size: the size to reserve for each custom component vector.
     */
    void reserve(std::size_t size)
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
    void resize(std::size_t size)
    {
        for (auto& [name, vec] : mMap) {
            const std::size_t oldSize = vec.size();
            if (size > oldSize) {
                vec.resize(size);
                auto factory = mFactory.at(name);
                for (std::size_t i = oldSize; i < size; ++i) {
                    vec[i] = factory();
                }
            }
            else {
                vec.resize(size);
            }
        }
    }

    /**
     * @brief Compacts each custom component vector according to the given
     * indices.
     *
     * All the indices of the newIndices vector with value `NULL_INDEX<Index>`
     * are considered as deleted, and the corresponding elements in the custom
     * component vectors are removed. The remaining indices are compacted, and
     * the custom component vectors are resized to the new size.
     *
     * @param[in] newIndices: a vector that tells, for each element index, the
     * new index of the element, and `NULL_INDEX<Index>` if the element has been
     * deleted.
     */
    template<typename Index>
    void compact(const std::vector<Index>& newIndices)
    {
        for (auto& p : mMap) {
            compactVector(p.second, newIndices);
        }
    }

    void swapCustomComponents(std::size_t i, std::size_t j)
    {
        for (auto& p : mMap) {
            std::swap(p.second[i], p.second[j]);
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
    void addNewComponent(const std::string& name, std::size_t size)
    {
        std::vector<std::any>& v = mMap[name];
        v.assign(size, CompType());
        mCompType.insert_or_assign(name, typeid(CompType));
        mFactory.insert_or_assign(name, &createDefault<CompType>);
    }

    /**
     * @brief Deletes the custom component vector with the given name.
     * It does nothing if the element does not exist.
     * @param[in] name: the name of the custom component vector to delete.
     */
    void deleteComponent(const std::string& name)
    {
        mMap.erase(name);
        mCompType.erase(name);
        mFactory.erase(name);
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
        return mMap.at(compName);
    }

    /**
     * @brief Returns a reference of std::vector<std::any> of the custom
     * component with the given name and the given template argument CompType.
     *
     * If the CompType does not mach with the type associated with compName,
     * thows a vcl::BadCustomComponentTypeException.
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
        return mMap.at(compName);
    }

    void importSameCustomComponentFrom(
        std::size_t                            thisPos,
        std::size_t                            otherPos,
        const std::string&                     compName,
        const CustomComponentsVectorMap<true>& other)
    {
        if (other.componentExists(compName) && componentExists(compName)) {
            if (other.componentType(compName) == componentType(compName)) {
                mMap.at(compName)[thisPos] = other.mMap.at(compName)[otherPos];
            }
        }
    }

    void append(
        const CustomComponentsVectorMap<true>& other,
        std::size_t                            thisOffset,
        std::size_t                            otherCount)
    {
        for (const auto& [name, type] : other.mCompType) {
            if (componentExists(name) && componentType(name) == type) {
                auto&       thisVec  = mMap.at(name);
                const auto& otherVec = other.mMap.at(name);
                for (std::size_t i = 0; i < otherCount; ++i) {
                    thisVec[thisOffset + i] = otherVec[i];
                }
            }
        }
    }

    template<typename CompType>
    void serializeCustomComponentsOfType(std::ostream& os) const
    {
        constexpr uint32_t CCMP_TAG = 0x504D4343; // 'C', 'C', 'M', 'P'
        vcl::serialize(os, CCMP_TAG);
        uint32_t typeSize = static_cast<uint32_t>(sizeof(CompType));
        vcl::serialize(os, typeSize);

        std::vector<std::string> compNames =
            allComponentNamesOfType<CompType>();
        vcl::serialize(os, compNames);
        for (const auto& name : compNames) {
            const std::vector<std::any>& v = componentVector<CompType>(name);
            vcl::serialize<CompType>(os, v);
        }
    }

    template<typename CompType>
    void deserializeCustomComponentsOfType(std::istream& is)
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
            std::vector<std::any> v;
            vcl::deserialize<CompType>(is, v);
            mMap[name] = std::move(v);
            mCompType.insert_or_assign(name, typeid(CompType));
            mFactory.insert_or_assign(name, &createDefault<CompType>);
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

} // namespace vcl::mesh::detail

#endif // VCL_MESH_CONTAINERS_BASE_CUSTOM_COMPONENTS_VECTOR_MAP_H
