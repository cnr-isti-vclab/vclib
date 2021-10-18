/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_TYPE_TRAITS_H
#define MGP_TYPE_TRAITS_H

#include <type_traits>

namespace mgp {

template <typename... Ts>
using void_t = void;
template <template <class...> class Trait, class AlwaysVoid, class... Args>
struct detector : std::false_type {};
template <template <class...> class Trait, class... Args>
struct detector<Trait, void_t<Trait<Args...>>, Args...> : std::true_type {};

}

#endif // MGP_TYPE_TRAITS_H
