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

#ifndef VCL_MESH_ITERATORS_VERTEX_COORD_ITERATOR_H
#define VCL_MESH_ITERATORS_VERTEX_COORD_ITERATOR_H

#include <vclib/iterator/iterator_wrapper.h>
#include <vclib/iterator/range.h>

namespace vcl {

/**
 * @brief The VertexCoordIterator class allows to iterate over the coordinates of the vertices given
 * an iterator It that iterates over the vertices.
 */
template<typename It>
class VertexCoordIterator : public It
{
	using VertexType = typename std::remove_pointer_t<typename It::pointer>;

	using CoordType = typename std::conditional_t<
		std::is_const_v<VertexType>,
		const typename VertexType::CoordType,
		typename VertexType::CoordType>;

public:
	using value_type = typename std::remove_const_t<CoordType>;
	using reference  = CoordType&;
	using pointer    = CoordType*;

	using It::It;
	VertexCoordIterator(const It& it) : It(it) {}

	reference operator*() const { return It::operator*().coord(); }
	pointer operator->() const { return &It::operator*().coord(); }

};

template<typename Rng>
class VertexCoordRange : public vcl::Range<VertexCoordIterator<typename Rng::iterator>>
{
	using Base = vcl::Range<VertexCoordIterator<typename Rng::iterator>>;
public:
	VertexCoordRange(const Rng& r) :
			Base(VertexCoordIterator(r.begin()), VertexCoordIterator(r.end()))
	{
	}
};

// Specialization when using a pointer instead of an actual iterator - forces to use the variant
// with an iterator, by wrapping the pointer type into a forward iterator
template<typename PointerType>
class VertexPointerCoordIterator : public VertexPointerCoordIterator<IteratorWrapper<PointerType>>
{
public:
	using VertexPointerCoordIterator<IteratorWrapper<PointerType>>::VertexPointerCoordIterator;
};

template<typename It> requires (std::is_class_v<It>)
class VertexPointerCoordIterator<It> : public It
{
	using VertexType = typename std::remove_pointer_t<typename It::value_type>;

	using CoordType = typename std::conditional_t<
		std::is_const_v<VertexType>,
		const typename VertexType::CoordType,
		typename VertexType::CoordType>;

public:
	using value_type = typename std::remove_const_t<CoordType>;
	using reference  = CoordType&;
	using pointer    = CoordType*;

	using It::It;
	VertexPointerCoordIterator(const It& it) : It(it) {}

	reference operator*() const { return It::operator*()->coord(); }
	pointer operator->() const { return &It::operator*()->coord(); }

};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_VERTEX_COORD_ITERATOR_H
