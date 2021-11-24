/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_MARK_H
#define VCL_MESH_COMPONENTS_OPTIONAL_MARK_H

#include "optional_info.h"

namespace vcl::comp {

template<typename T>
class OptionalMark : public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;
	unsigned int thisId() const { return B::index((T*)this); }

public:
	OptionalMark() {}
	int mark() const { return B::optCont().mark(thisId()); }
	void resetMark() { return B::optCont().mark(thisId()) = 0; }

	template<typename E>
	bool hasSameMark(const E& e) const
	{
		if constexpr (std::is_pointer<E>::value) {
			return e->mark() == B::optCont().mark(thisId());
		}
		else {
			return e.mark() == B::optCont().mark(thisId());
		}
	}

	void incrementMark() { B::optCont().mark(thisId())++; }
	void decrementMark() { B::optCont().mark(thisId())--; }
};

/**
 * Detector to check if a class has (inherits) OptionalColor
 */

template<typename T>
using hasOptionalMarkT = std::is_base_of<OptionalMark<T>, T>;

template<typename T>
bool constexpr hasOptionalMark()
{
	return hasOptionalMarkT<T>::value;
}

}

#endif // VCL_MESH_COMPONENTS_OPTIONAL_MARK_H
