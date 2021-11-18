/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face.h"

namespace mgp {

template<typename... Args>
Face<Args...>::Face(const std::vector<VertexType*>& list)
{
	setVertices(list);
}

template<typename... Args>
template<typename... V>
Face<Args...>::Face(V... args)
{
	setVertices({args...});
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is dynamic, will take care
 * to update the also the number of adjacent faces and the number of wedge components, if these
 * components are part of the Face and if the size of the Face is changed. On the contrary, if the
 * Face size is static, the number of vertices of the list must be equal to the size of the Face.
 *
 * @param list
 */
template<typename... Args>
void Face<Args...>::setVertices(const std::vector<VertexType*>& list)
{
	using F = Face<Args...>;
	
	VRefs::setVertices(list);

	// Note: in this function, we cannot use:
	// if constexpr (face::hasAdjacentFaces<F>()) {...}
	// if constexpr (face::hasOptionalAdjacentFaces<F>()) {...}
	// having at least two if constexpr in the same function causes error C2143 on MSVC
	// this is probably an MSVC bug. Works on gcc and clang.
	// Does not make any sense since face::hasAdjacentFaces<F>() can be called without
	// any other constexpr called, and beacuase it is literally the same of calling
	// comp::hasAdjacentFaces<F>::value, which works.

	static const int VN = F::VERTEX_NUMBER;
	if constexpr(VN < 0){
		//if constexpr (face::hasAdjacentFaces<F>()) {
		if constexpr (comp::hasAdjacentFacesT<F>::value) {
			using T = typename F::AdjacentFaces;

			T::resizeAdjFaces(list.size());
		}

		//if constexpr (face::hasOptionalAdjacentFaces<F>()) {
		if constexpr (comp::hasOptionalAdjacentFacesT<F>::value) {
			using T = typename Face::OptionalAdjacentFaces;
			
			if (T::adjFacesEnabled())
				T::resizeAdjFaces(list.size());
		}

		//if constexpr (face::hasWedgeTexCoords<F>()) {
		if constexpr (comp::hasWedgeTexCoordsT<F>::value) {
			using T = typename F::WedgeTexCoords;

			T::resizeWedgeTexCoords(list.size());
		}

		//if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
		if constexpr (comp::hasOptionalWedgeTexCoordsT<F>::value) {
			using T = typename F::OptionalWedgeTexCoords;
			
			if (T::wedgeTexCoordsEnabled())
				T::resizeWedgeTexCoords(list.size());
		}
	}
}

template<typename... Args>
template<typename... V>
void Face<Args...>::setVertices(V... args)
{
	setVertices({args...});
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
template<typename... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::resizeVertices(unsigned int n)
{
	using F = Face<Args...>;
	
	VRefs::resizeVertices(n);

	// Note: in this function, we cannot use:
	// if constexpr (face::hasAdjacentFaces<F>()) {...}
	// if constexpr (face::hasOptionalAdjacentFaces<F>()) {...}
	// having at least two if constexpr in the same function causes error C2143 on MSVC
	// this is probably an MSVC bug. Works on gcc and clang.
	// Does not make any sense since face::hasAdjacentFaces<F>() can be called without
	// any other constexpr called, and beacuase it is literally the same of calling
	// comp::hasAdjacentFaces<F>::value, which works.

	//if constexpr (face::hasAdjacentFaces<F>()) {
	if constexpr (comp::hasAdjacentFacesT<F>::value) {
		using T = typename F::AdjacentFaces;

		T::resizeAdjFaces(n);
	}

	//if constexpr (face::hasOptionalAdjacentFaces<F>()) {
	if constexpr (comp::hasOptionalAdjacentFacesT<F>::value) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::resizeAdjFaces(n);
	}

	//if constexpr (face::hasWedgeTexCoords<F>()) {
	if constexpr (comp::hasWedgeTexCoordsT<Face>::value) {
		using T = typename F::WedgeTexCoords;

		T::resizeWedgeTexCoords(n);
	}

	//if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
	if constexpr (comp::hasOptionalWedgeTexCoordsT<F>::value) {
		using T = typename F::OptionalWedgeTexCoords;
		
		if (T::wedgeTexCoordsEnabled())
			T::resizeWedgeTexCoords(n);
	}
}

template<typename... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::pushVertex(VertexType* v)
{
	using F = Face<Args...>;
	
	VRefs::pushVertex(v);

	// Note: in this function, we cannot use:
	// if constexpr (face::hasAdjacentFaces<F>()) {...}
	// if constexpr (face::hasOptionalAdjacentFaces<F>()) {...}
	// having at least two if constexpr in the same function causes error C2143 on MSVC
	// this is probably an MSVC bug. Works on gcc and clang.
	// Does not make any sense since face::hasAdjacentFaces<F>() can be called without
	// any other constexpr called, and beacuase it is literally the same of calling
	// comp::hasAdjacentFaces<F>::value, which works.

	//if constexpr (face::hasAdjacentFaces<F>()) {
	if constexpr (comp::hasAdjacentFacesT<F>::value) {
		using T = typename F::AdjacentFaces;

		T::pushAdjFace(nullptr);
	}

	//if constexpr (face::hasOptionalAdjacentFaces<F>()) {
	if constexpr (comp::hasOptionalAdjacentFacesT<F>::value) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::pushAdjFace(nullptr);
	}

	//if constexpr (face::hasWedgeTexCoords<F>()) {
	if constexpr (comp::hasWedgeTexCoordsT<F>::value) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::WedgeTexCoords;

		T::pushWedgeTexCoord(TexCoord<S>());
	}

	//if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
	if constexpr (comp::hasOptionalWedgeTexCoordsT<F>::value) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::OptionalWedgeTexCoords;
		
		if (T::wedgeTexCoordsEnabled())
			T::pushWedgeTexCoords(TexCoord<S>());
	}
}

template<typename... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::insertVertex(unsigned int i, VertexType* v)
{
	using F = Face<Args...>;
	
	VRefs::insertVertex(i, v);

	// Note: in this function, we cannot use:
	// if constexpr (face::hasAdjacentFaces<F>()) {...}
	// if constexpr (face::hasOptionalAdjacentFaces<F>()) {...}
	// having at least two if constexpr in the same function causes error C2143 on MSVC
	// this is probably an MSVC bug. Works on gcc and clang.
	// Does not make any sense since face::hasAdjacentFaces<F>() can be called without
	// any other constexpr called, and beacuase it is literally the same of calling
	// comp::hasAdjacentFaces<F>::value, which works.

	//if constexpr (face::hasAdjacentFaces<F>()) {
	if constexpr (comp::hasAdjacentFacesT<F>::value) {
		using T = typename F::AdjacentFaces;

		T::insertAdjFace(i, nullptr);
	}

	//if constexpr (face::hasOptionalAdjacentFaces<F>()) {
	if constexpr (comp::hasOptionalAdjacentFacesT<F>::value) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::insertAdjFace(i, nullptr);
	}

	//if constexpr (face::hasWedgeTexCoords<F>()) {
	if constexpr (comp::hasWedgeTexCoordsT<F>::value) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::WedgeTexCoords;

		T::insertWedgeTexCoord(i, TexCoord<S>());
	}

	//if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
	if constexpr (comp::hasOptionalWedgeTexCoordsT<F>::value) {
		using S = typename F::WedgeTexCoordScalarType;
		using T = typename F::OptionalWedgeTexCoords;
		
		if (T::wedgeTexCoordsEnabled())
			T::insertWedgeTexCoord(i, TexCoord<S>());
	}
}

template<typename... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::eraseVertex(unsigned int i)
{
	using F = Face<Args...>;
	
	VRefs::eraseVertex(i);

	// Note: in this function, we cannot use:
	// if constexpr (face::hasAdjacentFaces<F>()) {...}
	// if constexpr (face::hasOptionalAdjacentFaces<F>()) {...}
	// having at least two if constexpr in the same function causes error C2143 on MSVC
	// this is probably an MSVC bug. Works on gcc and clang.
	// Does not make any sense since face::hasAdjacentFaces<F>() can be called without
	// any other constexpr called, and beacuase it is literally the same of calling
	// comp::hasAdjacentFaces<F>::value, which works.

	//if constexpr (face::hasAdjacentFaces<F>()) {
	if constexpr (comp::hasAdjacentFacesT<F>::value) {
		using T = typename F::AdjacentFaces;

		T::eraseAdjFace(i);
	}

	//if constexpr (face::hasOptionalAdjacentFaces<F>()) {
	if constexpr (comp::hasOptionalAdjacentFacesT<F>::value) {
		using T = typename F::OptionalAdjacentFaces;
		
		if (T::adjFacesEnabled())
			T::eraseAdjFace(i);
	}

	//if constexpr (face::hasWedgeTexCoords<F>()) {
	if constexpr (comp::hasWedgeTexCoordsT<F>::value) {
		using T = typename F::WedgeTexCoords;

		T::eraseWedgeTexCoord(i);
	}

	//if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
	if constexpr (comp::hasOptionalWedgeTexCoordsT<F>::value) {
		using T = typename F::OptionalWedgeTexCoords;
		
		if (T::wedgeTexCoordsEnabled())
			T::eraseWedgeTexCoord(i);
	}
}

template<typename... Args>
template<int U>
comp::internal::ReturnIfIsVector<U, void> Face<Args...>::clearVertices()
{
	using F = Face<Args...>;
	
	VRefs::clearVertices();

	// Note: in this function, we cannot use:
	// if constexpr (face::hasAdjacentFaces<F>()) {...}
	// if constexpr (face::hasOptionalAdjacentFaces<F>()) {...}
	// having at least two if constexpr in the same function causes error C2143 on MSVC
	// this is probably an MSVC bug. Works on gcc and clang.
	// Does not make any sense since face::hasAdjacentFaces<F>() can be called without
	// any other constexpr called, and beacuase it is literally the same of calling
	// comp::hasAdjacentFaces<F>::value, which works.

	//if constexpr (face::hasAdjacentFaces<F>()) {
	if constexpr (comp::hasAdjacentFacesT<F>::value) {
		using T = typename F::AdjacentFaces;

		T::clearAdjFaces();
	}

	//if constexpr (face::hasOptionalAdjacentFaces<F>()) {
	if constexpr (comp::hasOptionalAdjacentFacesT<F>::value) {
		using T = typename F::OptionalAdjacentFaces;

		if (T::adjFacesEnabled())
			T::clearAdjFaces();
	}

	//if constexpr (face::hasWedgeTexCoords<F>()) {
	if constexpr (comp::hasWedgeTexCoordsT<F>::value) {
		using T = typename F::WedgeTexCoords;

		T::clearWedgeTexCoord();
	}

	//if constexpr (face::hasOptionalWedgeTexCoords<F>()) {
	if constexpr (comp::hasOptionalWedgeTexCoordsT<F>::value) {
		using T = typename F::OptionalWedgeTexCoords;
		
		if (T::wedgeTexCoordsEnabled())
			T::clearWedgeTexCoord();
	}
}

}

