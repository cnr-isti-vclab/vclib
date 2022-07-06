/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_ELEMENTS_FACE_CONCEPT_H
#define VCL_MESH_ELEMENTS_FACE_CONCEPT_H

#include "face_components.h"

namespace vcl {

template<typename...>
class Face;

namespace face {

// checks if a type derives from vcl::Face<Args...>
template<typename Derived>
using IsDerivedFromFace = IsDerivedFromTemplateSpecialization<Derived, Face>;

// checks if a type is a vcl::Face<Args...>
template<class T>
struct IsAFace : // Default case, no pattern match
		std::false_type
{
};

template<class... Args>
struct IsAFace<Face<Args...>> : // For types matching the pattern Face<Args...>
		std::true_type
{
};

} // namespace vcl::face

template<typename T>
concept FaceConcept = face::IsDerivedFromFace<T>::value || face::IsAFace<T>::value;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_FACE_CONCEPT_H
