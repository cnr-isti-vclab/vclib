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
 * @brief Sets a list of Vertex references to the face. If the Face size is dynamic, will take care
 * to update the also the number of adjacent faces and the number of wedge components, if these
 * components are part of the Face and if the size of the Face is changed. On the contrary, if the
 * Face size is static, the number of vertices of the list must be equal to the size of the Face.
 *
 * @param list
 */
template<class... Args>
template<typename Vertex>
void Face<Args...>::setVertices(const std::vector<Vertex*>& list)
{
	using F = Face<Args...>;
	
	VRefs::setVertices(list);
	static const int VN = F::VERTEX_NUMBER;
	if constexpr(VN < 0){
		if constexpr (face::hasAdjacentFaces<F>()) {
			using T = typename F::AdjacentFaces;

			T::resizeAdjFaces(list.size());
		}
		if constexpr (face::hasOptionalAdjacentFaces<F>()) {
			using T = typename Face::OptionalAdjacentFaces;
			
			if (T::adjFacesEnabled())
				T::resizeAdjFaces(list.size());
		}
		if constexpr (face::hasWedgeTexCoords<F>()) {
			using TC = typename F::WedgeTexCoords;

			TC::resizeWedgeTexCoords(list.size());
		}
		if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
			using TC = typename F::OptionalWedgeTexCoords;
			
			if (TC::wedgeTexCoordsEnabled())
				TC::resizeWedgeTexCoords(list.size());
		}
	}
}

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
	using F = Face<Args...>;
	
	VRefs::resizeVertices(n);
	if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::resizeAdjFaces(n);
	}
	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::resizeAdjFaces(n);
	}
	if constexpr (face::hasWedgeTexCoords<Face>()) {
		using TC = typename F::WedgeTexCoords;

		TC::resizeWedgeTexCoords(n);
	}
	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using TC = typename F::OptionalWedgeTexCoords;
		
		if (TC::wedgeTexCoordsEnabled())
			TC::resizeWedgeTexCoords(n);
	}
}

template<class... Args>
template<typename Vertex, int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::pushVertex(Vertex* v)
{
	using F = Face<Args...>;
	
	VRefs::pushVertex(v);
	if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::pushAdjFace(nullptr);
	}
	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::pushAdjFace(nullptr);
	}
	if constexpr (face::hasWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using TC = typename F::WedgeTexCoords;

		TC::pushWedgeTexCoord(TexCoord<S>());
	}
	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using TC = typename F::OptionalWedgeTexCoords;
		
		if (TC::wedgeTexCoordsEnabled())
			TC::pushWedgeTexCoords(TexCoord<S>());
	}
}

template<class... Args>
template<typename Vertex, int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::insertVertex(unsigned int i, Vertex* v)
{
	using F = Face<Args...>;
	
	VRefs::insertVertex(i, v);
	if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::insertAdjFace(i, nullptr);
	}
	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::insertAdjFace(i, nullptr);
	}
	if constexpr (face::hasWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using TC = typename F::WedgeTexCoords;

		TC::insertWedgeTexCoord(i, TexCoord<S>());
	}
	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using S = typename F::WedgeTexCoordScalarType;
		using TC = typename F::OptionalWedgeTexCoords;
		
		if (TC::wedgeTexCoordsEnabled())
			TC::insertWedgeTexCoord(i, TexCoord<S>());
	}
}

template<class... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::eraseVertex(unsigned int i)
{
	using F = Face<Args...>;
	
	VRefs::eraseVertex(i);
	if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::eraseAdjFace(i);
	}
	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::eraseAdjFace(i);
	}
	if constexpr (face::hasWedgeTexCoords<F>()) {
		using TC = typename F::WedgeTexCoords;

		TC::eraseWedgeTexCoord(i);
	}
	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using TC = typename F::OptionalWedgeTexCoords;
		
		if (TC::wedgeTexCoordsEnabled())
			TC::eraseWedgeTexCoord(i);
	}
}

template<class... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::clearVertices()
{
	using F = Face<Args...>;
	
	VRefs::clearVertices();
	if constexpr (face::hasAdjacentFaces<F>()) {
		using T = typename F::AdjacentFaces;

		T::clearAdjFaces();
	}
	if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::clearAdjFaces();
	}
	if constexpr (face::hasWedgeTexCoords<F>()) {
		using TC = typename F::WedgeTexCoords;

		TC::clearWedgeTexCoord();
	}
	if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		using TC = typename F::OptionalWedgeTexCoords;
		
		if (TC::wedgeTexCoordsEnabled())
			TC::clearWedgeTexCoord();
	}
}

}

