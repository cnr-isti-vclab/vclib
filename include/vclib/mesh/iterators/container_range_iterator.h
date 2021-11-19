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

#ifndef VCL_MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
#define VCL_MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H

namespace vcl {

template<typename Container, typename Iterator>
class ContainerRangeIterator
{
public:
	ContainerRangeIterator(
		Container& c,
		bool       jumpDeleted,
		Iterator (Container::*beginFunction)(bool),
		Iterator (Container::*endFunction)()) :
			c(c),
			beginFunction(beginFunction),
			endFunction(endFunction),
			jumpDeleted(jumpDeleted) {};

	Iterator begin() { return (c.*(beginFunction))(jumpDeleted); }

	Iterator end() { return (c.*(endFunction))(); }

private:
	Container& c;
	Iterator (Container::*beginFunction)(bool);
	Iterator (Container::*endFunction)();
	bool jumpDeleted;
};

template<typename Container, typename ConstIterator>
class ConstContainerRangeIterator
{
public:
	ConstContainerRangeIterator(
		const Container& c,
		bool             jumpDeleted,
		ConstIterator (Container::*beginFunction)(bool) const,
		ConstIterator (Container::*endFunction)() const) :
			c(c),
			beginFunction(beginFunction),
			endFunction(endFunction),
			jumpDeleted(jumpDeleted) {};

	ConstIterator begin() { return (c.*(beginFunction))(jumpDeleted); }

	ConstIterator end() { return (c.*(endFunction))(); }

private:
	const Container& c;
	ConstIterator (Container::*beginFunction)(bool) const;
	ConstIterator (Container::*endFunction)() const;
	bool jumpDeleted;
};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
