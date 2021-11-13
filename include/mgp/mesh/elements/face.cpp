/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face.h"

namespace mgp {

/**
 * @brief Return the id of the face. The id of a Face is tied to its Container position in a Mesh.
 * @return the id of the face.
 */
template<class... Args>
unsigned int Face<Args...>::id() const { return _id; }

/**
 * @brief Resize the number of Vertex References of the Face, taking care of updating also the
 * number of adjacent faces and the number of wedge components of the Face, if these components
 * are part of the Face.
 *
 * If n is greater than the old number of vertex references, n vertex references (and relative
 * wedge components) will be added. If n is lower than the old number of vertex references, the
 * difference of vertex references (and relative wedge components) will be removed.
 *
 * @note This member function is available only if the size of the face is dynamic (-1), that is if
 * the Mesh is Polygonal.
 *
 * @param n: the new number of vertices.
 */
template<class... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::resizeVertices(unsigned int n)
{
	VRefs::resizeVertices(n);
	if constexpr (face::hasAdjacentFaces<Face>()) {
		using T = typename Face::FaceReferences;

		T::resizeFaces(n);
	}
	if constexpr (face::hasOptionalAdjacentFaces<Face>()) {
		using T = typename Face::OptionalFaceReferences;
		if (T::faceReferencesEnabled())
			T::resizeFaces(n);
	}
	if constexpr (face::hasWedgeTexCoords<Face>()) {
		static const int N = Face::WEDGE_TEXCOORD_NUMBER;
		using S = typename Face::WedgeTexCoordScalarType;
		using TC = typename Face::template WedgeTexCoords<S, N>;

		TC::resizeWedgeTexCoords(n);
	}
}

template<class... Args>
template<typename Vertex, int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::pushVertex(Vertex* v)
{
	VRefs::pushVertex(v);
	if constexpr (face::hasAdjacentFaces<Face>()) {
		using T = typename Face::FaceReferences;

		T::pushFace(nullptr);
	}
	if constexpr (face::hasOptionalAdjacentFaces<Face>()) {
		using T = typename Face::OptionalFaceReferences;
		if (T::faceReferencesEnabled())
			T::pushFace(nullptr);
	}
	if constexpr (face::hasWedgeTexCoords<Face>()) {
		static const int N = Face::WEDGE_TEXCOORD_NUMBER;
		using S = typename Face::WedgeTexCoordScalarType;
		using TC = typename Face::template WedgeTexCoords<S, N>;

		TC::pushWedgeTexCoord(TexCoord<S>());
	}
}

template<class... Args>
template<typename Vertex, int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::insertVertex(unsigned int i, Vertex* v)
{
	VRefs::insertVertex(i, v);
	if constexpr (face::hasAdjacentFaces<Face>()) {
		using T = typename Face::FaceReferences;

		T::insertFace(i, nullptr);
	}
	if constexpr (face::hasOptionalAdjacentFaces<Face>()) {
		using T = typename Face::OptionalFaceReferences;
		if (T::faceReferencesEnabled())
			T::insertFace(i, nullptr);
	}
	if constexpr (face::hasWedgeTexCoords<Face>()) {
		static const int N = Face::WEDGE_TEXCOORD_NUMBER;
		using S = typename Face::WedgeTexCoordScalarType;
		using TC = typename Face::template WedgeTexCoords<S, N>;

		TC::insertWedgeTexCoord(i, TexCoord<S>());
	}
}

template<class... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::eraseVertex(unsigned int i)
{
	VRefs::eraseVertex(i);
	if constexpr (face::hasAdjacentFaces<Face>()) {
		using T = typename Face::FaceReferences;

		T::eraseFace(i);
	}
	if constexpr (face::hasOptionalAdjacentFaces<Face>()) {
		using T = typename Face::OptionalFaceReferences;
		if (T::faceReferencesEnabled())
			T::eraseFace(i);
	}
	if constexpr (face::hasWedgeTexCoords<Face>()) {
		static const int N = Face::WEDGE_TEXCOORD_NUMBER;
		using S = typename Face::WedgeTexCoordScalarType;
		using TC = typename Face::template WedgeTexCoords<S, N>;

		TC::eraseWedgeTexCoord(i);
	}
}

template<class... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::clearVertices()
{
	VRefs::clearVertices();
	if constexpr (face::hasAdjacentFaces<Face>()) {
		using T = typename Face::FaceReferences;

		T::clearFaces();
	}
	if constexpr (face::hasOptionalAdjacentFaces<Face>()) {
		using T = typename Face::OptionalFaceReferences;
		if (T::faceReferencesEnabled())
			T::clearFaces();
	}
	if constexpr (face::hasWedgeTexCoords<Face>()) {
		static const int N = Face::WEDGE_TEXCOORD_NUMBER;
		using S = typename Face::WedgeTexCoordScalarType;
		using TC = typename Face::template WedgeTexCoords<S, N>;

		TC::clearWedgeTexCoord();
	}
}

}

