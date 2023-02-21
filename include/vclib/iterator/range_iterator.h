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

#ifndef VCL_ITERATOR_RANGE_ITERATOR_H
#define VCL_ITERATOR_RANGE_ITERATOR_H

namespace vcl {

template<typename Iterator>
class RangeIterator
{
public:
	RangeIterator(Iterator begin, Iterator end) :
			b(begin), e(end)
	{
	};

	Iterator begin() { return b; }

	Iterator end() { return e; }

protected:
	Iterator b, e;
};

} // namespace vcl

#endif // VCL_ITERATOR_RANGE_ITERATOR_H
