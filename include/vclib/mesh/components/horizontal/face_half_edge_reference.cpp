/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

template<typename HalfEdge>
FaceHalfEdgeReference<HalfEdge>::FaceHalfEdgeReference()
{
}

template<typename HalfEdge>
const HalfEdge* FaceHalfEdgeReference<HalfEdge>::outerHalfEdge() const
{
	return ohe;
}

template<typename HalfEdge>
HalfEdge*& FaceHalfEdgeReference<HalfEdge>::outerHalfEdge()
{
	return ohe;
}

template<typename HalfEdge>
uint FaceHalfEdgeReference<HalfEdge>::numberHoles() const
{
	return ihe.size();
}

template<typename HalfEdge>
const HalfEdge* FaceHalfEdgeReference<HalfEdge>::innerHalfEdge(uint i) const
{
	return ihe[i];
}

template<typename HalfEdge>
HalfEdge*& FaceHalfEdgeReference<HalfEdge>::innerHalfEdge(uint i)
{
	return ihe[i];
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::resizeInnerHalfEdges(uint n)
{
	ihe.resize(n);
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::pushInnerHalfEdge(HalfEdge* he)
{
	ihe.push_back(he);
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::insertInnerHalfEdge(uint i, HalfEdge* he)
{
	assert(i < ihe.size());
	ihe.insert(ihe.begin() + i, he);
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::eraseInnerHalfEdge(uint i)
{
	assert(i < ihe.size());
	ihe.erase(ihe.begin() + i);
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::clearInnerHalfEdges()
{
	ihe.clear();
}

template<typename HalfEdge>
uint FaceHalfEdgeReference<HalfEdge>::vertexNumber() const
{
	uint cnt = 0;
	for (const Vertex* v : vertices())
		++cnt;
	return cnt;
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::Vertex*& FaceHalfEdgeReference<HalfEdge>::vertex(uint i)
{
	uint           j  = 0;
	VertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename FaceHalfEdgeReference<HalfEdge>::Vertex*
FaceHalfEdgeReference<HalfEdge>::vertex(uint i) const
{
	uint                j  = 0;
	ConstVertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::Vertex*& FaceHalfEdgeReference<HalfEdge>::vertexMod(int i)
{
	uint           j  = 0;
	VertexIterator it = VertexIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename FaceHalfEdgeReference<HalfEdge>::Vertex*
FaceHalfEdgeReference<HalfEdge>::vertexMod(int i) const
{
	uint                j = 0;
	ConstVertexIterator it =
		ConstVertexIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setVertex(Vertex* v, uint i)
{
	vertex(i) = v;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setVertices(const std::vector<Vertex*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Vertex*& v : vertices()) {
		v = list[i++];
	}
}

template<typename HalfEdge>
bool FaceHalfEdgeReference<HalfEdge>::containsVertex(const Vertex* v) const
{
	return findVertex(v) != vertexEnd();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexIterator
FaceHalfEdgeReference<HalfEdge>::findVertex(const Vertex* v)
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexIterator
FaceHalfEdgeReference<HalfEdge>::findVertex(const Vertex* v) const
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HalfEdge>
int FaceHalfEdgeReference<HalfEdge>::indexOfVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : vertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HalfEdge>
int FaceHalfEdgeReference<HalfEdge>::indexOfEdge(const Vertex* v1, const Vertex* v2) const
{
	int vid = 0;
	for (const HalfEdge* he : halfEdges()) {
		if (he->fromVertex() == v1 && he->toVertex() == v2)
			return vid;
		if (he->toVertex() == v1 && he->fromVertex() == v2)
			return vid;
		vid++;
	}
	return -1;
}

template<typename HalfEdge>
uint FaceHalfEdgeReference<HalfEdge>::adjFacesNumber() const
{
	return vertexNumber();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::Face*& FaceHalfEdgeReference<HalfEdge>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename FaceHalfEdgeReference<HalfEdge>::Face*
FaceHalfEdgeReference<HalfEdge>::adjFace(uint i) const
{
	uint                      j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::Face*& FaceHalfEdgeReference<HalfEdge>::adjFaceMod(int i)
{
	uint                 j = 0;
	AdjacentFaceIterator it =
		AdjacentFaceIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename FaceHalfEdgeReference<HalfEdge>::Face*
FaceHalfEdgeReference<HalfEdge>::adjFaceMod(int i) const
{
	uint                      j = 0;
	ConstAdjacentFaceIterator it =
		ConstAdjacentFaceIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setAdjFaces(const std::vector<Face*>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HalfEdge>
bool FaceHalfEdgeReference<HalfEdge>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::AdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HalfEdge>
int FaceHalfEdgeReference<HalfEdge>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HalfEdge>
vcl::Color &FaceHalfEdgeReference<HalfEdge>::wedgeColor(uint i) requires HasColor<HalfEdge>
{
	uint                j  = 0;
	WedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const vcl::Color&
FaceHalfEdgeReference<HalfEdge>::wedgeColor(uint i) const requires HasColor<HalfEdge>
{
	uint                     j  = 0;
	ConstWedgeColorsIterator it = wedgeColorBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
vcl::Color& FaceHalfEdgeReference<HalfEdge>::wedgeColorMod(int i) requires HasColor<HalfEdge>
{
	uint                j  = 0;
	WedgeColorsIterator it = WedgeColorsIterator(ohe, nullptr); // this iterator does not have end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const vcl::Color&
FaceHalfEdgeReference<HalfEdge>::wedgeColorMod(int i) const requires HasColor<HalfEdge>
{
	uint                j  = 0;
	ConstWedgeColorsIterator it =
		ConstWedgeColorsIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setWedgeColor(const vcl::Color& t, uint i) requires
	HasColor<HalfEdge>
{
	wedgeColor(i) = t;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setWedgeColors(
	const std::vector<vcl::Color>& list) requires HasColor<HalfEdge>
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (vcl::Color& c : wedgeColors()) {
		c = list[i++];
	}
}

template<typename HalfEdge>
bool FaceHalfEdgeReference<HalfEdge>::isWedgeColorsEnabled() const requires HasColor<HalfEdge>
{
	if constexpr (HasOptionalColor<HalfEdge>) {
		return ohe->isColorEnabled();
	}
	else {
		return true;
	}
}

template<typename HalfEdge>
template<HasTexCoord HE>
typename HE::TexCoordType& FaceHalfEdgeReference<HalfEdge>::wedgeTexCoord(uint i)
{
	uint                   j  = 0;
	WedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
template<HasTexCoord HE>
const typename HE::TexCoordType& FaceHalfEdgeReference<HalfEdge>::wedgeTexCoord(uint i) const
{
	uint                        j  = 0;
	ConstWedgeTexCoordsIterator it = wedgeTexCoordBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
template<HasTexCoord HE>
typename HE::TexCoordType& FaceHalfEdgeReference<HalfEdge>::wedgeTexCoordMod(int i)
{
	uint                   j  = 0;
	WedgeTexCoordsIterator it =
		WedgeTexCoordsIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
template<HasTexCoord HE>
const typename HE::TexCoordType& FaceHalfEdgeReference<HalfEdge>::wedgeTexCoordMod(int i) const
{
	uint                        j  = 0;
	ConstWedgeTexCoordsIterator it =
		ConstWedgeTexCoordsIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
template<HasTexCoord HE>
void FaceHalfEdgeReference<HalfEdge>::setWedgeTexCoord(const typename HE::TexCoordType& t, uint i)
{
	wedgeTexCoord(i) = t;
}

template<typename HalfEdge>
template<HasTexCoord HE>
void FaceHalfEdgeReference<HalfEdge>::setWedgeTexCoords(
	const std::vector<typename HE::TexCoordType>& list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (typename HE::TexCoordType& c : wedgeTexCoords()) {
		c = list[i++];
	}
}

template<typename HalfEdge>
template<HasTexCoord HE>
short& FaceHalfEdgeReference<HalfEdge>::textureIndex()
{
	return texIndex;
}

template<typename HalfEdge>
template<HasTexCoord HE>
const short& FaceHalfEdgeReference<HalfEdge>::textureIndex() const
{
	return texIndex;
}

template<typename HalfEdge>
bool FaceHalfEdgeReference<HalfEdge>::isWedgeTexCoordsEnabled() const requires HasTexCoord<HalfEdge>
{
	if constexpr (HasOptionalTexCoord<HalfEdge>) {
		return ohe->isTexCoordEnabled();
	}
	else {
		return true;
	}
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::AdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::adjFaceBegin()
{
	return AdjacentFaceIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::AdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::adjFaceBegin(HalfEdge* he)
{
	assert(he->face() == this);
	return AdjacentFaceIterator(he);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::AdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::adjFaceBegin(const HalfEdge* he) const
{
	assert(he->face() == this);
	return ConstAdjacentFaceIterator(he);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstAdjacentFaceIterator
FaceHalfEdgeReference<HalfEdge>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::AdjacentFaceRangeIterator
FaceHalfEdgeReference<HalfEdge>::adjFaces()
{
	return AdjacentFaceRangeIterator(
		*this, &FaceHalfEdgeReference::adjFaceBegin, &FaceHalfEdgeReference::adjFaceEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstAdjacentFaceRangeIterator
FaceHalfEdgeReference<HalfEdge>::adjFaces() const
{
	return ConstAdjacentFaceRangeIterator(
		*this, &FaceHalfEdgeReference::adjFaceBegin, &FaceHalfEdgeReference::adjFaceEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::HalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::haflEdgeBegin()
{
	return HalfEdgeIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::HalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::haflEdgeBegin(HalfEdge* he)
{
	assert(he->face() == this);
	return HalfEdgeIterator(he);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::halfEdgeBegin() const
{
	return ConstHalfEdgeIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::halfEdgeBegin(const HalfEdge* he) const
{
	assert(he->face() == this);
	return ConstHalfEdgeIterator(he);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::HalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::halfEdgeEnd()
{
	return HalfEdgeIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstHalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::halfEdgeEnd() const
{
	return ConstHalfEdgeIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::HalfEdgeRangeIterator
FaceHalfEdgeReference<HalfEdge>::halfEdges()
{
	return HalfEdgeRangeIterator(
		*this, &FaceHalfEdgeReference::halfEdgeBegin, &FaceHalfEdgeReference::halfEdgeEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstHalfEdgeRangeIterator
FaceHalfEdgeReference<HalfEdge>::halfEdges() const
{
	return ConstHalfEdgeRangeIterator(
		*this, &FaceHalfEdgeReference::halfEdgeBegin, &FaceHalfEdgeReference::halfEdgeEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::InnerHalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::innerHalfEdgeBegin()
{
	return ihe.begin();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstInnerHalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::innerHalfEdgeBegin() const
{
	return ihe.begin();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::InnerHalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::innerHalfEdgeEnd()
{
	return ihe.end();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstInnerHalfEdgeIterator
FaceHalfEdgeReference<HalfEdge>::innerHalfEdgeEnd() const
{
	return ihe.end();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::InnerHalfEdgeRangeIterator
FaceHalfEdgeReference<HalfEdge>::innerHalfEdges()
{
	return InnerHalfEdgeRangeIterator(
		*this,
		&FaceHalfEdgeReference::innerHalfEdgeBegin,
		&FaceHalfEdgeReference::innerHalfEdgeEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstInnerHalfEdgeRangeIterator
FaceHalfEdgeReference<HalfEdge>::innerHalfEdges() const
{
	return ConstInnerHalfEdgeRangeIterator(
		*this,
		&FaceHalfEdgeReference::innerHalfEdgeBegin,
		&FaceHalfEdgeReference::innerHalfEdgeEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexBegin()
{
	return VertexIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexBegin(HalfEdge* he)
{
	assert(he->face() == this);
	return VertexIterator(he);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexBegin() const
{
	return ConstVertexIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexBegin(const HalfEdge* he) const
{
	assert(he->face() == this);
	return ConstVertexIterator(he);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexEnd()
{
	return VertexIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexEnd() const
{
	return ConstVertexIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexRangeIterator
FaceHalfEdgeReference<HalfEdge>::vertices()
{
	return VertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexRangeIterator
FaceHalfEdgeReference<HalfEdge>::vertices() const
{
	return ConstVertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::WedgeColorsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeColorBegin() requires HasColor<HalfEdge>
{
	return WedgeColorsIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::WedgeColorsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeColorEnd() requires HasColor<HalfEdge>
{
	return WedgeColorsIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstWedgeColorsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeColorBegin() const requires HasColor<HalfEdge>
{
	return ConstWedgeColorsIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstWedgeColorsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeColorEnd() const requires HasColor<HalfEdge>
{
	return ConstWedgeColorsIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::WedgeColorsRangeIterator
FaceHalfEdgeReference<HalfEdge>::wedgeColors() requires HasColor<HalfEdge>
{
	return WedgeColorsRangeIterator(
		*this, &FaceHalfEdgeReference::wedgeColorBegin, &FaceHalfEdgeReference::wedgeColorEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstWedgeColorsRangeIterator
FaceHalfEdgeReference<HalfEdge>::wedgeColors() const requires HasColor<HalfEdge>
{
	return ConstWedgeColorsRangeIterator(
		*this, &FaceHalfEdgeReference::wedgeColorBegin, &FaceHalfEdgeReference::wedgeColorEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::WedgeTexCoordsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeTexCoordBegin() requires HasTexCoord<HalfEdge>
{
	return WedgeTexCoordsIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::WedgeTexCoordsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeTexCoordEnd() requires HasTexCoord<HalfEdge>
{
	return WedgeTexCoordsIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstWedgeTexCoordsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeTexCoordBegin() const requires HasTexCoord<HalfEdge>
{
	return ConstWedgeTexCoordsIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstWedgeTexCoordsIterator
FaceHalfEdgeReference<HalfEdge>::wedgeTexCoordEnd() const requires HasTexCoord<HalfEdge>
{
	return ConstWedgeTexCoordsIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::WedgeTexCoordsRangeIterator
FaceHalfEdgeReference<HalfEdge>::wedgeTexCoords() requires HasTexCoord<HalfEdge>
{
	return WedgeTexCoordsRangeIterator(
		*this,
		&FaceHalfEdgeReference::wedgeTexCoordBegin,
		&FaceHalfEdgeReference::wedgeTexCoordEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstWedgeTexCoordsRangeIterator
FaceHalfEdgeReference<HalfEdge>::wedgeTexCoords() const requires HasTexCoord<HalfEdge>
{
	return ConstWedgeTexCoordsRangeIterator(
		*this,
		&FaceHalfEdgeReference::wedgeTexCoordBegin,
		&FaceHalfEdgeReference::wedgeTexCoordEnd);
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::updateHalfEdgeReferences(
	const HalfEdge* oldBase,
	const HalfEdge* newBase)
{
	if (ohe != nullptr) {
		size_t diff = ohe - oldBase;
		ohe = (HalfEdge*)newBase + diff;
	}
	for (uint i = 0; i < ihe.size(); ++i) {
		if (ihe[i] != nullptr) {
			size_t diff = ihe[i] - oldBase;
			ihe[i] = (HalfEdge*)newBase + diff;
		}
	}
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::updateHalfEdgeReferencesAfterCompact(
	const HalfEdge*         base,
	const std::vector<int>& newIndices)
{
	if (ohe != nullptr) {
		size_t diff = ohe - base;
		if (newIndices[diff] < 0)
			ohe = nullptr;
		else
			ohe = (HalfEdge*)base + newIndices[diff];
	}
	for (uint i = 0; i < ihe.size(); ++i) {
		if (ihe[i] != nullptr) {
			size_t diff = ihe[i] - base;
			if (newIndices[diff] < 0)
				ihe[i] = nullptr;
			else
				ihe[i] = (HalfEdge*)base + newIndices[diff];
		}
	}
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::updateVertexReferences(const Vertex *, const Vertex *)
{
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::updateVertexReferencesAfterCompact(
	const Vertex*,
	const std::vector<int>&)
{
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::updateFaceReferences(const Face*, const Face*)
{
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::updateFaceReferencesAfterCompact(
	const Face*,
	const std::vector<int>&)
{
}

template<typename HalfEdge>
template<typename Element>
void FaceHalfEdgeReference<HalfEdge>::importFrom(const Element &e)
{
}

template<typename HalfEdge>
template<typename OtherFace, typename OtherHEdge>
void FaceHalfEdgeReference<HalfEdge>::importHalfEdgeReferencesFrom(
	const OtherFace&  e,
	HalfEdge*         base,
	const OtherHEdge* ebase)
{
	if constexpr (HasFaceHalfEdgeReference<OtherFace>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.outerHalfEdge() != nullptr) {
				ohe = (HalfEdge*)base + (e.outerHalfEdge() - ebase);
			}
			ihe.resize(e.numberHoles());
			for (uint i = 0; i < ihe.size(); ++i) {
				if (e.innerHalfEdge(i) != nullptr) {
					ihe[i] = (HalfEdge*)base + (e.innerHalfEdge(i) - ebase);
				}
			}
		}
	}
}

} // namespace vcl::comp
