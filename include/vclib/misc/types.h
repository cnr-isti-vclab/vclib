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

#ifndef VCL_MISC_TYPES_H
#define VCL_MISC_TYPES_H

#define NOMINMAX

#include <assert.h>
#include <concepts>
#include <numeric>
#include <tuple>
#include <type_traits>

using uint = unsigned int;
using ushort = unsigned short;

namespace vcl {

/*
 * Represent a null value of uintm that is the maximum value that can be represented with unsigned
 * int. Allows to fully use all the possible values (except one) that can be represented in an
 * unsigned int, but with the possibility to flag is a value is not initialized or is set to null.
 */
const uint UINT_NULL = std::numeric_limits<uint>::max();


/*******************************************/
/****** VARIADIC TEMPLATES MANAGEMENT ******/
/*******************************************/

/*
 * A simple structure that wraps a list of variadic templates, without instantiating anything.
 * Useful when you need to wrap a list of types, and consider them as a single type.
 */
template<typename...>
struct TypeWrapper
{
};

template<typename... Args>
struct FirstType
{
	using type = typename std::tuple_element<0, std::tuple<Args...>>::type;
};

template<typename... Args>
struct FirstType<TypeWrapper<Args...>>
{
	using type = typename std::tuple_element<0, std::tuple<Args...>>::type;
};

/*
 * Possibility to get the index of a Type T in a pack of types (variadic templates).
 * The pack is composed of U and Us...
 *
 * https://stackoverflow.com/a/71477756/5851101
 */
template <typename T, typename U, typename... Us>
constexpr uint indexInTypePack() {
	if constexpr (std::is_same_v<T, U>) {
		return 0;
	}
	else {
		if constexpr (sizeof...(Us)) { // there is at least another type to check
			return 1 + indexInTypePack<T, Us...>();
		}
		else { // not found
			return UINT_NULL;
		}
	}
}

template<typename T, typename... Us>
struct IndexInTypes
{
	static constexpr uint value = indexInTypePack<T, Us...>();
};

template<typename T, typename... Us>
struct IndexInTypes<T, TypeWrapper<Us...>>
{
	static constexpr uint value = indexInTypePack<T, Us...>();
};

template<typename... Args>
struct NumberOfTypes
{
	static constexpr uint value = sizeof...(Args);
};

template<typename... Args>
struct NumberOfTypes<TypeWrapper<Args...>> : public NumberOfTypes<Args...>
{
};

/* Remove all types that do not satisfy a condition, and get them as a TypeWrapper. */

namespace internal {

template <typename, typename> struct TypeWrapperConstructor;

template <typename  T, typename ...Args>
struct TypeWrapperConstructor<T, TypeWrapper<Args...>>
{
	using type = TypeWrapper<T, Args...>;
};

} // namespace vcl::internal

template <template <class> class, typename ...>
struct FilterTypesByCondition
{
	using type = TypeWrapper<>;
};

/**
 * @brief Removes all types that do not satisfy a condition, and get them as a tuple.
 *
 * Usage:
 *
 * @code{.cpp}
 * using ResTypes = FilterTypesByCondition<std::is_integral, int, float, double, char>::type;
 * static_assert(std::is_same<ResTypes, TypeWrapper<int, char>>::value, "");
 * @endcode
 *
 * ResTuple will be a TypeWrapper<int, char> (int and char are the only integral types)
 */
template <template <class> class Pred, typename Head, typename ...Tail>
struct FilterTypesByCondition<Pred, Head, Tail...>
{
	using type = typename std::conditional<
		Pred<Head>::value,
		typename internal::TypeWrapperConstructor<
			Head,
			typename FilterTypesByCondition<Pred, Tail...>::type>::type,
		typename FilterTypesByCondition<Pred, Tail...>::type>::type;
};

// TypeWrapper specialization
template <template <class> class Pred, typename ...Tail>
struct FilterTypesByCondition<Pred, TypeWrapper<Tail...>>
{
	using type = typename FilterTypesByCondition<Pred, Tail...>::type;
};

/**
 * @brief Its value is set to true if there is at least one type in the given pack Args...
 * that satisfies the given condition
 *
 * Usage:
 *
 * @code{.cpp}
 * // there is a type (int) that is integral
 * static const bool res = TypesContainConditionType<std::is_integral, int, float, double>::value;
 * static_assert(res == true, "");
 * static const bool res2 = TypesContainConditionType<std::is_integral, float, double>::value;
 * static_assert(res2 != true, "");
 * @endcode
 */
template <template <class> class Pred, typename ...Args>
struct TypesContainConditionType
{
private:
	using ResTypes = typename FilterTypesByCondition<Pred, Args...>::type;
public:
	static constexpr bool value = NumberOfTypes<ResTypes>::value != 0;
};

// TypeWrapper specialization
template <template <class> class Pred, typename ...Args>
struct TypesContainConditionType<Pred, TypeWrapper<Args...>>
{
	using type = typename TypesContainConditionType<Pred, Args...>::type;
};

/**
 * @brief The the first type of a pack that satisties the given condition
 *
 * Usage:
 * @code{.cpp}
 * // the first integral type is char
 * using ResType = GetTypeByCondition<std::is_integral, float, double, char, int>::type;
 * static_assert(std::is_same<ResType, char>::value, "");
 * @endcode
 */
template <template <class> class Pred, typename ...Args>
struct GetTypeByCondition
{
private:
	using ResTypes = typename FilterTypesByCondition<Pred, Args...>::type;
public:
	using type = typename FirstType<ResTypes>::type;
};

// TypeWrapper specialization
template <template <class> class Pred, typename ...Args>
struct GetTypeByCondition<Pred, TypeWrapper<Args...>>
{
	using type = typename GetTypeByCondition<Pred, Args...>::type;
};

/*******************************************************************/
/****** POINTERS, REFERENCES AND CONST CORRECTNESS MANAGEMENT ******/
/*******************************************************************/

/*
 * Utility to get clean type from an input type that could have a reference or a pointer.
 */
template<typename T>
using RemoveRefAndPointer =
	typename std::remove_pointer<typename std::remove_reference<T>::type>::type;

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

/*****************************************************/
/****** INHERITANCE AND POLYMORPHISM MANAGEMENT ******/
/*****************************************************/

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
template<typename derived, template<typename...> class base>
using IsDerivedFromTemplateSpecialization = typename std::invoke_result<
	internal::IsDerivedFromImplementation<base>,
	typename std::remove_cv<derived>::type*>::type;

} // namespace vcl

#endif // VCL_MISC_TYPES_H
