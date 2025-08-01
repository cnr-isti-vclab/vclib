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

#ifndef VCL_BASE_PAIR_H
#define VCL_BASE_PAIR_H

namespace vcl {

template<typename T1, typename T2>
class RefPair
{
public:
    RefPair(T1& f, T2& s) : first(f), second(s) {}

    T1& first;
    T2& second;
};

template<typename T1, typename T2>
class FirstRefPair
{
public:
    FirstRefPair(T1& f, const T2& s) : first(f), second(s) {}

    T1& first;
    T2  second;
};

template<typename T1, typename T2>
class SecondRefPair
{
public:
    SecondRefPair(const T1& f, T2& s) : first(f), second(s) {}

    T1  first;
    T2& second;
};

template<typename T1, typename T2>
class KeyRefValueRefPair : public RefPair<T1, T2>
{
public:
    KeyRefValueRefPair(T1& f, T2& s) : RefPair<T1, T2>(f, s) {}

    T1& key   = RefPair<T1, T2>::first;  // alias
    T2& value = RefPair<T1, T2>::second; // alias
};

template<typename T1, typename T2>
class KeyValueRefPair : public SecondRefPair<T1, T2>
{
public:
    KeyValueRefPair(const T1& f, T2& s) : SecondRefPair<T1, T2>(f, s) {}

    T1& key   = SecondRefPair<T1, T2>::first;  // alias
    T2& value = SecondRefPair<T1, T2>::second; // alias
};

} // namespace vcl

#endif // VCL_BASE_PAIR_H
