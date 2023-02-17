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

#ifndef VCL_MESH_COMPONENTS_FACE_HALF_EDGE_REFERENCE_H
#define VCL_MESH_COMPONENTS_FACE_HALF_EDGE_REFERENCE_H

#include <vector>

#include <vclib/mesh/iterators/half_edge/face_adj_face_iterator.h>
#include <vclib/mesh/iterators/half_edge/face_half_edge_iterator.h>
#include <vclib/mesh/iterators/half_edge/face_vertex_iterator.h>
#include <vclib/mesh/iterators/half_edge/face_wedge_color_iterator.h>
#include <vclib/mesh/iterators/half_edge/face_wedge_tex_coord_iterator.h>
#include <vclib/iterators/container_range_iterator.h>

#include <vclib/space/color.h>

#include "concepts/face_half_edge_reference.h"
#include "concepts/color.h"
#include "concepts/tex_coord.h"
#include "internal/component_data.h"

namespace vcl::comp {

template<
	typename HalfEdge,
	typename ElementType = void,
	bool optional        = false>
class FaceHalfEdgeReference
{
	using ThisType = FaceHalfEdgeReference<HalfEdge, ElementType, optional>;

	struct FHERData {
		HalfEdge*              ohe; // outer half edge
		std::vector<HalfEdge*> ihe; // inner half edges, one for each hole of the face

		short texIndex;
	};

	using Vertex = typename HalfEdge::VertexType;
	using Face   = typename HalfEdge::FaceType;

public:
	using DataValueType = FHERData; // data that the component stores internally (or vertically)
	using FaceHalfEdgeReferencesComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	using HalfEdgeType = HalfEdge;
	using VertexType   = typename HalfEdge::VertexType;

	/* Iterator Types declaration */

	using AdjacentFaceIterator      = vcl::FaceAdjFaceIterator<HalfEdge>;
	using ConstAdjacentFaceIterator = vcl::ConstFaceAdjFaceIterator<HalfEdge>;
	using AdjacentFaceRangeIterator = ContainerRangeIterator<FaceHalfEdgeReference, AdjacentFaceIterator>;
	using ConstAdjacentFaceRangeIterator =
		ConstContainerRangeIterator<FaceHalfEdgeReference, ConstAdjacentFaceIterator>;

	using InnerHalfEdgeIterator      = typename std::vector<HalfEdge*>::iterator;
	using ConstInnerHalfEdgeIterator = typename std::vector<HalfEdge*>::const_iterator;
	using InnerHalfEdgeRangeIterator = ContainerRangeIterator<FaceHalfEdgeReference, InnerHalfEdgeIterator>;
	using ConstInnerHalfEdgeRangeIterator =
		ContainerRangeIterator<FaceHalfEdgeReference, ConstInnerHalfEdgeIterator>;

	using HalfEdgeIterator      = vcl::FaceHalfEdgeIterator<HalfEdge>;
	using ConstHalfEdgeIterator = vcl::ConstFaceHalfEdgeIterator<HalfEdge>;
	using HalfEdgeRangeIterator = ContainerRangeIterator<FaceHalfEdgeReference, HalfEdgeIterator>;
	using ConstHalfEdgeRangeIterator =
		ConstContainerRangeIterator<FaceHalfEdgeReference, ConstHalfEdgeIterator>;

	using VertexIterator           = vcl::FaceVertexIterator<HalfEdge>;
	using ConstVertexIterator      = vcl::ConstFaceVertexIterator<HalfEdge>;
	using VertexRangeIterator      = ContainerRangeIterator<FaceHalfEdgeReference, VertexIterator>;
	using ConstVertexRangeIterator = ConstContainerRangeIterator<FaceHalfEdgeReference, ConstVertexIterator>;

	// these types won't be used when half edge has no colors - all member functions are disabled
	using WedgeColorsIterator = vcl::FaceWedgeColorIterator<HalfEdge>;
	using ConstWedgeColorsIterator = vcl::ConstFaceWedgeColorIterator<HalfEdge>;
	using WedgeColorsRangeIterator = ContainerRangeIterator<FaceHalfEdgeReference, WedgeColorsIterator>;
	using ConstWedgeColorsRangeIterator =
		ConstContainerRangeIterator<FaceHalfEdgeReference, ConstWedgeColorsIterator>;

	// these types won't be used when half edge has no texcoords - all member functions are disabled
	using WedgeTexCoordsIterator = vcl::FaceWedgeTexCoordIterator<HalfEdge>;
	using ConstWedgeTexCoordsIterator = vcl::ConstFaceWedgeTexCoordIterator<HalfEdge>;
	using WedgeTexCoordsRangeIterator = ContainerRangeIterator<FaceHalfEdgeReference, WedgeTexCoordsIterator>;
	using ConstWedgeTexCoordsRangeIterator =
		ConstContainerRangeIterator<FaceHalfEdgeReference, ConstWedgeTexCoordsIterator>;

	// Vertex references can be accessed from a face using half edge reference, therefore this
	// component claims that it is the VertexReferences component. This is done just for
	// compatibility between mesh types.
	using VertexReferences         = FaceHalfEdgeReference;

	static const int VERTEX_NUMBER          = -1; // half edges support by design polygonal meshes
	static const int ADJ_FACE_NUMBER        = -1;
	static const int WEDGE_COLOR_NUMBER     = -1;
	static const int WEDGE_TEX_COORD_NUMBER = -1;

	/* Constructor and isEnabled */

	FaceHalfEdgeReference();

	void init();

	bool isEnabled();

	/* Member functions */

	const HalfEdge* outerHalfEdge() const;
	HalfEdge*&      outerHalfEdge();

	uint            numberHoles() const;
	const HalfEdge* innerHalfEdge(uint i) const;
	HalfEdge*&      innerHalfEdge(uint i);

	void resizeInnerHalfEdges(uint n);
	void pushInnerHalfEdge(HalfEdge* he);
	void insertInnerHalfEdge(uint i, HalfEdge* he);
	void eraseInnerHalfEdge(uint i);
	void clearInnerHalfEdges();

	/* VertexReferences compatibility */

	uint vertexNumber() const;

	Vertex*&      vertex(uint i);
	const Vertex* vertex(uint i) const;
	Vertex*&      vertexMod(int i);
	const Vertex* vertexMod(int i) const;

	void setVertex(Vertex* v, uint i);
	void setVertices(const std::vector<Vertex*>& list);

	bool containsVertex(const Vertex* v) const;

	VertexIterator      findVertex(const Vertex* v);
	ConstVertexIterator findVertex(const Vertex* v) const;

	int indexOfVertex(const Vertex* v) const;
	int indexOfEdge(const Vertex* v1, const Vertex* v2) const;

	/* AdjacentFaces compatibility */

	uint adjFacesNumber() const;

	Face*&      adjFace(uint i);
	const Face* adjFace(uint i) const;
	Face*&      adjFaceMod(int i);
	const Face* adjFaceMod(int i) const;

	void setAdjFace(Face* f, uint i);
	void setAdjFaces(const std::vector<Face*>& list);

	bool containsAdjFace(const Face* f) const;

	AdjacentFaceIterator      findAdjFace(const Face* f);
	ConstAdjacentFaceIterator findAdjFace(const Face* f) const;

	int indexOfAdjFace(const Face* f) const;

	/* WedgeColor compatibility */

	vcl::Color&       wedgeColor(uint i) requires HasColor<HalfEdge>;
	const vcl::Color& wedgeColor(uint i) const requires HasColor<HalfEdge>;

	vcl::Color&       wedgeColorMod(int i) requires HasColor<HalfEdge>;
	const vcl::Color& wedgeColorMod(int i) const requires HasColor<HalfEdge>;

	void setWedgeColor(const vcl::Color& t, uint i) requires HasColor<HalfEdge>;
	void setWedgeColors(const std::vector<vcl::Color>& list) requires HasColor<HalfEdge>;

	bool isWedgeColorsEnabled() const requires HasColor<HalfEdge>;

	/* WedgeTexCoords compatibility */

	template<HasTexCoord OHE = HalfEdge>
	typename OHE::TexCoordType& wedgeTexCoord(uint i);

	template<HasTexCoord OHE = HalfEdge>
	const typename OHE::TexCoordType& wedgeTexCoord(uint i) const;

	template<HasTexCoord OHE = HalfEdge>
	typename OHE::TexCoordType& wedgeTexCoordMod(int i);

	template<HasTexCoord OHE = HalfEdge>
	const typename OHE::TexCoordType& wedgeTexCoordMod(int i) const;

	template<HasTexCoord OHE = HalfEdge>
	void setWedgeTexCoord(const typename OHE::TexCoordType& t, uint i);

	template<HasTexCoord OHE = HalfEdge>
	void setWedgeTexCoords(const std::vector<typename OHE::TexCoordType>& list);

	template<HasTexCoord OHE = HalfEdge>
	short& textureIndex();

	template<HasTexCoord OHE = HalfEdge>
	short textureIndex() const;

	bool isWedgeTexCoordsEnabled() const requires HasTexCoord<HalfEdge>;

	/* Iterator Member functions */

	AdjacentFaceIterator           adjFaceBegin();
	AdjacentFaceIterator           adjFaceBegin(HalfEdge* he);
	AdjacentFaceIterator           adjFaceEnd();
	ConstAdjacentFaceIterator      adjFaceBegin() const;
	ConstAdjacentFaceIterator      adjFaceBegin(const HalfEdge* he) const;
	ConstAdjacentFaceIterator      adjFaceEnd() const;
	AdjacentFaceRangeIterator      adjFaces();
	ConstAdjacentFaceRangeIterator adjFaces() const;

	HalfEdgeIterator           haflEdgeBegin();
	HalfEdgeIterator           haflEdgeBegin(HalfEdge* he);
	ConstHalfEdgeIterator      halfEdgeBegin() const;
	ConstHalfEdgeIterator      halfEdgeBegin(const HalfEdge* he) const;
	HalfEdgeIterator           halfEdgeEnd();
	ConstHalfEdgeIterator      halfEdgeEnd() const;
	HalfEdgeRangeIterator      halfEdges();
	ConstHalfEdgeRangeIterator halfEdges() const;

	InnerHalfEdgeIterator           innerHalfEdgeBegin();
	ConstInnerHalfEdgeIterator      innerHalfEdgeBegin() const;
	InnerHalfEdgeIterator           innerHalfEdgeEnd();
	ConstInnerHalfEdgeIterator      innerHalfEdgeEnd() const;
	InnerHalfEdgeRangeIterator      innerHalfEdges();
	ConstInnerHalfEdgeRangeIterator innerHalfEdges() const;

	VertexIterator           vertexBegin();
	VertexIterator           vertexBegin(HalfEdge* he);
	ConstVertexIterator      vertexBegin() const;
	ConstVertexIterator      vertexBegin(const HalfEdge* he) const;
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertices();
	ConstVertexRangeIterator vertices() const;

	WedgeColorsIterator           wedgeColorBegin() requires HasColor<HalfEdge>;
	WedgeColorsIterator           wedgeColorEnd() requires HasColor<HalfEdge>;
	ConstWedgeColorsIterator      wedgeColorBegin() const requires HasColor<HalfEdge>;
	ConstWedgeColorsIterator      wedgeColorEnd() const requires HasColor<HalfEdge>;
	WedgeColorsRangeIterator      wedgeColors() requires HasColor<HalfEdge>;
	ConstWedgeColorsRangeIterator wedgeColors() const requires HasColor<HalfEdge>;

	WedgeTexCoordsIterator           wedgeTexCoordBegin() requires HasTexCoord<HalfEdge>;
	WedgeTexCoordsIterator           wedgeTexCoordEnd() requires HasTexCoord<HalfEdge>;
	ConstWedgeTexCoordsIterator      wedgeTexCoordBegin() const requires HasTexCoord<HalfEdge>;
	ConstWedgeTexCoordsIterator      wedgeTexCoordEnd() const requires HasTexCoord<HalfEdge>;
	WedgeTexCoordsRangeIterator      wedgeTexCoords() requires HasTexCoord<HalfEdge>;
	ConstWedgeTexCoordsRangeIterator wedgeTexCoords() const requires HasTexCoord<HalfEdge>;

protected:
	void updateHalfEdgeReferences(const HalfEdge* oldBase, const HalfEdge* newBase);

	void
	updateHalfEdgeReferencesAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	void updateFaceReferences(const Face* oldBase, const Face* newBase);
	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

	template<typename Element>
	void importFrom(const Element& e);

	template<typename OtherFace, typename OtherHEdge>
	void importHalfEdgeReferencesFrom(const OtherFace& e, HalfEdge* base, const OtherHEdge* ebase);

private:
	HalfEdge*& ohe(); // outer half edge
	const HalfEdge* ohe() const;
	std::vector<HalfEdge*>& ihe();           // inner half edges, one for each hole of the face
	const std::vector<HalfEdge*>& ihe() const;

	short& texIndex();
	short texIndex() const;

	internal::ComponentData<FHERData, IS_VERTICAL> data;
};

} // namespace vcl::comp

#include "face_half_edge_reference.cpp"

#endif // VCL_MESH_COMPONENTS_FACE_HALF_EDGE_REFERENCE_H
