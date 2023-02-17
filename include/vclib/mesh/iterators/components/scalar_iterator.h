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

#ifndef VCL_MESH_ITERATORS_COMPONENTS_SCALAR_ITERATOR_H
#define VCL_MESH_ITERATORS_COMPONENTS_SCALAR_ITERATOR_H

#include <cstddef>
#include <type_traits>

#include <vclib/iterators/range_iterator.h>

namespace vcl {

namespace internal {

template<typename ElemIterator, bool CONST = false>
class ScalarIterator : public ElemIterator
{
public:
	using difference_type   = ptrdiff_t;
	using value_type        = typename ElemIterator::value_type::ScalarType;
	using reference         = typename std::conditional_t<CONST, const value_type&, value_type&>;
	using pointer           = typename std::conditional_t<CONST, const value_type*, value_type*>;

	using ElemIterator::ElemIterator;
	ScalarIterator(const ElemIterator& it) :
			ElemIterator(it)
	{
	}

	reference operator*() const
	{
		return ElemIterator::operator*().scalar();
	}

	pointer operator->() const
	{
		return &ElemIterator::operator*().scalar();
	}
};

} // namespace vcl::internal

template<typename ElemIterator>
using ScalarIterator = internal::ScalarIterator<ElemIterator>;

template<typename ElemIterator>
using ConstScalarIterator = internal::ScalarIterator<ElemIterator, true>;

template<typename MeshType>
using VertexScalarIterator = ScalarIterator<typename MeshType::VertexIterator>;

template<typename MeshType>
using ConstVertexScalarIterator = ConstScalarIterator<typename MeshType::ConstVertexIterator>;

template<typename MeshType>
using VertexScalarRangeIterator = RangeIterator<VertexScalarIterator<MeshType>>;

template<typename MeshType>
using ConstVertexScalarRangeIterator = RangeIterator<ConstVertexScalarIterator<MeshType>>;

template<typename MeshType>
VertexScalarRangeIterator<MeshType> vertexScalars(MeshType& m)
{
	return VertexScalarRangeIterator<MeshType>(
		VertexScalarIterator<MeshType>(m.vertexBegin()),
		VertexScalarIterator<MeshType>(m.vertexEnd()));
}

template<typename MeshType>
ConstVertexScalarRangeIterator<MeshType> vertexScalars(const MeshType& m)
{
	return ConstVertexScalarRangeIterator<MeshType>(
		ConstVertexScalarIterator<MeshType>(m.vertexBegin()),
		ConstVertexScalarIterator<MeshType>(m.vertexEnd()));
}

} // namespace vcl


#endif // VCL_MESH_ITERATORS_COMPONENTS_SCALAR_ITERATOR_H
