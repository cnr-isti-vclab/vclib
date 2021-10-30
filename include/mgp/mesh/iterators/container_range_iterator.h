/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H
#define MESH_ITERATORS_CONTAINER_RANGE_ITERATOR_H

namespace mgp {

template<typename Container, typename Iterator>
class RangeIterator
{
public:
	RangeIterator(
		Container& c,
		Iterator (Container::*beginFunction)(bool),
		Iterator (Container::*endFunction)(void)) :
			c(c), beginFunction(beginFunction), endFunction(endFunction) {};

	Iterator begin() { return (c.*(beginFunction))(); }

	Iterator end() { return (c.*(endFunction))(); }

protected:
	Container& c;
	Iterator (Container::*beginFunction)(bool);
	Iterator (Container::*endFunction)(void);
};

template<typename Container, typename Iterator>
class RangeIteratorDeletedOption : public RangeIterator<Container, Iterator>
{
public:
	RangeIteratorDeletedOption(
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
class ConstRangeIterator
{
public:
	ConstRangeIterator(
		const Container& c,
		ConstIterator (Container::*beginFunction)(bool) const,
		ConstIterator (Container::*endFunction)(void) const) :
			c(c), beginFunction(beginFunction), endFunction(endFunction) {};

	ConstIterator begin() { return (c.*(beginFunction))(); }

	ConstIterator end() { return (c.*(endFunction))(); }

protected:
	const Container& c;
	ConstIterator (Container::*beginFunction)(bool) const;
	ConstIterator (Container::*endFunction)(void) const;
};

template<typename Container, typename ConstIterator>
class ConstRangeIteratorDeletedOption : public ConstRangeIterator<Container, ConstIterator>
{
public:
	ConstRangeIteratorDeletedOption(
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
