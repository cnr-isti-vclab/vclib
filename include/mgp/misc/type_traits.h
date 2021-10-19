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
 * using hasmyMembery_t = decltype(std::declval<const T&>().myMember(std::declval<int>()));
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

}

#endif // MGP_TYPE_TRAITS_H
