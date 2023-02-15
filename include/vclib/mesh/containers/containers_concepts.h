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

#ifndef  VCL_MESH_CONTAINERS_CONCEPTS_H
#define  VCL_MESH_CONTAINERS_CONCEPTS_H

#include <vector>

#include <vclib/misc/types.h>

namespace vcl::mesh {

class ElementContainerTriggerer
{
};

template<typename T>
concept IsElementContainer =
	std::is_base_of<ElementContainerTriggerer, T>::value;

template <typename T>
concept HasEdgeContainer = requires(T o)
{
	typename T::EdgeType;
	o.edge(uint());
};

template <typename T>
concept HasHalfEdgeContainer = requires(T o)
{
	typename T::HalfEdgeType;
	o.halfEdge(uint());
};

template <typename T>
concept HasFaceContainer = requires(T o, const T& co, typename T::FaceType* f)
{
	typename T::FaceType;
	typename T::FaceIterator;
	typename T::ConstFaceIterator;
	typename T::FaceRangeIterator;
	typename T::ConstFaceRangeIterator;
	{ o.face(uint()) } -> std::same_as<typename T::FaceType&>;
	{ co.face(uint()) } -> std::same_as<const typename T::FaceType&>;
	{ co.faceNumber() } -> std::same_as<uint>;
	{ co.faceContainerSize() } -> std::same_as<uint>;
	{ co.deletedFaceNumber() } -> std::same_as<uint>;
	o.deleteFace(uint());
	o.deleteFace(f);
	{ o.faceIndexIfCompact(uint()) } -> std::same_as<uint>;
	{ o.faceCompactIndices() } -> std::same_as<std::vector<int>>;

	{ o.faceBegin() } -> std::same_as<typename T::FaceIterator>;
	{ co.faceBegin() } -> std::same_as<typename T::ConstFaceIterator>;
	{ o.faceEnd() } -> std::same_as<typename T::FaceIterator>;
	{ co.faceEnd() } -> std::same_as<typename T::ConstFaceIterator>;
	{ o.faces() } -> std::same_as<typename T::FaceRangeIterator>;
	{ co.faces() } -> std::same_as<typename T::ConstFaceRangeIterator>;
};

template <typename T>
concept HasVertexContainer = requires(T o, const T& co, typename T::VertexType* v)
{
	typename T::VertexType;
	typename T::VertexIterator;
	typename T::ConstVertexIterator;
	typename T::VertexRangeIterator;
	typename T::ConstVertexRangeIterator;
	{ o.vertex(uint()) } -> std::same_as<typename T::VertexType&>;
	{ co.vertex(uint()) } -> std::same_as<const typename T::VertexType&>;
	{ co.vertexNumber() } -> std::same_as<uint>;
	{ co.vertexContainerSize() } -> std::same_as<uint>;
	{ co.deletedVertexNumber() } -> std::same_as<uint>;
	o.deleteVertex(uint());
	o.deleteVertex(v);
	{ o.vertexIndexIfCompact(uint()) } -> std::same_as<uint>;
	{ o.vertexCompactIndices() } -> std::same_as<std::vector<int>>;

	{ o.vertexBegin() } -> std::same_as<typename T::VertexIterator>;
	{ co.vertexBegin() } -> std::same_as<typename T::ConstVertexIterator>;
	{ o.vertexEnd() } -> std::same_as<typename T::VertexIterator>;
	{ co.vertexEnd() } -> std::same_as<typename T::ConstVertexIterator>;
	{ o.vertices() } -> std::same_as<typename T::VertexRangeIterator>;
	{ co.vertices() } -> std::same_as<typename T::ConstVertexRangeIterator>;
};

} // namespace vcl::mesh

#endif //  VCL_MESH_CONTAINERS_CONCEPTS_H
