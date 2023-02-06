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

#include "face_half_edge_reference.h"

namespace vcl::comp {

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::init()
{
	ohe() = nullptr;
	texIndex() = 0;
}

template<typename HE, typename El, bool h>
const HE* FaceHalfEdgeReference<HE, El, h>::outerHalfEdge() const
{
	return ohe();
}

template<typename HE, typename El, bool h>
HE*& FaceHalfEdgeReference<HE, El, h>::outerHalfEdge()
{
	return ohe();
}

template<typename HE, typename El, bool h>
uint FaceHalfEdgeReference<HE, El, h>::numberHoles() const
{
	return ihe().size();
}

template<typename HE, typename El, bool h>
const HE* FaceHalfEdgeReference<HE, El, h>::innerHalfEdge(uint i) const
{
	return ihe()[i];
}

template<typename HE, typename El, bool h>
HE*& FaceHalfEdgeReference<HE, El, h>::innerHalfEdge(uint i)
{
	return ihe()[i];
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::resizeInnerHalfEdges(uint n)
{
	ihe().resize(n);
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::pushInnerHalfEdge(HE* he)
{
	ihe().push_back(he);
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::insertInnerHalfEdge(uint i, HE* he)
{
	assert(i < ihe().size());
	ihe().insert(ihe().begin() + i, he);
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::eraseInnerHalfEdge(uint i)
{
	assert(i < ihe().size());
	ihe().erase(ihe().begin() + i);
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::clearInnerHalfEdges()
{
	ihe().clear();
}

template<typename HE, typename El, bool h>
uint FaceHalfEdgeReference<HE, El, h>::vertexNumber() const
{
	uint cnt = 0;
	for (const Vertex* v : vertices())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::Vertex*& FaceHalfEdgeReference<HE, El, h>::vertex(uint i)
{
	uint           j  = 0;
	VertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
const typename FaceHalfEdgeReference<HE, El, h>::Vertex*
FaceHalfEdgeReference<HE, El, h>::vertex(uint i) const
{
	uint                j  = 0;
	ConstVertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::Vertex*& FaceHalfEdgeReference<HE, El, h>::vertexMod(int i)
{
	uint           j  = 0;
	VertexIterator it = VertexIterator(ohe(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
const typename FaceHalfEdgeReference<HE, El, h>::Vertex*
FaceHalfEdgeReference<HE, El, h>::vertexMod(int i) const
{
	uint                j = 0;
	ConstVertexIterator it =
		ConstVertexIterator(ohe(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::setVertex(Vertex* v, uint i)
{
	vertex(i) = v;
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::setVertices(const std::vector<Vertex*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Vertex*& v : vertices()) {
		v = list[i++];
	}
}

template<typename HE, typename El, bool h>
bool FaceHalfEdgeReference<HE, El, h>::containsVertex(const Vertex* v) const
{
	return findVertex(v) != vertexEnd();
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::VertexIterator
FaceHalfEdgeReference<HE, El, h>::findVertex(const Vertex* v)
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h>::findVertex(const Vertex* v) const
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HE, typename El, bool h>
int FaceHalfEdgeReference<HE, El, h>::indexOfVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : vertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool h>
int FaceHalfEdgeReference<HE, El, h>::indexOfEdge(const Vertex* v1, const Vertex* v2) const
{
	int vid = 0;
	for (const HE* he : halfEdges()) {
		if (he->fromVertex() == v1 && he->toVertex() == v2)
			return vid;
		if (he->toVertex() == v1 && he->fromVertex() == v2)
			return vid;
		vid++;
	}
	return -1;
}

template<typename HE, typename El, bool h>
uint FaceHalfEdgeReference<HE, El, h>::adjFacesNumber() const
{
	return vertexNumber();
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::Face*& FaceHalfEdgeReference<HE, El, h>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
const typename FaceHalfEdgeReference<HE, El, h>::Face*
FaceHalfEdgeReference<HE, El, h>::adjFace(uint i) const
{
	uint                      j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::Face*& FaceHalfEdgeReference<HE, El, h>::adjFaceMod(int i)
{
	uint                 j = 0;
	AdjacentFaceIterator it =
		AdjacentFaceIterator(ohe(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
const typename FaceHalfEdgeReference<HE, El, h>::Face*
FaceHalfEdgeReference<HE, El, h>::adjFaceMod(int i) const
{
	uint                      j = 0;
	ConstAdjacentFaceIterator it =
		ConstAdjacentFaceIterator(ohe(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::setAdjFaces(const std::vector<Face*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HE, typename El, bool h>
bool FaceHalfEdgeReference<HE, El, h>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool h>
int FaceHalfEdgeReference<HE, El, h>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool h>
vcl::Color &FaceHalfEdgeReference<HE, El, h>::wedgeColor(uint i) requires HasColor<HE>
{
	uint                j  = 0;
	WedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
const vcl::Color&
FaceHalfEdgeReference<HE, El, h>::wedgeColor(uint i) const requires HasColor<HE>
{
	uint                     j  = 0;
	ConstWedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
vcl::Color& FaceHalfEdgeReference<HE, El, h>::wedgeColorMod(int i) requires HasColor<HE>
{
	uint                j  = 0;
	WedgeColorsIterator it = WedgeColorsIterator(ohe(), nullptr); // this iterator does not have end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
const vcl::Color&
FaceHalfEdgeReference<HE, El, h>::wedgeColorMod(int i) const requires HasColor<HE>
{
	uint                j  = 0;
	ConstWedgeColorsIterator it =
		ConstWedgeColorsIterator(ohe(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::setWedgeColor(const vcl::Color& t, uint i) requires
	HasColor<HE>
{
	wedgeColor(i) = t;
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::setWedgeColors(
	const std::vector<vcl::Color>& list) requires HasColor<HE>
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (vcl::Color& c : wedgeColors()) {
		c = list[i++];
	}
}

template<typename HE, typename El, bool h>
bool FaceHalfEdgeReference<HE, El, h>::isWedgeColorsEnabled() const requires HasColor<HE>
{
	if constexpr (HasOptionalColor<HE>) {
		return ohe()->isColorEnabled();
	}
	else {
		return true;
	}
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h>::wedgeTexCoord(uint i)
{
	uint                   j  = 0;
	WedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
const typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h>::wedgeTexCoord(uint i) const
{
	uint                        j  = 0;
	ConstWedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h>::wedgeTexCoordMod(int i)
{
	uint                   j  = 0;
	WedgeTexCoordsIterator it =
		WedgeTexCoordsIterator(ohe(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
const typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h>::wedgeTexCoordMod(int i) const
{
	uint                        j  = 0;
	ConstWedgeTexCoordsIterator it =
		ConstWedgeTexCoordsIterator(ohe(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
void FaceHalfEdgeReference<HE, El, h>::setWedgeTexCoord(const typename OHE::TexCoordType& t, uint i)
{
	wedgeTexCoord(i) = t;
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
void FaceHalfEdgeReference<HE, El, h>::setWedgeTexCoords(
	const std::vector<typename OHE::TexCoordType>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (typename OHE::TexCoordType& c : wedgeTexCoords()) {
		c = list[i++];
	}
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
short& FaceHalfEdgeReference<HE, El, h>::textureIndex()
{
	return texIndex();
}

template<typename HE, typename El, bool h>
template<HasTexCoord OHE>
short FaceHalfEdgeReference<HE, El, h>::textureIndex() const
{
	return texIndex();
}

template<typename HE, typename El, bool h>
bool FaceHalfEdgeReference<HE, El, h>::isWedgeTexCoordsEnabled() const requires HasTexCoord<HE>
{
	if constexpr (HasOptionalTexCoord<HE>) {
		return ohe()->isTexCoordEnabled();
	}
	else {
		return true;
	}
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::adjFaceBegin()
{
	return AdjacentFaceIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::adjFaceBegin(HE* he)
{
	assert(he->face() == this);
	return AdjacentFaceIterator(he);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::adjFaceBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstAdjacentFaceIterator(he);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::AdjacentFaceRangeIterator
FaceHalfEdgeReference<HE, El, h>::adjFaces()
{
	return AdjacentFaceRangeIterator(
		*this, &FaceHalfEdgeReference::adjFaceBegin, &FaceHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstAdjacentFaceRangeIterator
FaceHalfEdgeReference<HE, El, h>::adjFaces() const
{
	return ConstAdjacentFaceRangeIterator(
		*this, &FaceHalfEdgeReference::adjFaceBegin, &FaceHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::HalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::haflEdgeBegin()
{
	return HalfEdgeIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::HalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::haflEdgeBegin(HE* he)
{
	assert(he->face() == this);
	return HalfEdgeIterator(he);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::halfEdgeBegin() const
{
	return ConstHalfEdgeIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::halfEdgeBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstHalfEdgeIterator(he);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::HalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::halfEdgeEnd()
{
	return HalfEdgeIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::halfEdgeEnd() const
{
	return ConstHalfEdgeIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::HalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h>::halfEdges()
{
	return HalfEdgeRangeIterator(
		*this, &FaceHalfEdgeReference::halfEdgeBegin, &FaceHalfEdgeReference::halfEdgeEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstHalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h>::halfEdges() const
{
	return ConstHalfEdgeRangeIterator(
		*this, &FaceHalfEdgeReference::halfEdgeBegin, &FaceHalfEdgeReference::halfEdgeEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::InnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::innerHalfEdgeBegin()
{
	return ihe().begin();
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstInnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::innerHalfEdgeBegin() const
{
	return ihe().begin();
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::InnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::innerHalfEdgeEnd()
{
	return ihe().end();
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstInnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h>::innerHalfEdgeEnd() const
{
	return ihe().end();
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::InnerHalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h>::innerHalfEdges()
{
	return InnerHalfEdgeRangeIterator(
		*this,
		&FaceHalfEdgeReference::innerHalfEdgeBegin,
		&FaceHalfEdgeReference::innerHalfEdgeEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstInnerHalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h>::innerHalfEdges() const
{
	return ConstInnerHalfEdgeRangeIterator(
		*this,
		&FaceHalfEdgeReference::innerHalfEdgeBegin,
		&FaceHalfEdgeReference::innerHalfEdgeEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::VertexIterator
FaceHalfEdgeReference<HE, El, h>::vertexBegin()
{
	return VertexIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::VertexIterator
FaceHalfEdgeReference<HE, El, h>::vertexBegin(HE* he)
{
	assert(he->face() == this);
	return VertexIterator(he);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h>::vertexBegin() const
{
	return ConstVertexIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h>::vertexBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstVertexIterator(he);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::VertexIterator
FaceHalfEdgeReference<HE, El, h>::vertexEnd()
{
	return VertexIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h>::vertexEnd() const
{
	return ConstVertexIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::VertexRangeIterator
FaceHalfEdgeReference<HE, El, h>::vertices()
{
	return VertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstVertexRangeIterator
FaceHalfEdgeReference<HE, El, h>::vertices() const
{
	return ConstVertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::WedgeColorsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeColorBegin() requires HasColor<HE>
{
	return WedgeColorsIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::WedgeColorsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeColorEnd() requires HasColor<HE>
{
	return WedgeColorsIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstWedgeColorsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeColorBegin() const requires HasColor<HE>
{
	return ConstWedgeColorsIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstWedgeColorsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeColorEnd() const requires HasColor<HE>
{
	return ConstWedgeColorsIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::WedgeColorsRangeIterator
FaceHalfEdgeReference<HE, El, h>::wedgeColors() requires HasColor<HE>
{
	return WedgeColorsRangeIterator(
		*this, &FaceHalfEdgeReference::wedgeColorBegin, &FaceHalfEdgeReference::wedgeColorEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstWedgeColorsRangeIterator
FaceHalfEdgeReference<HE, El, h>::wedgeColors() const requires HasColor<HE>
{
	return ConstWedgeColorsRangeIterator(
		*this, &FaceHalfEdgeReference::wedgeColorBegin, &FaceHalfEdgeReference::wedgeColorEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::WedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeTexCoordBegin() requires HasTexCoord<HE>
{
	return WedgeTexCoordsIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::WedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeTexCoordEnd() requires HasTexCoord<HE>
{
	return WedgeTexCoordsIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstWedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeTexCoordBegin() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsIterator(ohe());
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstWedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h>::wedgeTexCoordEnd() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsIterator(nullptr);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::WedgeTexCoordsRangeIterator
FaceHalfEdgeReference<HE, El, h>::wedgeTexCoords() requires HasTexCoord<HE>
{
	return WedgeTexCoordsRangeIterator(
		*this,
		&FaceHalfEdgeReference::wedgeTexCoordBegin,
		&FaceHalfEdgeReference::wedgeTexCoordEnd);
}

template<typename HE, typename El, bool h>
typename FaceHalfEdgeReference<HE, El, h>::ConstWedgeTexCoordsRangeIterator
FaceHalfEdgeReference<HE, El, h>::wedgeTexCoords() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsRangeIterator(
		*this,
		&FaceHalfEdgeReference::wedgeTexCoordBegin,
		&FaceHalfEdgeReference::wedgeTexCoordEnd);
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::updateHalfEdgeReferences(
	const HE* oldBase,
	const HE* newBase)
{
	if (ohe() != nullptr) {
		size_t diff = ohe() - oldBase;
		ohe() = (HE*)newBase + diff;
	}
	for (uint i = 0; i < ihe().size(); ++i) {
		if (ihe()[i] != nullptr) {
			size_t diff = ihe()[i] - oldBase;
			ihe()[i] = (HE*)newBase + diff;
		}
	}
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::updateHalfEdgeReferencesAfterCompact(
	const HE*         base,
	const std::vector<int>& newIndices)
{
	if (ohe() != nullptr) {
		size_t diff = ohe() - base;
		if (newIndices[diff] < 0)
			ohe() = nullptr;
		else
			ohe() = (HE*)base + newIndices[diff];
	}
	for (uint i = 0; i < ihe().size(); ++i) {
		if (ihe()[i] != nullptr) {
			size_t diff = ihe()[i] - base;
			if (newIndices[diff] < 0)
				ihe()[i] = nullptr;
			else
				ihe()[i] = (HE*)base + newIndices[diff];
		}
	}
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::updateVertexReferences(const Vertex *, const Vertex *)
{
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::updateVertexReferencesAfterCompact(
	const Vertex*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::updateFaceReferences(const Face*, const Face*)
{
}

template<typename HE, typename El, bool h>
void FaceHalfEdgeReference<HE, El, h>::updateFaceReferencesAfterCompact(
	const Face*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool h>
template<typename Element>
void FaceHalfEdgeReference<HE, El, h>::importFrom(const Element &)
{
}

template<typename HE, typename El, bool h>
template<typename OtherFace, typename OtherHEdge>
void FaceHalfEdgeReference<HE, El, h>::importHalfEdgeReferencesFrom(
	const OtherFace&  e,
	HE*         base,
	const OtherHEdge* ebase)
{
	if constexpr (HasFaceHalfEdgeReference<OtherFace>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.outerHalfEdge() != nullptr) {
				ohe() = (HE*)base + (e.outerHalfEdge() - ebase);
			}
			ihe().resize(e.numberHoles());
			for (uint i = 0; i < ihe().size(); ++i) {
				if (e.innerHalfEdge(i) != nullptr) {
					ihe()[i] = (HE*)base + (e.innerHalfEdge(i) - ebase);
				}
			}
		}
	}
}

template<typename HE, typename El, bool h>
HE*& FaceHalfEdgeReference<HE, El, h>::ohe()
{
	return data.template get<El>(this).ohe;
}

template<typename HE, typename El, bool h>
const HE* FaceHalfEdgeReference<HE, El, h>::ohe() const
{
	return data.template get<El>(this).ohe;
}

template<typename HE, typename El, bool h>
std::vector<HE*>& FaceHalfEdgeReference<HE, El, h>::ihe()
{
	return data.template get<El>(this).ihe;
}

template<typename HE, typename El, bool h>
const std::vector<HE*>& FaceHalfEdgeReference<HE, El, h>::ihe() const
{
	return data.template get<El>(this).ihe;
}

template<typename HE, typename El, bool h>
short& FaceHalfEdgeReference<HE, El, h>::texIndex()
{
	return data.template get<El>(this).texIndex;
}

template<typename HE, typename El, bool h>
short FaceHalfEdgeReference<HE, El, h>::texIndex() const
{
	return data.template get<El>(this).texIndex;
}

} // namespace vcl::comp
