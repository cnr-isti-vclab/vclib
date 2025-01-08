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

#ifndef FOO_COMPONENT_H
#define FOO_COMPONENT_H

#include <vclib/types.h>

/*
 * This is an example of a simple plain component class.
 * The component can be used only as a horizontal component, for elements and
 * meshes.
 */

// a component needs a concept, that allows to test if other elements/meshes
// have the same component.
// the concept should just check if the element/mesh has the member functions
// that are part of the component class.
template<typename T>
concept HasFooComponent = requires (T t, const T& ct) {
    // accessor to the foo component, returns int&
    { t.foo() } -> std::same_as<int&>;

    // const accessor to the foo component
    { ct.foo() } -> std::same_as<int>;
};

// class of the Foo component
// there are two main requirements:
// - the component must have a static const uint COMPONENT_ID
// - the component must have a protected importFrom member function
class FooComponent
{
public:
    // first requirement: an unique static const uint ID of the component
    // vcl::CompId::COMPONENTS_NUMBER is the number of components that are
    // already defined in VCLib. You can use it to avoid collisions.
    static const unsigned int COMPONENT_ID = vcl::CompId::COMPONENTS_NUMBER + 0;

    // any member that you want to add to the component
    int& foo() { return data; }

    int foo() const { return data; }

protected:
    // second requirement: a protected `importFrom` member function
    template<typename Element> // another element, maybe from another mesh type
    void importFrom(const Element& e, bool = true)
    {
        // will import the foo component from the element e only if it also has
        // the foo component
        if constexpr (HasFooComponent<Element>) { // compile time check
            data = e.foo();
        }
    }

private:
    // the data that you want to store in a component
    int data;
};

static_assert(
    HasFooComponent<FooComponent>,
    "Make sure that the concept is satisfied with its component.");

#endif // FOO_COMPONENT_H
