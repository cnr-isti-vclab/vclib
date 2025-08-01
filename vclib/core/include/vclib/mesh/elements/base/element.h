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

#ifndef VCL_MESH_ELEMENTS_BASE_ELEMENT_H
#define VCL_MESH_ELEMENTS_BASE_ELEMENT_H

#include "base.h"

#include <vclib/mesh/components/base/component.h>
#include <vclib/mesh/components/parent_mesh_pointer.h>

#include <vclib/types.h>

/**
 * @defgroup elements Elements
 * @ingroup mesh
 *
 * @brief List of all the Element classes, along with their concepts and
 * functions.
 */

/**
 * @defgroup element_concepts Element Concepts
 * @ingroup mesh_concepts
 * @ingroup elements
 *
 * @brief List of concepts for types related to the Elements of a Mesh.
 */

namespace vcl {

/* Concepts */

template<typename T>
concept ElementConcept = requires (T&& obj) {
    RemoveRef<T>::ELEMENT_ID;
    typename RemoveRef<T>::Components;
    typename RemoveRef<T>::ParentMeshType;
    { obj.index() } -> std::same_as<uint>;
};

namespace mesh {

template<ElementConcept>
class ElementContainer;

} // namespace mesh

/**
 * @brief The Element class
 *
 * @ingroup elements
 */
template<uint ELEM_ID, typename MeshType, comp::ComponentConcept... Comps>
class Element : public comp::ParentMeshPointer<MeshType>, public Comps...
{
    template<ElementConcept>
    friend class mesh::ElementContainer;

public:
    using ParentMeshType = MeshType;

    /**
     * @brief Components is an alias to a vcl::TypeWrapper that wraps all the
     * Components from which the Element inherits (Comps).
     */
    using Components = TypeWrapper<Comps...>;

    static const uint ELEMENT_ID = ELEM_ID;

    uint index() const
    {
        assert(comp::ParentMeshPointer<MeshType>::parentMesh());
        return comp::ParentMeshPointer<MeshType>::parentMesh()
            ->template elementIndex<ELEM_ID>(this);
    }

    template<uint COMP_ID>
    auto& component()
    {
        using Comp = GetComponentFromID<COMP_ID>::type;
        return *static_cast<Comp*>(this);
    }

    template<uint COMP_ID>
    const auto& component() const
    {
        using Comp = GetComponentFromID<COMP_ID>::type;
        return *static_cast<const Comp*>(this);
    }

    template<typename ElType>
    void importFrom(const ElType& v, bool importRefs = true)
    {
        // we need to call importFrom for each component of the Element,
        // but importFrom must be called only on components that are
        // available (e.g. optional and not enabled)
        (importComponent<Comps>(v, importRefs), ...);
    }

    void serialize(std::ostream& out) const
    {
        // we need to call serialize for each component of the Element,
        // but serialize must be called only on components that are
        // available (e.g. optional and not enabled)
        (serializeComponent<Comps>(out), ...);
    }

    void deserialize(std::istream& in)
    {
        // we need to call deserialize for each component of the Element,
        // but deserialize must be called only on components that are
        // available (e.g. optional and not enabled)
        (deserializeComponent<Comps>(in), ...);
    }

private:
    // hide init and isAvailable members
    void init() {}

    bool isAvailable() const { return true; }

    // init to call after set parent mesh
    void initVerticalComponents() { (construct<Comps>(), ...); }

    template<typename Comp>
    void construct()
    {
        if constexpr (
            comp::IsVerticalComponent<Comp> &&
            comp::HasInitMemberFunction<Comp>) {
            if constexpr (comp::HasIsAvailableMemberFunction<Comp>) {
                if (Comp::isAvailable()) {
                    Comp::init();
                }
            }
            // no possibility to check if is available, it means that is always
            // available
            else {
                Comp::init();
            }
        }
    }

    template<typename Comp, typename ElType>
    void importComponent(const ElType& v, bool importRefs)
    {
        if constexpr (!comp::IsOptionalComponent<Comp>) {
            Comp::importFrom(v, importRefs); // safe to call importFrom
        }
        else {                         // it is optional...
            if (Comp::isAvailable()) { // check if it is available
                Comp::importFrom(v, importRefs);
            }
        }
    }

    template<typename Comp>
    void serializeComponent(std::ostream& out) const
    {
        if constexpr (!comp::IsOptionalComponent<Comp>) {
            Comp::serialize(out); // safe to call serialize
        }
        else {                         // it is optional...
            if (Comp::isAvailable()) { // check if it is available
                Comp::serialize(out);
            }
        }
    }

    template<typename Comp>
    void deserializeComponent(std::istream& in)
    {
        if constexpr (!comp::IsOptionalComponent<Comp>) {
            Comp::deserialize(in); // safe to call deserialize
        }
        else {                         // it is optional...
            if (Comp::isAvailable()) { // check if it is available
                Comp::deserialize(in);
            }
        }
    }

    // Predicate structures

    // Components can be individuated with their ID, which is an unsigned int.
    // This struct sets its bool `value` to true if this Element has a Component
    // with the given unsigned integer COMP_ID. Sets also `type` with a
    // TypeWrapper containing the Component that satisfied the condition. The
    // TypeWrapper will be empty if no Components were found.
    template<uint COMP_ID>
    struct ComponentIDPred
    {
    private:
        template<typename Cont>
        struct SameCmpPred
        {
            static constexpr bool value = Cont::COMPONENT_ID == COMP_ID;
        };

    public:
        // TypeWrapper of the found component, if any
        using type = FilterTypesByCondition<SameCmpPred, Components>::type;
        static constexpr bool value = NumberOfTypes<type>::value == 1;
    };

    template<uint COMP_ID>
    struct GetComponentFromID
    {
    private:
        template<typename>
        struct TypeUnwrapper
        {
        };

        template<typename C>
        struct TypeUnwrapper<TypeWrapper<C>>
        {
            using type = C;
        };

    public:
        using type =
            TypeUnwrapper<typename ComponentIDPred<COMP_ID>::type>::type;
    };
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_BASE_ELEMENT_H
