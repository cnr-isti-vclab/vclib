/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_TYPE_TRAITS_H
#define MGP_TYPE_TRAITS_H

#include <type_traits>

namespace mgp {

/**
 * Utility templated tools that allows to detect if a given template has the definition
 * of a given member function.
 *
 * Usage for detecting if the class 'MyClass' has the member function 'myMember(int)':
 *
 * // declare the member function that you are looking for:
 * template<typename T>
 * using hasmyMember_t = decltype(std::declval<T&>().myMember(std::declval<int>()));
 *
 * // create a type trait for that member in a templated T class
 * template <typename T>
 * using hasmyMember = typename detector<hasmyMember_t, void, T>::type;
 *
 * // check if it exists in MyClass using a static if:
 * if constexpr (hasmyMember<MyClass>::value) {
 *    // ...
 * }
 */

template <typename... Ts>
using void_t = void;

template <template <class...> class Trait, class AlwaysVoid, class... Args>
struct detector : std::false_type {};

template <template <class...> class Trait, class... Args>
struct detector<Trait, void_t<Trait<Args...>>, Args...> : std::true_type {};

/**
 * Utility template that allows to shorten the template parameter used to tell
 * if a class D inherits from B (or, vice versa, if B is base of D)
 *
 * Usage:
 *
 * // generic
 * template<class T, typename = void>
 * class MyClass {}; // here, B is not a base of T
 *
 * // specialization
 * template<class T>
 * class MyClass<T, mgp::ifIsBaseOf<B, T>> {}; // here, B is base of T
 */

template <typename B, typename D>
using ifIsBaseOf = std::enable_if_t<std::is_base_of<B,D>::value>;

}

#endif // MGP_TYPE_TRAITS_H
