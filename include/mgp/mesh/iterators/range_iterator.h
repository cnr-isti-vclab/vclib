/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ITERATORS_RANGE_ITERATOR_H
#define MGP_MESH_ITERATORS_RANGE_ITERATOR_H

namespace mgp {

template<typename Container, typename Iterator>
class RangeIterator
{
public:
	RangeIterator(
		Container& c,
		Iterator (Container::*beginFunction)(),
		Iterator (Container::*endFunction)()) :
			c(c), beginFunction(beginFunction), endFunction(endFunction) {};

	Iterator begin() { return (c.*(beginFunction))(); }

	Iterator end() { return (c.*(endFunction))(); }

protected:
	Container& c;
	Iterator (Container::*beginFunction)();
	Iterator (Container::*endFunction)();
};

template<typename Container, typename ConstIterator>
class ConstRangeIterator
{
public:
	ConstRangeIterator(
		const Container& c,
		ConstIterator (Container::*beginFunction)() const,
		ConstIterator (Container::*endFunction)() const) :
			c(c), beginFunction(beginFunction), endFunction(endFunction) {};

	ConstIterator begin() { return (c.*(beginFunction))(); }

	ConstIterator end() { return (c.*(endFunction))(); }

protected:
	const Container& c;
	ConstIterator (Container::*beginFunction)() const;
	ConstIterator (Container::*endFunction)() const;
};

}

#endif // MGP_MESH_ITERATORS_RANGE_ITERATOR_H
