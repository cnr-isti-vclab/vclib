/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "optional_mark.h"

namespace vcl::comp {

template<typename T>
int OptionalMark<T>::mark() const
{
	return B::optCont().mark(thisId());
}

template<typename T>
void OptionalMark<T>::resetMark()
{
	return B::optCont().mark(thisId()) = 0;
}

template<typename T>
template<typename E>
bool OptionalMark<T>::hasSameMark(const E& e) const
{
	if constexpr (std::is_pointer<E>::value) {
		if (e == nullptr) return false;
		return e->mark() == B::optCont().mark(thisId());
	}
	else {
		return e.mark() == B::optCont().mark(thisId());
	}
}

template<typename T>
void OptionalMark<T>::incrementMark()
{
	B::optCont().mark(thisId())++;
}

template<typename T>
void OptionalMark<T>::decrementMark()
{
	B::optCont().mark(thisId())--;
}

template<typename T>
bool OptionalMark<T>::isMarkEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isMarkEnabled();
	else
		return false;
}

template<typename T>
template<typename Element>
void OptionalMark<T>::importFrom(const Element& e)
{
	if constexpr (hasMark<Element>()) {
		if (isMarkEnabled() && isMarkEnabledOn(e)) {
			B::optCont().mark(thisId()) = e.mark();
		}
	}
}

} // namespace vcl::comp
