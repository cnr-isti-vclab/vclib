// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_INHERITANCE_H
#define VCL_BASE_INHERITANCE_H

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

/**
 * @brief Utility class that allows to check if given class 'Derived' is derived
 * from a specialization of a templated class.
 *
 * Given a class X and a templated class C<template T>, it can be used in the
 * following way:
 *
 * @code {.cpp}
 * using myCheck = vcl::IsDerivedFromTemplateSpecialization<X, C>::type;
 *
 * if constexpr (myCheck::value) { ... }
 * @endcode
 *
 * and will return true if X derives from any specialization of C.
 *
 * @tparam derived The class to be checked.
 * @tparam base The templated class to be checked.
 *
 * @ingroup base
 */
// https://stackoverflow.com/a/25846080/5851101
// https://stackoverflow.com/questions/25845536#comment40451928_25846080
// http://coliru.stacked-crooked.com/a/9feadc62e7594eb2
template<typename derived, template<typename...> class base>
using IsDerivedFromTemplateSpecialization = std::invoke_result<
    detail::IsDerivedFromImplementation<base>,
    typename std::remove_cv<derived>::type*>::type;

} // namespace vcl

#endif // VCL_BASE_INHERITANCE_H
