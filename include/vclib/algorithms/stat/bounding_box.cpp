/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "bounding_box.h"

namespace vcl {

template<PointConcept PointType>
auto boundingBox(const PointType &p)
{
	vcl::Box<PointType> b;
	b.add(p);
	return b;
}

template<MeshConcept MeshType>
auto boundingBox(const MeshType& m)
{
	using VertexType = typename MeshType::VertexType;
	vcl::Box<typename VertexType::CoordType> b;

	for (const VertexType& v : m.vertices()) {
		b.add(v.coord());
	}

	return b;
}

template<VertexConcept VertexType>
auto boundingBox(const VertexType& v)
{
	vcl::Box<typename VertexType::CoordType> b;
	b.add(v.coord());
	return b;
}

template<VertexConcept VertexType>
auto boundingBox(const VertexType* v)
{
	vcl::Box<typename VertexType::CoordType> b;
	b.add(v->coord());
	return b;
}

template<FaceConcept FaceType>
auto boundingBox(const FaceType& f)
{
	using VertexType = typename FaceType::VertexType;

	vcl::Box<typename VertexType::CoordType> b;
	for (const VertexType* v : f.vertices())
		b.add(v->coord());
	return b;
}

template<FaceConcept FaceType>
auto boundingBox(const FaceType* f)
{
	using VertexType = typename FaceType::VertexType;

	vcl::Box<typename VertexType::CoordType> b;
	for (const VertexType* v : f->vertices())
		b.add(v->coord());
	return b;
}

template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType& e)
{
	using VertexType = typename EdgeType::VertexType;

	vcl::Box<typename VertexType::CoordType> b;
	for (const VertexType* v : e.vertices())
		b.add(v->coord());
	return b;
}

template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType* e)
{
	using VertexType = typename EdgeType::VertexType;

	vcl::Box<typename VertexType::CoordType> b;
	for (const VertexType* v : e->vertices())
		b.add(v->coord());
	return b;
}

template<typename Iterator>
auto boundingBox(Iterator begin, Iterator end)
{
	vcl::Box<typename Iterator::value_type> b;

	for (; begin != end; ++begin)
		b.add(boundingBox(*begin));

	return b;
}

} // namespace vcl
