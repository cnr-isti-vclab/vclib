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

#ifndef VCL_CONCEPTS_POLYMORPHISM_H
#define VCL_CONCEPTS_POLYMORPHISM_H

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
concept Cloneable = requires (const T& obj) {
    { obj.clone() } -> std::same_as<std::shared_ptr<T>>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_POLYMORPHISM_H
