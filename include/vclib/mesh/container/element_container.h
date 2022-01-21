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

#ifndef VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
#define VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H

#include <vector>

#include <vclib/iterators/container_iterator.h>
#include <vclib/iterators/container_range_iterator.h>
#include "../components/vertical/vertical_info.h"
#include "../components/vertical/vectors/vertical_components_vector.h"

namespace vcl::mesh {

template <typename T>
class ElementContainer
{
	friend class comp::VerticalInfo<T>;

	using ElementContainerType = ElementContainer<T>;

public:
	using ElementIterator      = ContainerIterator<std::vector, T>;
	using ConstElementIterator = ConstContainerIterator<std::vector, T>;
	using ElementRangeIterator = ContainerRangeIterator<ElementContainerType, ElementIterator>;
	using ConstElementRangeIterator =
		ConstContainerRangeIterator<ElementContainerType, ConstElementIterator>;

	ElementContainer();

	const T &element(uint i) const;
	T &element(uint i);

	uint elementNumber() const;
	uint elementContainerSize() const;
	uint deletedElementNumber() const;

	void deleteElement(uint i);
	void deleteElement(const T *e);

	uint elementIndexIfCompact(uint i) const;
	std::vector<int> elementCompactIndices() const;

	ElementIterator           elementBegin(bool jumpDeleted = true);
	ElementIterator           elementEnd();
	ConstElementIterator      elementBegin(bool jumpDeleted = true) const;
	ConstElementIterator      elementEnd() const;
	ElementRangeIterator      elements(bool jumpDeleted = true);
	ConstElementRangeIterator elements(bool jumpDeleted = true) const;

protected:
	uint index(const T *e) const;
	void clearElements();

	/**
	 * @brief en: the number of elements in the container. Could be different from elements.size()
	 * due to elements marked as deleted into the container.
	 */
	uint en;
	std::vector<T> vec;
	internal::VerticalComponentsVector<T> optionalVec;
};

}

#include "element_container.cpp"

#endif // VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
