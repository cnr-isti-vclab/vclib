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

#ifndef VCL_MESH_COMPONENTS_DETAIL_COMPONENT_DATA_H
#define VCL_MESH_COMPONENTS_DETAIL_COMPONENT_DATA_H

#include <vclib/concepts/mesh/components/component.h>

namespace vcl::comp::detail {

// store the data if horizontal
template<typename Data, bool VERTICAL>
class ComponentData
{
    Data mData;

public:
    template<typename, typename Comp>
    Data& get(Comp*)
    {
        return mData;
    }

    template<typename, typename Comp>
    const Data& get(const Comp*) const
    {
        return mData;
    }

    template<typename, typename Comp>
    constexpr bool isComponentAvailable(const Comp*) const
    {
        return true;
    }
};

// do not store data if vertical; it will be fetched by vertical vectors in the
// element container
template<typename Data>
class ComponentData<Data, true>
{
public:
    /*
     * These member functions allow to access to the data of a vertical
     * component.
     *
     * We are in the in the scenario where, a vertical component ComponentType
     * of an element ElementType wants to access to its data, which is not
     * stored in the Component itself, but in the tuple of vectors stored in the
     * element container.
     *
     * To do that, these functions ask for two template parameters:
     * - ElementType, the class of the element (Vertex, Face)..
     * - ComponentType, the component from which the ElementType is the derived
     * class. And a parameter comp, which is the 'this' pointer of the
     * ComponentType instance that performs the function call.
     *
     * These member functions first perform a static cast from the Component to
     * the Element (this operation is safe also in case of multiple inheritance
     * - see https://stackoverflow.com/questions/65177399/), then access to the
     * parent mesh of the Element and asks for access to the tuple of vectors of
     * vertical components of the element (they are private, but ComponentData
     * is friends of the Mesh). Then, gets the vector of the given component and
     * then the component associated to the index of the element.
     */

    template<typename ElementType, typename ComponentType>
    Data& get(ComponentType* comp)
    {
        ElementType* elem = static_cast<ElementType*>(comp);
        assert(elem->parentMesh());

        // get the tuple of vector of vertical components
        auto& tvc =
            elem->parentMesh()->template verticalComponents<ElementType>();

        // get the vector of the required component
        auto& vc = tvc.template vector<ComponentType>();

        // return the component at the index of this vertex
        return vc[elem->index()];
    }

    template<typename ElementType, typename ComponentType>
    const Data& get(const ComponentType* comp) const
    {
        const ElementType* elem = static_cast<const ElementType*>(comp);
        assert(elem->parentMesh());

        // get the tuple of vector of vertical components
        auto& tvc =
            elem->parentMesh()->template verticalComponents<ElementType>();

        // get the vector of the required component
        auto& vc = tvc.template vector<ComponentType>();

        // return the component at the index of this vertex
        return vc[elem->index()];
    }

    template<typename ElementType, typename ComponentType>
    bool isComponentAvailable(const ComponentType* comp) const
    {
        // just vertical component
        if constexpr (!IsOptionalComponent<ComponentType>) {
            return true;
        }
        // optional component -> need to check at runtime the vector tuple
        else {
            const ElementType* elem = static_cast<const ElementType*>(comp);
            assert(elem->parentMesh());

            // get the tuple of vector of vertical components
            auto& tvc =
                elem->parentMesh()->template verticalComponents<ElementType>();

            return tvc.template isComponentEnabled<ComponentType>();
        }
    }
};

} // namespace vcl::comp::detail

#endif // VCL_MESH_COMPONENTS_DETAIL_COMPONENT_DATA_H
