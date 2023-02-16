/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_ITERATORS_RANGE_ITERATOR_H
#define VCL_ITERATORS_RANGE_ITERATOR_H

namespace vcl {

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

#endif // VCL_ITERATORS_RANGE_ITERATOR_H
