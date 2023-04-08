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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_FACE_BASE_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_FACE_BASE_ITERATOR_H

#include "base_iterator.h"

namespace vcl::internal {

// This class is for internal use, and it contains only static member functions.
// Its functions are meant to be used by the iterators that iterate over Face components
// trough its half edge. Functions are provided here to avoid code duplication, and are
// the functions for the operators++ and --, prefix and postfix.
// These functions allow to move to the next / prev half edge checking for the end of the iteration
// and returning the correct iterator after the operation.
// This class is meant to be declared as friend of the iterator,
// and will access to the `current` and `end` members of the iterator passed as parameter of each
// function (which should be the *this iterator).
class FaceBaseIterator {
public:

	template<typename It>
	static It& increment(It& it)
	{
		it.current = it.current->next();
		if (it.current == it.end) it.current = nullptr;
		return it;
	}

	template<typename It>
	static It postIncrement(It& it)
	{
		It tmp = it;
		increment(it);
		return tmp;
	}

	template<typename It>
	static It& decrement(It& it)
	{
		it.current = it.current->prev();
		if (it.current == it.end) it.current = nullptr;
		return it;
	}

	template<typename It>
	static It postDecrement(It& it)
	{
		It tmp = it;
		decrement(it);
		return tmp;
	}
};

} // namespace vcl::internal

#endif // VCL_MESH_ITERATORS_HALF_EDGE_FACE_BASE_ITERATOR_H
