/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_PREDICATES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_PREDICATES_H

#include <type_traits>

namespace vcl::comp {

// type-bool
namespace TB {

namespace detail {

template<typename T, template<typename, bool> class Template>
struct IsSpecializationOfComp : std::false_type
{
};

template<template<typename, bool> class Template, typename T, bool B>
struct IsSpecializationOfComp<Template<T, B>, Template> : std::true_type
{
};

template<typename T, template<typename, bool> class Template>
struct IsDerivedFromSpecializationOfComp
{
private:
    template<typename TT, bool B>
    static std::true_type  test(const Template<TT, B>*);
    static std::false_type test(...);

public:
    static constexpr bool value =
        decltype(test(std::declval<std::remove_cvref_t<T>*>()))::value;
};

} // namespace detail

template<typename T, template<typename, bool> class Template>
constexpr bool IsDerivedFromSpecializationOfV =
    detail::IsDerivedFromSpecializationOfComp<T, Template>::value;

} // namespace TB

// type-type-bool
namespace TTB {

namespace detail {

template<typename T, template<typename, typename, bool> class Template>
struct IsSpecializationOfComp : std::false_type
{
};

template<
    template<typename, typename, bool> class Template,
    typename T1,
    typename T2,
    bool B>
struct IsSpecializationOfComp<Template<T1, T2, B>, Template> : std::true_type
{
};

template<typename T, template<typename, typename, bool> class Template>
struct IsDerivedFromSpecializationOfComp
{
private:
    template<typename T1, typename T2, bool B>
    static std::true_type  test(const Template<T1, T2, B>*);
    static std::false_type test(...);

public:
    static constexpr bool value =
        decltype(test(std::declval<std::remove_cvref_t<T>*>()))::value;
};

} // namespace detail

template<typename T, template<typename, typename, bool> class Template>
constexpr bool IsDerivedFromSpecializationOfV =
    detail::IsDerivedFromSpecializationOfComp<T, Template>::value;

} // namespace TTB

// int-type-bool
namespace ITB {

namespace detail {

template<typename T, template<int, typename, bool> class Template>
struct IsSpecializationOfComp : std::false_type
{
};

template<
    template<int, typename, bool> class Template,
    int I,
    typename T,
    bool B>
struct IsSpecializationOfComp<Template<I, T, B>, Template> : std::true_type
{
};

template<typename T, template<int, typename, bool> class Template>
struct IsDerivedFromSpecializationOfComp
{
private:
    template<int I, typename TT, bool B>
    static std::true_type  test(const Template<I, TT, B>*);
    static std::false_type test(...);

public:
    static constexpr bool value =
        decltype(test(std::declval<std::remove_cvref_t<T>*>()))::value;
};

} // namespace detail

template<typename T, template<int, typename, bool> class Template>
constexpr bool IsDerivedFromSpecializationOfV =
    detail::IsDerivedFromSpecializationOfComp<T, Template>::value;

} // namespace ITB

// bool-type-int-bool-type-bool-bool
namespace BTIBTBB {

namespace detail {

template<
    typename T,
    template<bool, typename, int, bool, typename, bool, bool> class Template>
struct IsSpecializationOfComp : std::false_type
{
};

template<
    template<bool, typename, int, bool, typename, bool, bool> class Template,
    bool B1,
    typename T1,
    int  I,
    bool B2,
    typename T2,
    bool B3,
    bool B4>
struct IsSpecializationOfComp<Template<B1, T1, I, B2, T2, B3, B4>, Template> :
        std::true_type
{
};

template<
    typename T,
    template<bool, typename, int, bool, typename, bool, bool> class Template>
struct IsDerivedFromSpecializationOfComp
{
private:
    template<
        bool B1,
        typename T1,
        int  I,
        bool B2,
        typename T2,
        bool B3,
        bool B4>
    static std::true_type  test(const Template<B1, T1, I, B2, T2, B3, B4>*);
    static std::false_type test(...);

public:
    static constexpr bool value =
        decltype(test(std::declval<std::remove_cvref_t<T>*>()))::value;
};

} // namespace detail

template<
    typename T,
    template<bool, typename, int, bool, typename, bool, bool> class Template>
constexpr bool IsDerivedFromSpecializationOfV =
    detail::IsDerivedFromSpecializationOfComp<T, Template>::value;

} // namespace BTIBTBB

// bool-type-type-bool-bool
namespace BTTBB {

namespace detail {

template<
    typename T,
    template<bool, typename, typename, bool, bool> class Template>
struct IsSpecializationOfComp : std::false_type
{
};

template<
    template<bool, typename, typename, bool, bool> class Template,
    bool B1,
    typename T1,
    typename T2,
    bool B2,
    bool B3>
struct IsSpecializationOfComp<Template<B1, T1, T2, B2, B3>, Template> :
        std::true_type
{
};

template<
    typename T,
    template<bool, typename, typename, bool, bool> class Template>
struct IsDerivedFromSpecializationOfComp
{
private:
    template<bool B1, typename T1, typename T2, bool B2, bool B3>
    static std::true_type  test(const Template<B1, T1, T2, B2, B3>*);
    static std::false_type test(...);

public:
    static constexpr bool value =
        decltype(test(std::declval<std::remove_cvref_t<T>*>()))::value;
};

} // namespace detail

template<
    typename T,
    template<bool, typename, typename, bool, bool> class Template>
constexpr bool IsDerivedFromSpecializationOfV =
    detail::IsDerivedFromSpecializationOfComp<T, Template>::value;

} // namespace BTTBB

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_PREDICATES_H
