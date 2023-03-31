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

#ifndef VCL_MESH_ITERATORS_COMPONENT_COMPONENT_VIEW_H
#define VCL_MESH_ITERATORS_COMPONENT_COMPONENT_VIEW_H

#include <vclib/iterators/view.h>

namespace vcl::internal {

/**
 * @brief The ComponentView class creates a View class (begin()/end() member functions) over a
 * component, starting from:
 * - Rng: a View on elements on which iterate (vertices, faces)
 * - Iterator: the Iterator that allows to iterate over the component starting from the Iterator
 *             of the view ViewType
 *
 * Example: we want to create a View iterator that allows to iterate over Vertex Coordinates.
 * We already have:
 * - A VertexView class that iterate over vertices (e.g., given a Mesh m, m.vertices() return an istance
 *   of a View class);
 * - A CoordIterator class templated over a VertexIterator (the one used by the the View)
 * Then, we can create our CoordView in the following way:
 *
 * using CoordView = internal::ComponentView<VertexView, CoordIterator>;
 *
 * Or, we could template it on just on a view of vertices (the actual type will be deduced
 * trough the ComponentView constructor):
 *
 * template<typename Rng>
 * using CoordView = internal::ComponentView<Rng, CoordIterator>;
 *
 * Usage:
 *
 * for (CoordType& c : CoordView(m.vertices()) {
 *     c *= 2; // scale by 2 factor
 * }
 */
template<typename ViewType, template <typename> typename Iterator>
class ComponentView : public vcl::View<Iterator<typename ViewType::iterator>>
{
	using Base = vcl::View<Iterator<typename ViewType::iterator>>;
public:
	ComponentView(const ViewType& r) :
			Base(Iterator(r.begin()), Iterator(r.end()))
	{
	}
};

} // namespace vcl::internal

#endif // VCL_MESH_ITERATORS_COMPONENT_COMPONENT_VIEW_H
