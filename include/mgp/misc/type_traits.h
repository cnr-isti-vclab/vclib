/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_TYPE_TRAITS_H
#define MGP_TYPE_TRAITS_H

#include <type_traits>

namespace mgp {

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

template<typename B, typename D>
using ifIsBaseOf = std::enable_if_t<std::is_base_of<B, D>::value>;

} // namespace mgp

#endif // MGP_TYPE_TRAITS_H
