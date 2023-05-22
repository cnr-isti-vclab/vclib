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

#ifndef VCL_CONCEPTS_MESH_ELEMENTS_%EL_U%_H
#define VCL_CONCEPTS_MESH_ELEMENTS_%EL_U%_H

#include <vclib/concepts/mesh/components.h>

#include "element.h"

/**
 * @defgroup %EL%_concepts %EL_UC% Concepts
 * @ingroup element_concepts
 *
 * @brief List of concepts for types related to the %EL_UC% Element.
 */

namespace vcl {

template<typename, typename...>
class %EL_UC%;

namespace %EL_NS% {

// checks if a type derives from vcl::%EL_UC%<Args...>
template<typename Derived>
using IsDerivedFrom%EL_UC% = IsDerivedFromTemplateSpecialization<Derived, %EL_UC%>;

// checks if a type is an vcl::%EL_UC%<Args...>
template<class T>
struct IsA%EL_UC% : // Default case, no pattern match
		std::false_type
{
};

template<class... Args>
struct IsA%EL_UC%<%EL_UC%<Args...>> : // For types matching the pattern %EL_UC%<Args...>
		std::true_type
{
};

/**
 * @ingroup %EL%_concepts %EL%_components
 * @{
 */

/* Port concepts into the %EL_NS% namespace */
%COMPONENTS_CONCEPTS%

/** @} */ // end of group

} // namespace vcl::%EL_NS%

/**
 * @brief The %EL_UC%Concept describes how a %EL_UC% element that can be used for a %EL_UC%Container should
 * be organized.
 *
 * The %EL_UC% concept is satisfied for a class E if ALL the following sentences are true:
 * - The class E is vcl::%EL_UC%, or derives from it;
 * - The class E has the BitFlags component (or an equivalent one);
 *
 * @ingroup %EL%_concepts
 */
template<typename T>
concept %EL_UC%Concept =
	T::ELEMENT_TYPE == %EL_U% &&
	(%EL_NS%::IsDerivedFrom%EL_UC%<T>::value || %EL_NS%::IsA%EL_UC%<T>::value) &&
	%EL_NS%::HasBitFlags<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_ELEMENTS_%EL_U%_H
