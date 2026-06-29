// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_TEMPLATED_TYPE_WRAPPER_H
#define VCL_BASE_TEMPLATED_TYPE_WRAPPER_H

#include "variadic_templates.h"

namespace vcl {

template<template<typename...> typename... Args>
struct TemplatedTypeWrapper
{
    static constexpr uint size() { return sizeof...(Args); }
};

// note: specialization from variadic_templates.h
template<template<typename...> typename... Args>
struct FirstType<TemplatedTypeWrapper<Args...>>
{
    template<typename... T>
    using type = std::tuple_element<0, std::tuple<Args<T>...>>::type;
};

} // namespace vcl

#endif // VCL_BASE_TEMPLATED_TYPE_WRAPPER_H
