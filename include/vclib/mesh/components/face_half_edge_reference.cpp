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

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::init()
{
	ohe() = nullptr;
	texIndex() = 0;
}

template<typename HE, typename El, bool h, bool o>
const HE* FaceHalfEdgeReference<HE, El, h, o>::outerHalfEdge() const
{
	return ohe();
}

template<typename HE, typename El, bool h, bool o>
HE*& FaceHalfEdgeReference<HE, El, h, o>::outerHalfEdge()
{
	return ohe();
}

template<typename HE, typename El, bool h, bool o>
uint FaceHalfEdgeReference<HE, El, h, o>::numberHoles() const
{
	return ihe().size();
}

template<typename HE, typename El, bool h, bool o>
const HE* FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdge(uint i) const
{
	return ihe()[i];
}

template<typename HE, typename El, bool h, bool o>
HE*& FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdge(uint i)
{
	return ihe()[i];
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::resizeInnerHalfEdges(uint n)
{
	ihe().resize(n);
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::pushInnerHalfEdge(HE* he)
{
	ihe().push_back(he);
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::insertInnerHalfEdge(uint i, HE* he)
{
	assert(i < ihe().size());
	ihe().insert(ihe().begin() + i, he);
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::eraseInnerHalfEdge(uint i)
{
	assert(i < ihe().size());
	ihe().erase(ihe().begin() + i);
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::clearInnerHalfEdges()
{
	ihe().clear();
}

template<typename HE, typename El, bool h, bool o>
uint FaceHalfEdgeReference<HE, El, h, o>::vertexNumber() const
{
	uint cnt = 0;
	for (const Vertex* v : vertices())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::Vertex*& FaceHalfEdgeReference<HE, El, h, o>::vertex(uint i)
{
	uint           j  = 0;
	VertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
const typename FaceHalfEdgeReference<HE, El, h, o>::Vertex*
FaceHalfEdgeReference<HE, El, h, o>::vertex(uint i) const
{
	uint                j  = 0;
	ConstVertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::Vertex*& FaceHalfEdgeReference<HE, El, h, o>::vertexMod(int i)
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

template<typename HE, typename El, bool h, bool o>
const typename FaceHalfEdgeReference<HE, El, h, o>::Vertex*
FaceHalfEdgeReference<HE, El, h, o>::vertexMod(int i) const
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

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::setVertex(Vertex* v, uint i)
{
	vertex(i) = v;
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::setVertices(const std::vector<Vertex*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Vertex*& v : vertices()) {
		v = list[i++];
	}
}

template<typename HE, typename El, bool h, bool o>
bool FaceHalfEdgeReference<HE, El, h, o>::containsVertex(const Vertex* v) const
{
	return findVertex(v) != vertexEnd();
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::VertexIterator
FaceHalfEdgeReference<HE, El, h, o>::findVertex(const Vertex* v)
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h, o>::findVertex(const Vertex* v) const
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HE, typename El, bool h, bool o>
int FaceHalfEdgeReference<HE, El, h, o>::indexOfVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : vertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool h, bool o>
int FaceHalfEdgeReference<HE, El, h, o>::indexOfEdge(const Vertex* v1, const Vertex* v2) const
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

template<typename HE, typename El, bool h, bool o>
uint FaceHalfEdgeReference<HE, El, h, o>::adjFacesNumber() const
{
	return vertexNumber();
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::Face*& FaceHalfEdgeReference<HE, El, h, o>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
const typename FaceHalfEdgeReference<HE, El, h, o>::Face*
FaceHalfEdgeReference<HE, El, h, o>::adjFace(uint i) const
{
	uint                      j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::Face*& FaceHalfEdgeReference<HE, El, h, o>::adjFaceMod(int i)
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

template<typename HE, typename El, bool h, bool o>
const typename FaceHalfEdgeReference<HE, El, h, o>::Face*
FaceHalfEdgeReference<HE, El, h, o>::adjFaceMod(int i) const
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

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::setAdjFaces(const std::vector<Face*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HE, typename El, bool h, bool o>
bool FaceHalfEdgeReference<HE, El, h, o>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool h, bool o>
int FaceHalfEdgeReference<HE, El, h, o>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool h, bool o>
vcl::Color &FaceHalfEdgeReference<HE, El, h, o>::wedgeColor(uint i) requires HasColor<HE>
{
	uint                j  = 0;
	WedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
const vcl::Color&
FaceHalfEdgeReference<HE, El, h, o>::wedgeColor(uint i) const requires HasColor<HE>
{
	uint                     j  = 0;
	ConstWedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
vcl::Color& FaceHalfEdgeReference<HE, El, h, o>::wedgeColorMod(int i) requires HasColor<HE>
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

template<typename HE, typename El, bool h, bool o>
const vcl::Color&
FaceHalfEdgeReference<HE, El, h, o>::wedgeColorMod(int i) const requires HasColor<HE>
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

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::setWedgeColor(const vcl::Color& t, uint i) requires
	HasColor<HE>
{
	wedgeColor(i) = t;
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::setWedgeColors(
	const std::vector<vcl::Color>& list) requires HasColor<HE>
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (vcl::Color& c : wedgeColors()) {
		c = list[i++];
	}
}

template<typename HE, typename El, bool h, bool o>
bool FaceHalfEdgeReference<HE, El, h, o>::isWedgeColorsEnabled() const requires HasColor<HE>
{
	if constexpr (HasOptionalColor<HE>) {
		return ohe()->isColorEnabled();
	}
	else {
		return true;
	}
}

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoord(uint i)
{
	uint                   j  = 0;
	WedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
const typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoord(uint i) const
{
	uint                        j  = 0;
	ConstWedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoordMod(int i)
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

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
const typename OHE::TexCoordType& FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoordMod(int i) const
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

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
void FaceHalfEdgeReference<HE, El, h, o>::setWedgeTexCoord(const typename OHE::TexCoordType& t, uint i)
{
	wedgeTexCoord(i) = t;
}

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
void FaceHalfEdgeReference<HE, El, h, o>::setWedgeTexCoords(
	const std::vector<typename OHE::TexCoordType>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (typename OHE::TexCoordType& c : wedgeTexCoords()) {
		c = list[i++];
	}
}

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
short& FaceHalfEdgeReference<HE, El, h, o>::textureIndex()
{
	return texIndex();
}

template<typename HE, typename El, bool h, bool o>
template<HasTexCoord OHE>
short FaceHalfEdgeReference<HE, El, h, o>::textureIndex() const
{
	return texIndex();
}

template<typename HE, typename El, bool h, bool o>
bool FaceHalfEdgeReference<HE, El, h, o>::isWedgeTexCoordsEnabled() const requires HasTexCoord<HE>
{
	if constexpr (HasOptionalTexCoord<HE>) {
		return ohe()->isTexCoordEnabled();
	}
	else {
		return true;
	}
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaceBegin()
{
	return AdjacentFaceIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaceBegin(HE* he)
{
	assert(he->face() == this);
	return AdjacentFaceIterator(he);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::AdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaceBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstAdjacentFaceIterator(he);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::AdjacentFaceRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaces()
{
	return AdjacentFaceRangeIterator(
		*this, &FaceHalfEdgeReference::adjFaceBegin, &FaceHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::adjFaces() const
{
	return ConstAdjacentFaceRangeIterator(
		*this, &FaceHalfEdgeReference::adjFaceBegin, &FaceHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::HalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::haflEdgeBegin()
{
	return HalfEdgeIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::HalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::haflEdgeBegin(HE* he)
{
	assert(he->face() == this);
	return HalfEdgeIterator(he);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::halfEdgeBegin() const
{
	return ConstHalfEdgeIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::halfEdgeBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstHalfEdgeIterator(he);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::HalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::halfEdgeEnd()
{
	return HalfEdgeIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::halfEdgeEnd() const
{
	return ConstHalfEdgeIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::HalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::halfEdges()
{
	return HalfEdgeRangeIterator(
		*this, &FaceHalfEdgeReference::halfEdgeBegin, &FaceHalfEdgeReference::halfEdgeEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstHalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::halfEdges() const
{
	return ConstHalfEdgeRangeIterator(
		*this, &FaceHalfEdgeReference::halfEdgeBegin, &FaceHalfEdgeReference::halfEdgeEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::InnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdgeBegin()
{
	return ihe().begin();
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstInnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdgeBegin() const
{
	return ihe().begin();
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::InnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdgeEnd()
{
	return ihe().end();
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstInnerHalfEdgeIterator
FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdgeEnd() const
{
	return ihe().end();
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::InnerHalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdges()
{
	return InnerHalfEdgeRangeIterator(
		*this,
		&FaceHalfEdgeReference::innerHalfEdgeBegin,
		&FaceHalfEdgeReference::innerHalfEdgeEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstInnerHalfEdgeRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::innerHalfEdges() const
{
	return ConstInnerHalfEdgeRangeIterator(
		*this,
		&FaceHalfEdgeReference::innerHalfEdgeBegin,
		&FaceHalfEdgeReference::innerHalfEdgeEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::VertexIterator
FaceHalfEdgeReference<HE, El, h, o>::vertexBegin()
{
	return VertexIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::VertexIterator
FaceHalfEdgeReference<HE, El, h, o>::vertexBegin(HE* he)
{
	assert(he->face() == this);
	return VertexIterator(he);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h, o>::vertexBegin() const
{
	return ConstVertexIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h, o>::vertexBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstVertexIterator(he);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::VertexIterator
FaceHalfEdgeReference<HE, El, h, o>::vertexEnd()
{
	return VertexIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstVertexIterator
FaceHalfEdgeReference<HE, El, h, o>::vertexEnd() const
{
	return ConstVertexIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::VertexRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::vertices()
{
	return VertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstVertexRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::vertices() const
{
	return ConstVertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::WedgeColorsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeColorBegin() requires HasColor<HE>
{
	return WedgeColorsIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::WedgeColorsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeColorEnd() requires HasColor<HE>
{
	return WedgeColorsIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstWedgeColorsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeColorBegin() const requires HasColor<HE>
{
	return ConstWedgeColorsIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstWedgeColorsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeColorEnd() const requires HasColor<HE>
{
	return ConstWedgeColorsIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::WedgeColorsRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeColors() requires HasColor<HE>
{
	return WedgeColorsRangeIterator(
		*this, &FaceHalfEdgeReference::wedgeColorBegin, &FaceHalfEdgeReference::wedgeColorEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstWedgeColorsRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeColors() const requires HasColor<HE>
{
	return ConstWedgeColorsRangeIterator(
		*this, &FaceHalfEdgeReference::wedgeColorBegin, &FaceHalfEdgeReference::wedgeColorEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::WedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoordBegin() requires HasTexCoord<HE>
{
	return WedgeTexCoordsIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::WedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoordEnd() requires HasTexCoord<HE>
{
	return WedgeTexCoordsIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstWedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoordBegin() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsIterator(ohe());
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstWedgeTexCoordsIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoordEnd() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::WedgeTexCoordsRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoords() requires HasTexCoord<HE>
{
	return WedgeTexCoordsRangeIterator(
		*this,
		&FaceHalfEdgeReference::wedgeTexCoordBegin,
		&FaceHalfEdgeReference::wedgeTexCoordEnd);
}

template<typename HE, typename El, bool h, bool o>
typename FaceHalfEdgeReference<HE, El, h, o>::ConstWedgeTexCoordsRangeIterator
FaceHalfEdgeReference<HE, El, h, o>::wedgeTexCoords() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsRangeIterator(
		*this,
		&FaceHalfEdgeReference::wedgeTexCoordBegin,
		&FaceHalfEdgeReference::wedgeTexCoordEnd);
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::updateHalfEdgeReferences(
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

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::updateHalfEdgeReferencesAfterCompact(
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

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::updateVertexReferences(const Vertex *, const Vertex *)
{
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::updateVertexReferencesAfterCompact(
	const Vertex*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::updateFaceReferences(const Face*, const Face*)
{
}

template<typename HE, typename El, bool h, bool o>
void FaceHalfEdgeReference<HE, El, h, o>::updateFaceReferencesAfterCompact(
	const Face*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool h, bool o>
template<typename Element>
void FaceHalfEdgeReference<HE, El, h, o>::importFrom(const Element &)
{
}

template<typename HE, typename El, bool h, bool o>
template<typename OtherFace, typename OtherHEdge>
void FaceHalfEdgeReference<HE, El, h, o>::importHalfEdgeReferencesFrom(
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

template<typename HE, typename El, bool h, bool o>
HE*& FaceHalfEdgeReference<HE, El, h, o>::ohe()
{
	return data.template get<El>(this).ohe;
}

template<typename HE, typename El, bool h, bool o>
const HE* FaceHalfEdgeReference<HE, El, h, o>::ohe() const
{
	return data.template get<El>(this).ohe;
}

template<typename HE, typename El, bool h, bool o>
std::vector<HE*>& FaceHalfEdgeReference<HE, El, h, o>::ihe()
{
	return data.template get<El>(this).ihe;
}

template<typename HE, typename El, bool h, bool o>
const std::vector<HE*>& FaceHalfEdgeReference<HE, El, h, o>::ihe() const
{
	return data.template get<El>(this).ihe;
}

template<typename HE, typename El, bool h, bool o>
short& FaceHalfEdgeReference<HE, El, h, o>::texIndex()
{
	return data.template get<El>(this).texIndex;
}

template<typename HE, typename El, bool h, bool o>
short FaceHalfEdgeReference<HE, El, h, o>::texIndex() const
{
	return data.template get<El>(this).texIndex;
}

} // namespace vcl::comp
