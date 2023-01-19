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

#ifndef VCL_MISC_PAIR_H
#define VCL_MISC_PAIR_H

#include <vclib/misc/types.h>

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
	T2 second;
};

template<typename T1, typename T2>
class SecondRefPair
{
public:
	SecondRefPair(const T1& f, T2& s) : first(f), second(s) {}
	T1 first;
	T2& second;
};

template<typename T1, typename T2>
class KeyRefValueRefPair : public RefPair<T1, T2>
{
public:
	KeyRefValueRefPair(T1& f, T2& s) : RefPair<T1, T2>(f, s) {}
	T1& key = RefPair<T1, T2>::first; // alias
	T2& value = RefPair<T1, T2>::second; // alias
};

template<typename T1, typename T2>
class KeyValueRefPair : public SecondRefPair<T1, T2>
{
public:
	KeyValueRefPair(const T1& f, T2& s) : SecondRefPair<T1, T2>(f, s) {}
	T1& key = SecondRefPair<T1, T2>::first; // alias
	T2& value = SecondRefPair<T1, T2>::second; // alias
};

} // namespace vcl

#endif // VCL_MISC_PAIR_H
