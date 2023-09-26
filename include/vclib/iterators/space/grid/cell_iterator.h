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

#ifndef VCL_ITERATORS_SPACE_GRID_CELL_ITERATOR_H
#define VCL_ITERATORS_SPACE_GRID_CELL_ITERATOR_H

#include <vclib/space/point.h>

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

template<int N>
class CellIterator
{
public:
	using difference_type   = ptrdiff_t;
	using value_type        = vcl::Point<uint, N>;
	using reference         = const vcl::Point<uint, N>&;
	using pointer           = const vcl::Point<uint, N>*;
	using iterator_category = std::forward_iterator_tag;

	CellIterator();
	CellIterator(const vcl::Point<uint, N>& end);
	CellIterator(
		const vcl::Point<uint, N>& first,
		const vcl::Point<uint, N>& end);

	reference operator*() const;
	pointer   operator->() const;

	bool operator==(const CellIterator& oi) const;
	bool operator!=(const CellIterator& oi) const;

	CellIterator operator++();
	CellIterator operator++(int);

private:
	vcl::Point<uint, N> it;
	vcl::Point<uint, N> first, end;

	void incrementIt(uint d);
};

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

template<int N>
CellIterator<N>::CellIterator()
{
	it.setConstant(-1);
	first = end = it;
}

template<int N>
CellIterator<N>::CellIterator(const vcl::Point<uint, N>& end) :
		it(end), first(end), end(end)
{
}

template<int N>
CellIterator<N>::CellIterator(
	const vcl::Point<uint, N>& first,
	const vcl::Point<uint, N>& end) :
		it(first),
		first(first), end(end)
{
}

template<int N>
auto CellIterator<N>::operator*() const -> reference
{
	return it;
}

template<int N>
auto CellIterator<N>::operator->() const -> pointer
{
	return &it;
}

template<int N>
bool CellIterator<N>::operator==(const CellIterator& oi) const
{
	return (it == oi.it);
}

template<int N>
bool CellIterator<N>::operator!=(const CellIterator &oi) const
{
	return (it != oi.it);
}

template<int N>
auto CellIterator<N>::operator++() -> CellIterator
{
	uint d = N-1;
	while (d != -1 && it(d) == end(d) - 1) {
		it(d) = first(d);
		d--;
	}
	if (d != -1)
		it(d)++;
	else
		it.setConstant(-1);
	return it;
}

template<int N>
auto CellIterator<N>::operator++(int) -> CellIterator
{
	CellIterator<N> oit = it;
	++(*this);
	return oit;
}

} // namespace vcl

#endif // VCL_ITERATORS_SPACE_GRID_CELL_ITERATOR_H
