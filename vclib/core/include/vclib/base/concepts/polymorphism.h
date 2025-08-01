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

#ifndef VCL_BASE_CONCEPTS_POLYMORPHISM_H
#define VCL_BASE_CONCEPTS_POLYMORPHISM_H

#include <concepts>
#include <memory>

namespace vcl {

/**
 * @brief Concept that is evaluated true if T is a cloneable object.
 *
 * A cloneable object is an object that can be cloned by calling the method
 * `clone()`, which returns a shared pointer to the cloned object.
 *
 * Cloneable objects are useful when polymorphism is needed, and the object
 * needs to be copied without knowing the exact type of the object:
 *
 * @code
 * std::shared_ptr<Shape> circle = std::make_shared<Circle>();
 *
 * std::shared_ptr<Shape> clonedCircle = circle->clone();
 * @endcode
 *
 * @tparam T The type to be checked if it is cloneable.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept Cloneable = requires (T&& obj) {
    // TODO: Right now, this concept can be used only with a base class that has
    // a clone method, because the concept requires that the return type of the
    // clone method is a shared pointer to the same class as the object. This is
    // not always the case, especially when the clone method is overridden in
    // derived classes. We need to find a way to make this concept work with
    // overridden clone methods.
    //
    // However, we should also consider the consequences of this change for the
    // class vcl::PolymorphicObjectVector, which relies on this concept to
    // determine if an object can be cloned, and stores objects of the Base
    // class in a vector.
    //
    // Example:
    // class Base {
    // public:
    //     virtual std::shared_ptr<Base> clone() const = 0;
    // };
    //
    // class Derived : public Base {
    // public:
    //     std::shared_ptr<Derived> clone() const { ... }
    // };
    //
    // static_assert(vcl::Cloneable<Base>, ""); // OK
    // static_assert(vcl::Cloneable<Derived>, ""); // Error, but should work
    { obj.clone() } -> std::same_as<std::shared_ptr<std::remove_cvref_t<T>>>;
};

} // namespace vcl

#endif // VCL_BASE_CONCEPTS_POLYMORPHISM_H
