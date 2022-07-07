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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_MARK_H
#define VCL_MESH_COMPONENTS_OPTIONAL_MARK_H

#include "../../concepts/mark.h"
#include "../optional_component.h"

namespace vcl::comp {

template<typename T>
class OptionalMark : public OptionalComponent<T>
{
private:
	using B = VerticalComponent<T>;
	uint thisId() const { return B::index((T*) this); }

public:
	int  mark() const;
	void resetMark();

	template<typename E>
	bool hasSameMark(const E& e) const;

	void incrementMark();
	void decrementMark();

	bool isMarkEnabled() const;

protected:
	template <typename Element>
	void importFrom(const Element& e);
};

} // namespace vcl::comp

#include "optional_mark.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_MARK_H
