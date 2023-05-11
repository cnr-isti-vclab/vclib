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

#include "face_half_edge_pointers.h"

namespace vcl::comp {

template<typename HalfEdge, typename El, bool O>
FaceHalfEdgePointers<HalfEdge, El, O>::FaceHalfEdgePointers()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::init()
{
	ohe() = nullptr;
	texIndex() = 0;
}

template<typename HalfEdge, typename El, bool O>
bool FaceHalfEdgePointers<HalfEdge, El, O>::isEnabled()
{
	return Base::isEnabled(this);
}

template<typename HE, typename El, bool O>
const HE* FaceHalfEdgePointers<HE, El, O>::outerHalfEdge() const
{
	return ohe();
}

template<typename HE, typename El, bool O>
HE*& FaceHalfEdgePointers<HE, El, O>::outerHalfEdge()
{
	return ohe();
}

template<typename HE, typename El, bool O>
uint FaceHalfEdgePointers<HE, El, O>::numberHoles() const
{
	return ihe().size();
}

template<typename HE, typename El, bool O>
const HE* FaceHalfEdgePointers<HE, El, O>::innerHalfEdge(uint i) const
{
	return ihe()[i];
}

template<typename HE, typename El, bool O>
HE*& FaceHalfEdgePointers<HE, El, O>::innerHalfEdge(uint i)
{
	return ihe()[i];
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::resizeInnerHalfEdges(uint n)
{
	ihe().resize(n);
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::pushInnerHalfEdge(HE* he)
{
	ihe().push_back(he);
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::insertInnerHalfEdge(uint i, HE* he)
{
	assert(i < ihe().size());
	ihe().insert(ihe().begin() + i, he);
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::eraseInnerHalfEdge(uint i)
{
	assert(i < ihe().size());
	ihe().erase(ihe().begin() + i);
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::clearInnerHalfEdges()
{
	ihe().clear();
}

template<typename HE, typename El, bool O>
uint FaceHalfEdgePointers<HE, El, O>::vertexNumber() const
{
	uint cnt = 0;
	for (const Vertex* v : vertices())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::Vertex*& FaceHalfEdgePointers<HE, El, O>::vertex(uint i)
{
	uint           j  = 0;
	VertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
const typename FaceHalfEdgePointers<HE, El, O>::Vertex*
FaceHalfEdgePointers<HE, El, O>::vertex(uint i) const
{
	uint                j  = 0;
	ConstVertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::Vertex*& FaceHalfEdgePointers<HE, El, O>::vertexMod(int i)
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

template<typename HE, typename El, bool O>
const typename FaceHalfEdgePointers<HE, El, O>::Vertex*
FaceHalfEdgePointers<HE, El, O>::vertexMod(int i) const
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

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::setVertex(Vertex* v, uint i)
{
	vertex(i) = v;
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::setVertices(const std::vector<Vertex*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Vertex*& v : vertices()) {
		v = list[i++];
	}
}

template<typename HE, typename El, bool O>
bool FaceHalfEdgePointers<HE, El, O>::containsVertex(const Vertex* v) const
{
	return findVertex(v) != vertexEnd();
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::VertexIterator
FaceHalfEdgePointers<HE, El, O>::findVertex(const Vertex* v)
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstVertexIterator
FaceHalfEdgePointers<HE, El, O>::findVertex(const Vertex* v) const
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HE, typename El, bool O>
int FaceHalfEdgePointers<HE, El, O>::indexOfVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : vertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool O>
int FaceHalfEdgePointers<HE, El, O>::indexOfEdge(const Vertex* v1, const Vertex* v2) const
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

template<typename HE, typename El, bool O>
uint FaceHalfEdgePointers<HE, El, O>::adjFacesNumber() const
{
	return vertexNumber();
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::Face*& FaceHalfEdgePointers<HE, El, O>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
const typename FaceHalfEdgePointers<HE, El, O>::Face*
FaceHalfEdgePointers<HE, El, O>::adjFace(uint i) const
{
	uint                      j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::Face*& FaceHalfEdgePointers<HE, El, O>::adjFaceMod(int i)
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

template<typename HE, typename El, bool O>
const typename FaceHalfEdgePointers<HE, El, O>::Face*
FaceHalfEdgePointers<HE, El, O>::adjFaceMod(int i) const
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

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::setAdjFaces(const std::vector<Face*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HE, typename El, bool O>
bool FaceHalfEdgePointers<HE, El, O>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::AdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstAdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool O>
int FaceHalfEdgePointers<HE, El, O>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool O>
vcl::Color &FaceHalfEdgePointers<HE, El, O>::wedgeColor(uint i) requires HasColor<HE>
{
	uint                j  = 0;
	WedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
const vcl::Color&
FaceHalfEdgePointers<HE, El, O>::wedgeColor(uint i) const requires HasColor<HE>
{
	uint                     j  = 0;
	ConstWedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
vcl::Color& FaceHalfEdgePointers<HE, El, O>::wedgeColorMod(int i) requires HasColor<HE>
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

template<typename HE, typename El, bool O>
const vcl::Color&
FaceHalfEdgePointers<HE, El, O>::wedgeColorMod(int i) const requires HasColor<HE>
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

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::setWedgeColor(const vcl::Color& t, uint i) requires
	HasColor<HE>
{
	wedgeColor(i) = t;
}

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::setWedgeColors(
	const std::vector<vcl::Color>& list) requires HasColor<HE>
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (vcl::Color& c : wedgeColors()) {
		c = list[i++];
	}
}

template<typename HE, typename El, bool O>
bool FaceHalfEdgePointers<HE, El, O>::isWedgeColorsEnabled() const requires HasColor<HE>
{
	if constexpr (HasOptionalColor<HE>) {
		return ohe()->isColorEnabled();
	}
	else {
		return true;
	}
}

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
typename OHE::TexCoordType& FaceHalfEdgePointers<HE, El, O>::wedgeTexCoord(uint i)
{
	uint                   j  = 0;
	WedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
const typename OHE::TexCoordType& FaceHalfEdgePointers<HE, El, O>::wedgeTexCoord(uint i) const
{
	uint                        j  = 0;
	ConstWedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
typename OHE::TexCoordType& FaceHalfEdgePointers<HE, El, O>::wedgeTexCoordMod(int i)
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

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
const typename OHE::TexCoordType& FaceHalfEdgePointers<HE, El, O>::wedgeTexCoordMod(int i) const
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

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
void FaceHalfEdgePointers<HE, El, O>::setWedgeTexCoord(const typename OHE::TexCoordType& t, uint i)
{
	wedgeTexCoord(i) = t;
}

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
void FaceHalfEdgePointers<HE, El, O>::setWedgeTexCoords(
	const std::vector<typename OHE::TexCoordType>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (typename OHE::TexCoordType& c : wedgeTexCoords()) {
		c = list[i++];
	}
}

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
short& FaceHalfEdgePointers<HE, El, O>::textureIndex()
{
	return texIndex();
}

template<typename HE, typename El, bool O>
template<HasTexCoord OHE>
short FaceHalfEdgePointers<HE, El, O>::textureIndex() const
{
	return texIndex();
}

template<typename HE, typename El, bool O>
bool FaceHalfEdgePointers<HE, El, O>::isWedgeTexCoordsEnabled() const requires HasTexCoord<HE>
{
	if constexpr (HasOptionalTexCoord<HE>) {
		return ohe()->isTexCoordEnabled();
	}
	else {
		return true;
	}
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::AdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::adjFaceBegin()
{
	return AdjacentFaceIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::AdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::adjFaceBegin(HE* he)
{
	assert(he->face() == this);
	return AdjacentFaceIterator(he);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::AdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstAdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstAdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::adjFaceBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstAdjacentFaceIterator(he);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstAdjacentFaceIterator
FaceHalfEdgePointers<HE, El, O>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::adjFaces()
{
	return View(adjFaceBegin(), adjFaceEnd());
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::adjFaces() const
{
	return View(adjFaceBegin(), adjFaceEnd());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::HalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::haflEdgeBegin()
{
	return HalfEdgeIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::HalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::haflEdgeBegin(HE* he)
{
	assert(he->face() == this);
	return HalfEdgeIterator(he);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstHalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::halfEdgeBegin() const
{
	return ConstHalfEdgeIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstHalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::halfEdgeBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstHalfEdgeIterator(he);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::HalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::halfEdgeEnd()
{
	return HalfEdgeIterator(nullptr);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstHalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::halfEdgeEnd() const
{
	return ConstHalfEdgeIterator(nullptr);
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::halfEdges()
{
	return View(halfEdgeBegin(), halfEdgeEnd());
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::halfEdges() const
{
	return View(halfEdgeBegin(), halfEdgeEnd());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::InnerHalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::innerHalfEdgeBegin()
{
	return ihe().begin();
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstInnerHalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::innerHalfEdgeBegin() const
{
	return ihe().begin();
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::InnerHalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::innerHalfEdgeEnd()
{
	return ihe().end();
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstInnerHalfEdgeIterator
FaceHalfEdgePointers<HE, El, O>::innerHalfEdgeEnd() const
{
	return ihe().end();
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::innerHalfEdges()
{
	return View(innerHalfEdgeBegin(), innerHalfEdgeEnd());
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::innerHalfEdges() const
{
	return View(innerHalfEdgeBegin(), innerHalfEdgeEnd());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::VertexIterator
FaceHalfEdgePointers<HE, El, O>::vertexBegin()
{
	return VertexIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::VertexIterator
FaceHalfEdgePointers<HE, El, O>::vertexBegin(HE* he)
{
	assert(he->face() == this);
	return VertexIterator(he);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstVertexIterator
FaceHalfEdgePointers<HE, El, O>::vertexBegin() const
{
	return ConstVertexIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstVertexIterator
FaceHalfEdgePointers<HE, El, O>::vertexBegin(const HE* he) const
{
	assert(he->face() == this);
	return ConstVertexIterator(he);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::VertexIterator
FaceHalfEdgePointers<HE, El, O>::vertexEnd()
{
	return VertexIterator(nullptr);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstVertexIterator
FaceHalfEdgePointers<HE, El, O>::vertexEnd() const
{
	return ConstVertexIterator(nullptr);
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::vertices()
{
	return View(vertexBegin(), vertexEnd());
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::vertices() const
{
	return View(vertexBegin(), vertexEnd());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::WedgeColorsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeColorBegin() requires HasColor<HE>
{
	return WedgeColorsIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::WedgeColorsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeColorEnd() requires HasColor<HE>
{
	return WedgeColorsIterator(nullptr);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstWedgeColorsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeColorBegin() const requires HasColor<HE>
{
	return ConstWedgeColorsIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstWedgeColorsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeColorEnd() const requires HasColor<HE>
{
	return ConstWedgeColorsIterator(nullptr);
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::wedgeColors() requires HasColor<HE>
{
	return View(wedgeColorBegin(), wedgeColorEnd());
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::wedgeColors() const requires HasColor<HE>
{
	return View(wedgeColorBegin(), wedgeColorEnd());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::WedgeTexCoordsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeTexCoordBegin() requires HasTexCoord<HE>
{
	return WedgeTexCoordsIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::WedgeTexCoordsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeTexCoordEnd() requires HasTexCoord<HE>
{
	return WedgeTexCoordsIterator(nullptr);
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstWedgeTexCoordsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeTexCoordBegin() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsIterator(ohe());
}

template<typename HE, typename El, bool O>
typename FaceHalfEdgePointers<HE, El, O>::ConstWedgeTexCoordsIterator
FaceHalfEdgePointers<HE, El, O>::wedgeTexCoordEnd() const requires HasTexCoord<HE>
{
	return ConstWedgeTexCoordsIterator(nullptr);
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::wedgeTexCoords() requires HasTexCoord<HE>
{
	return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
}

template<typename HE, typename El, bool O>
auto FaceHalfEdgePointers<HE, El, O>::wedgeTexCoords() const requires HasTexCoord<HE>
{
	return View(wedgeTexCoordBegin(), wedgeTexCoordEnd());
}

template<typename HE, typename El, bool O>
template<typename Element>
void FaceHalfEdgePointers<HE, El, O>::importFrom(const Element &)
{
}

template<typename HE, typename El, bool O>
template<typename OtherFace, typename OtherHEdge>
void FaceHalfEdgePointers<HE, El, O>::importPointersFrom(
	const OtherFace&  e,
	HE*         base,
	const OtherHEdge* ebase)
{
	if constexpr (HasFaceHalfEdgePointers<OtherFace>) {
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

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::updatePointers(
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

template<typename HE, typename El, bool O>
void FaceHalfEdgePointers<HE, El, O>::updatePointersAfterCompact(
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

template<typename HE, typename El, bool O>
HE*& FaceHalfEdgePointers<HE, El, O>::ohe()
{
	return Base::data(this).ohe;
}

template<typename HE, typename El, bool O>
const HE* FaceHalfEdgePointers<HE, El, O>::ohe() const
{
	return Base::data(this).ohe;
}

template<typename HE, typename El, bool O>
std::vector<HE*>& FaceHalfEdgePointers<HE, El, O>::ihe()
{
	return Base::data(this).ihe;
}

template<typename HE, typename El, bool O>
const std::vector<HE*>& FaceHalfEdgePointers<HE, El, O>::ihe() const
{
	return Base::data(this).ihe;
}

template<typename HE, typename El, bool O>
short& FaceHalfEdgePointers<HE, El, O>::texIndex()
{
	return Base::data(this).texIndex;
}

template<typename HE, typename El, bool O>
short FaceHalfEdgePointers<HE, El, O>::texIndex() const
{
	return Base::data(this).texIndex;
}

} // namespace vcl::comp
