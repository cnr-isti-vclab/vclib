/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
#define MGP_MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H

#include "range_iterator.h"

namespace mgp {

template<typename Container, typename Iterator>
class ContainerRangeIterator : public RangeIterator<Container, Iterator>
{
public:
	ContainerRangeIterator(
		Container& c,
		bool       jumpDeleted,
		Iterator (Container::*beginFunction)(bool),
		Iterator (Container::*endFunction)(void)) :
			RangeIterator<Container, Iterator>(c, beginFunction, endFunction),
			jumpDeleted(jumpDeleted) {};

	Iterator begin()
	{
		return (
			RangeIterator<Container, Iterator>::c.*
			(RangeIterator<Container, Iterator>::beginFunction))(jumpDeleted);
	}

private:
	bool jumpDeleted;
};

template<typename Container, typename ConstIterator>
class ConstContainerRangeIterator : public ConstRangeIterator<Container, ConstIterator>
{
public:
	ConstContainerRangeIterator(
		const Container& c,
		bool             jumpDeleted,
		ConstIterator (Container::*beginFunction)(bool) const,
		ConstIterator (Container::*endFunction)(void) const) :
			ConstRangeIterator<Container, ConstIterator>(c, beginFunction, endFunction),
			jumpDeleted(jumpDeleted) {};

	ConstIterator begin()
	{
		return (
			ConstRangeIterator<Container, ConstIterator>::c.*
			(ConstRangeIterator<Container, ConstIterator>::beginFunction))(jumpDeleted);
	}

private:
	bool jumpDeleted;
};

} // namespace mgp

#endif // MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
