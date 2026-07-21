// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
