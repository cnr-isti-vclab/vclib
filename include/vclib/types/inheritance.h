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

#ifndef VCL_TYPES_INHERITANCE_H
#define VCL_TYPES_INHERITANCE_H

#include <type_traits>

namespace vcl {

namespace detail {

template<template<typename... formal> class base>
struct IsDerivedFromImplementation
{
    template<typename... actual>
    std::true_type operator()(base<actual...>*) const;

    std::false_type operator()(void*) const;
};

} // namespace detail

/*
 * Utility class that allows to check if given class 'Derived' is derived from a
 * specialization of a templated class.
 *
 * Given a class X and a templated class C<template T>, it can be used in the
 * following way:
 *
 * using myCheck = vcl::IsDerivedFromTemplateSpecialization<X, C>::type;
 *
 * if constexpr (myCheck::value) { ... }
 *
 * and will return true if X derives from any specialization of C.
 *
 * https://stackoverflow.com/a/25846080/5851101
 * https://stackoverflow.com/questions/25845536#comment40451928_25846080
 * http://coliru.stacked-crooked.com/a/9feadc62e7594eb2
 */
template<typename derived, template<typename...> class base>
using IsDerivedFromTemplateSpecialization = std::invoke_result<
    detail::IsDerivedFromImplementation<base>,
    typename std::remove_cv<derived>::type*>::type;

} // namespace vcl

#endif // VCL_TYPES_INHERITANCE_H
