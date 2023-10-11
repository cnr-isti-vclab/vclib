/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef BAR_COMPONENT_H
#define BAR_COMPONENT_H

#include <vclib/mesh/components/bases/component.h>
#include <vclib/types.h>

/*
 * This is an example of a component class, that can be used as horizontal,
 * vertical and optional in an Element of a Mesh.
 * Due to the complexity of this class, VCLib provides a vcl::Component class
 * that can be used as a base class.
 *
 * In this example, the component stores two values: an integer that can be
 * accessed trough the member function `foo()`, and a double that can be
 * accessed trough the member function `bar()`.
 */

// a component needs a concept, that allows to test if other elements/meshes
// have the same component.
// the concept should just check if the element/mesh has the member functions
// that are part of the component class.
template<typename T>
concept HasBarComponent = requires(T t, const T& ct)
{
    // accessor to the int value of the bar component, returns int&
    { t.foo() } -> std::same_as<int&>;

    // const accessor to the int value of the bar component
    { ct.foo() } -> std::same_as<int>;

    // accessor to the double value of the bar component, returns int&
    { t.bar() } -> std::same_as<double&>;

    // const accessor to the double value of the bar component
    { ct.bar() } -> std::same_as<double>;
};

// if you want, you can also define a concept that checks if an element has
// the bar component optional -- it could be useful in some contexts
template<typename T>
concept HasOptionalColor =
    HasBarComponent<T> &&
    vcl::comp::IsOptionalComponent<typename T::BarComponentT>;

// define a constant uint that identifies the component (same idea used for
// COMPONENT_ID in the FooComponent)
inline static const uint BAR_COMPONENT = vcl::COMPONENTS_NUMBER + 0;

namespace internal {

// we wrap all the data stored by the component in a struct.
// in this case we could use also a std::pair, but it doesn't matter
// the only important thing is to have all the data wrapped in a single type
struct BarData
{
    int foo;
    double bar;
};

} // namespace internal

// We declare here a function that checks at runtime if the component is
// available. The check is a combination of a compile time check (if the
// component actually exists) and a runtime check (in case the component is
// optional, if it is enabled).
// You can use the ElementOrMeshConcept if you plan to use this component also
// on a Mesh. If you plan to use it only on Elements, use the ElementConcept.
// We will implement the function later
bool isBarComponentAvailableOn(const vcl::ElementOrMeshConcept auto& element);

// class of the Bar component
// we called it BarComponentT because BarComponent will be aliased without
// template arguments later (and will be the horizontal component).
//
// template arguments are two:
// - ElementType: void if the component will be horizontal, or the type of the
//   Element that will have the component if vertical;
// - OPTIONAL: true if the component will be optional. Will be valid only if
//   ElementType != void
//
// The class inherits from the vcl::comp::Component class, that has a list of
// template arguments.
template<typename ElementType = void, bool OPTIONAL = false>
class BarComponentT :
        public vcl::comp::Component<

            // CRTP: the first argument is always the component class itself
            BarComponentT<ElementType, OPTIONAL>,

            // ID of the component, same used in the FooComponent, but in this
            // case it is a template argument of the vcl::comp::Component class
            BAR_COMPONENT,

            // The type of the data that the component stores. It is needed
            // because if vertical, the mesh must know what data to store
            internal::BarData,

            // Same ElementType argument of the BarComponentT class
            ElementType,

            // Same OPTIONAL argument of the BarComponentT class
            OPTIONAL>
{
    // alias of the base vcl::comp::Component class - just for internal use
    // to access the member functions of the base class
    using Base = vcl::comp::Component<
        BarComponentT<ElementType, OPTIONAL>,
        BAR_COMPONENT,
        internal::BarData,
        ElementType,
        OPTIONAL>;

public:
    // any member that you want to add to the component

    // we access the data stored by calling Base::data(). The returned type
    // is the same of the third template argument of the vcl::comp::Component
    // class -- in this case, internal::BarData
    int& foo() { return Base::data().foo; }
    int foo() const { return Base::data().foo; }

    double& bar() { return Base::data().bar; }
    double bar() const { return Base::data().bar; }

protected:
    // first requirement: a protected `importFrom` member function
    template<typename Element> // another element, maybe from another mesh type
    void importFrom(const Element& e)
    {
        // will import the bar component from the element e only if it also has
        // the bar component
        if constexpr (HasBarComponent<Element>) { // compile time check
            if (isBarComponentAvailableOn(e)) { // runtime check
                foo() = e.foo();
                bar() = e.bar();
            }
        }
    }
};

// implementation of the availability checker function
bool isBarComponentAvailableOn(const vcl::ElementOrMeshConcept auto& element)
{
    // we use the utility function provided by vclib, using the ID of the
    // component
    return vcl::comp::isComponentAvailableOn<BAR_COMPONENT>(element);
}

// Aliases
using BarComponent = BarComponentT<>; // horizontal component

template<typename ElementType>
using VerticalBarComponent = BarComponentT<ElementType>; // vertical component

template<typename ElementType>
using OptionalBarComponent = BarComponentT<ElementType, true>; // optional

static_assert(
    HasBarComponent<BarComponent>,
    "Make sure that the concept is satisfied with its component.");

#endif // BAR_COMPONENT_H
