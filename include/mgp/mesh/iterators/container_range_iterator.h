/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
#define MGP_MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H

namespace mgp {

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

} // namespace mgp

#endif // MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
