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

#ifndef VCL_ITERATOR_POINTER_ITERATOR_H
#define VCL_ITERATOR_POINTER_ITERATOR_H

namespace vcl {

/**
 * @brief The PointerIterator class is an utility iterator wrapper that allows to create, starting
 * from an iterator It that iterates over a type T, an iterator that iterates over T*.
 *
 * This means that, the operator*() will return the pointer of the the element that would be
 * referenced by It. It could be useful when, having a container of elements, we want to construct
 * something using the pointer of its elements with the generic range constructors that take begin
 * and end iterators.
 *
 * For example, let's assume that we have a std::vector<int>. std::vector<int>::iterator allows to
 * iterate over the (maybe const) references of the elements (int& or const int&).
 * If we need to iterate over the pointers to the elements contained in the vector, we just need to
 * create a PointerIterator<std::vector<int>::iterator>.
 *
 * @code{.cpp}
 * std::vector<int> v;
 * using PI = PointerIterator<std::vector<int>::iterator>;
 *
 * // iterate over int
 * for (auto it = v.begin(); it != v.end(); ++it) {
 *     std::cerr(*it); // print value
 * }
 *
 * // iterate over int*
 * for (auto it = PI(v.begin()); it != PI(v.end()); ++it) {
 *     std::cerr(*it); // print pointer of the value
 * }
 *
 * // create a set that stores pointers to elements of the vector
 * std::set<int*> s(PI(v.begin()), PI(v.end()));
 * @endcode
 *
 * @note This iterator does not have operator->(), since it would make no sense to access to members
 * of a pointer.
 *
 * @note This wrapper class works only if the operator*() of the iterator returns a reference to the
 * iterated value. If the operator returns a copy of the iterated value, the usage of this operator
 * will lead to returning a temporary object pointer, which will cause undefined behaviour.
 */
template<typename It>
class PointerIterator : public It
{
public:
	using value_type = typename It::value_type*;
	using reference  = const value_type;

	using It::It;
	PointerIterator(const It& it) : It(it) {}

	reference operator*() const { return It::operator->(); }
};

} // namespace vcl

#endif // VCL_ITERATOR_POINTER_ITERATOR_H
