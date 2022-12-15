/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_TYPES_H
#define VCL_TYPES_H

#define NOMINMAX

#include <assert.h>
#include <concepts>
#include <type_traits>

// tbb and qt conflicts: if both are linked, we need to first undef Qt's emit
// see: https://github.com/oneapi-src/oneTBB/issues/547
#ifndef Q_MOC_RUN
#if defined(emit)
#undef emit
#include <execution>
#define emit // restore the macro definition of "emit", as it was defined in gtmetamacros.h
#else
#include <execution>
#endif // defined(emit)
#endif // Q_MOC_RUN

// clang does not support Standardization of Parallelism yet -> https://en.cppreference.com/w/cpp/compiler_support
#ifdef __clang__
#define VCL_PARALLEL 
#else
#define VCL_PARALLEL std::execution::par_unseq,
#endif

using uint = unsigned int;
using ushort = unsigned short;

namespace vcl {

/*
 * Utility type that makes possible to treat const pointers in a templated class that can treat a
 * both const and non-const pointer type.
 */

template<typename T>
struct MakeConstPointer
{
	typedef T type;
};

template<typename T>
struct MakeConstPointer<T*>
{
	typedef const T* type;
};

/*
 * Full deduction for the possibility to re-use same code for const and non-const member functions
 * https://stackoverflow.com/a/47369227/5851101
 */

template<typename T>
constexpr T& asConst(T const& value) noexcept
{
	return const_cast<T&>(value);
}
template<typename T>
constexpr T* asConst(T const* value) noexcept
{
	return const_cast<T*>(value);
}
template<typename T>
constexpr T* asConst(T* value) noexcept
{
	return value;
}
template<typename T>
void asConst(T const&&) = delete;

namespace internal {

template< template< typename ...formal > class base >
struct IsDerivedFromImplementation
{
	template< typename ...actual >
	std::true_type
	operator () (base< actual... > *) const;

	std::false_type
	operator () (void *) const;
};

} // namespace vcl::internal

/*
 * Utility class that allows to check if given class 'Derived' is derived from a
 * specialization of a templated class.
 *
 * Given a class X and a templated class C<template T>, it can be used in the following way:
 *
 * using myCheck = vcl::IsDerivedFromTemplateSpecialization<X, C>::type;
 *
 * and will return true if X derives from any specialization of C.
 *
 * https://stackoverflow.com/a/25846080/5851101
 * https://stackoverflow.com/questions/25845536/trait-to-check-if-some-specialization-of-template-class-is-base-class-of-specifi#comment40451928_25846080
 * http://coliru.stacked-crooked.com/a/9feadc62e7594eb2
 */
template< typename derived, template< typename ... > class base >
using IsDerivedFromTemplateSpecialization = typename std::invoke_result< internal::IsDerivedFromImplementation< base >, typename std::remove_cv< derived >::type * >::type;

} // namespace vcl

#endif // VCL_TYPES_H
