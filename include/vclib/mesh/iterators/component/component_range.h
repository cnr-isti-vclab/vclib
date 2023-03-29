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

#ifndef VCL_MESH_ITERATORS_COMPONENT_COMPONENT_RANGE_H
#define VCL_MESH_ITERATORS_COMPONENT_COMPONENT_RANGE_H

#include <vclib/iterator/range.h>

namespace vcl::internal {

/**
 * @brief The ComponentRange class creates a Range class (begin()/end() member functions) over a
 * component, starting from:
 * - Rng: a Range on elements on which iterate (vertices, faces)
 * - Iterator: the Iterator that allows to iterate over the component starting from the Iterator
 *             of the range Rng
 *
 * Example: we want to create a Range iterator that allows to iterate over Vertex Coordinates.
 * We already have:
 * - A VertexRange class that iterate over vertices (e.g., given a Mesh m, m.vertices() return an istance
 *   of a Range class);
 * - A CoordIterator class templated over a VertexIterator (the one used by the the Range)
 * Then, we can create our CoordRange in the following way:
 *
 * using CoordRange = internal::ComponentRange<VertexRange, CoordIterator>;
 *
 * Or, we could template it on just on a range of vertices (the actual type will be deduced
 * trough the ComponentRange constructor):
 *
 * template<typename Rng>
 * using CoordRange = internal::ComponentRange<Rng, CoordIterator>;
 *
 * Usage:
 *
 * for (CoordType& c : CoordRange(m.vertices()) {
 *     c *= 2; // scale by 2 factor
 * }
 */
template<typename Rng, template <typename> typename Iterator>
class ComponentRange : public vcl::Range<Iterator<typename Rng::iterator>>
{
	using Base = vcl::Range<Iterator<typename Rng::iterator>>;
public:
	ComponentRange(const Rng& r) :
			Base(Iterator(r.begin()), Iterator(r.end()))
	{
	}
};

} // namespace vcl::internal

#endif // VCL_MESH_ITERATORS_COMPONENT_COMPONENT_RANGE_H
