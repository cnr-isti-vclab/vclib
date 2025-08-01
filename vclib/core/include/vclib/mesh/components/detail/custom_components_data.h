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

#ifndef VCL_MESH_COMPONENTS_DETAIL_CUSTOM_COMPONENTS_DATA_H
#define VCL_MESH_COMPONENTS_DETAIL_CUSTOM_COMPONENTS_DATA_H

#include <vclib/base.h>

#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace vcl::comp::detail {

/*
 * The CustomComponentData is the data structure that manages the access to the
 * custom components from an Element. If the custom components are horizontal,
 * they need to be stored in the Element memory frame, if they are vertical they
 * need to be stored by the Container of elements.
 *
 * The CustomComponentData stores the data horizontally
 * The CustomComponentData<El, true> (specialized on the boolean template)
 * stores the data vertically.
 */

// horizontal, not specialized. Store the custom component in this struct
template<typename ElementType, bool VERTICAL>
class CustomComponentsData
{
    std::unordered_map<std::string, std::any>        mMap;
    std::unordered_map<std::string, std::type_index> mCompType;

public:
    bool componentExists(const std::string& compName, const ElementType*) const
    {
        return (mMap.find(compName) != mMap.end());
    }

    template<typename CompType>
    bool isComponentOfType(const std::string& compName, const ElementType*)
        const
    {
        std::type_index t(typeid(CompType));
        return t == mCompType.at(compName);
    }

    std::type_index componentType(
        const std::string& compName,
        const ElementType*) const
    {
        return mCompType.at(compName);
    }

    template<typename CompType>
    std::vector<std::string> componentNamesOfType(const ElementType*) const
    {
        std::vector<std::string> names;
        std::type_index          t(typeid(CompType));
        for (const auto& p : mCompType) {
            if (p.second == t)
                names.push_back(p.first);
        }
        return names;
    }

    template<typename CompType>
    const CompType& get(const std::string& compName, const ElementType*) const
    {
        return std::any_cast<const CompType&>(mMap.at(compName));
    }

    template<typename CompType>
    CompType& get(const std::string& compName, ElementType*)
    {
        return std::any_cast<CompType&>(mMap.at(compName));
    }

    template<typename CompType>
    void addCustomComponent(
        const std::string& compName,
        const CompType     c = CompType())
    {
        mMap[compName] = c;
        mCompType.emplace(compName, typeid(CompType));
    }

    void deleteCustomComponent(const std::string& compName)
    {
        mMap.erase(compName);
        mCompType.erase(compName);
    }
};

// vertical, specialized on the boolean.
// Access to the data stored in the Container of elements, trough the
// member function ccVec(elem), where elem is the pointer to the Element
// that is asking for the data
template<typename ElementType>
class CustomComponentsData<ElementType, true>
{
public:
    bool componentExists(const std::string& compName, const ElementType* elem)
        const
    {
        return ccVec(elem).componentExists(compName);
    }

    template<typename CompType>
    bool isComponentOfType(const std::string& compName, const ElementType* elem)
        const
    {
        return ccVec(elem).template isComponentOfType<CompType>(compName);
    }

    std::type_index componentType(
        const std::string& compName,
        const ElementType* elem) const
    {
        return ccVec(elem).componentType(compName);
    }

    template<typename CompType>
    std::vector<std::string> componentNamesOfType(const ElementType* elem) const
    {
        return ccVec(elem).template allComponentNamesOfType<CompType>();
    }

    template<typename CompType>
    const CompType& get(const std::string& compName, const ElementType* elem)
        const
    {
        return std::any_cast<const CompType&>(
            ccVec(elem).template componentVector<CompType>(
                compName)[thisId(elem)]);
    }

    template<typename CompType>
    CompType& get(const std::string& compName, ElementType* elem)
    {
        return std::any_cast<CompType&>(
            ccVec(elem).template componentVector<CompType>(
                compName)[thisId(elem)]);
    }

private:
    uint thisId(const ElementType* elem) const
    {
        assert(elem->parentMesh());
        return elem->index();
    }

    auto& ccVec(ElementType* elem)
    {
        assert(elem->parentMesh());
        // get the vector of custom components
        return elem->parentMesh()->template customComponents<ElementType>();
    }

    const auto& ccVec(const ElementType* elem) const
    {
        assert(elem->parentMesh());
        // get the vector of custom components
        return elem->parentMesh()->template customComponents<ElementType>();
    }
};

} // namespace vcl::comp::detail

#endif // VCL_MESH_COMPONENTS_DETAIL_CUSTOM_COMPONENTS_DATA_H
