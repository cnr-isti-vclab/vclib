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

#ifndef VCL_MISC_MARK_MARKABLE_VECTOR_H
#define VCL_MISC_MARK_MARKABLE_VECTOR_H

#include <vector>

#include <vclib/misc/types.h>

namespace vcl {

template<typename ValueType>
class MarkableVector
{
public:
	using Iterator = typename std::vector<ValueType>::iterator;
	using ConstIterator = typename std::vector<ValueType>::iterator;
	using ReverseIterator = typename std::vector<ValueType>::reverse_iterator;
	using ConstReverseIterator = typename std::vector<ValueType>::const_reverse_iterator;

	MarkableVector();

	MarkableVector(std::size_t size);
	MarkableVector(std::size_t size, const ValueType& defaultValue);

	template<typename ValueIterator>
	MarkableVector(ValueIterator begin, ValueIterator end);

	bool empty() const;
	std::size_t size() const;

	void clear();
	void reserve(std::size_t size);
	std::size_t capacity() const;
	void resize(std::size_t size);
	void resize(std::size_t size, const ValueType& defaultValue);

	void insert(uint p, const ValueType& v);
	void erase(uint p);
	void pushBack(const ValueType& v);
	void popBack();

	bool isMarked(uint i) const;
	void mark(uint i) const;
	void unMarkAll() const;

	ValueType* data();
	const ValueType* data() const;

	ValueType& at(uint i);
	const ValueType& at(uint i) const;
	ValueType& operator[](uint i);
	const ValueType& operator[](uint i) const;

	ValueType& front();
	const ValueType& front() const;
	ValueType& back();
	const ValueType& back() const;

	Iterator begin();
	ConstIterator begin() const;
	Iterator end();
	ConstIterator end() const;

private:
	std::vector<ValueType> vector;
	mutable std::vector<uint> marks;
	mutable uint m = 1;
};

} // namespace vcl

#include "markable_vector.cpp"

#endif // VCL_MISC_MARK_MARKABLE_VECTOR_H
