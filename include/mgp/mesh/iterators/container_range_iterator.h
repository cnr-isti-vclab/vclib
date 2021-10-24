/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
#define MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H

namespace mgp::mesh {

template<typename Container, typename Iterator>
class RangeIterator
{
public:
	RangeIterator(
		Container& c,
		bool       jumpDeleted,
		Iterator (Container::*beginFunction)(bool),
		Iterator (Container::*endFunction)(void)) :
			c(c),
			jumpDeleted(jumpDeleted),
			beginFunction(beginFunction),
			endFunction(endFunction) {};

	Iterator begin() { return (c.*(beginFunction))(jumpDeleted); }

	Iterator end() { return (c.*(endFunction))(); }

private:
	Container& c;
	bool       jumpDeleted;
	Iterator (Container::*beginFunction)(bool);
	Iterator (Container::*endFunction)(void);
};

template<typename Container, typename ConstIterator>
class ConstRangeIterator
{
public:
	ConstRangeIterator(
		const Container& c,
		bool             jumpDeleted,
		ConstIterator (Container::*beginFunction)(bool) const,
		ConstIterator (Container::*endFunction)(void) const) :
			c(c),
			jumpDeleted(jumpDeleted),
			beginFunction(beginFunction),
			endFunction(endFunction) {};

	ConstIterator begin() { return (c.*(beginFunction))(jumpDeleted); }

	ConstIterator end() { return (c.*(endFunction))(); }

private:
	const Container& c;
	bool             jumpDeleted;
	ConstIterator (Container::*beginFunction)(bool) const;
	ConstIterator (Container::*endFunction)(void) const;
};

} // namespace mgp::mesh

#endif // MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
