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

#include "face.h"

namespace vcl {

template<typename MeshType, typename... Args>
Face<MeshType, Args...>::Face()
{
	(construct<Args>(), ...);
}

/**
 * @brief Constructs a Face with the given set of vertices.
 *
 * Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of vertices of the list must be
 * equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @note This constructor cannot be called if the Face type belongs to a Dcel mesh.
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order that will be set as
 *                  vertices of the face.
 */
template<typename MeshType, typename... Args>
Face<MeshType, Args...>::Face(const std::vector<VertexType*>& list) // todo add requires
{
	setVertices(list);
}

template<typename MeshType, typename... Args>
uint Face<MeshType, Args...>::index() const
{
	assert(face::ParentMeshPointer<MeshType>::parentMesh());
	return face::ParentMeshPointer<MeshType>::parentMesh()->index(
		static_cast<const typename MeshType::FaceType*>(this));
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of arguments of this function
 * must be equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @note This constructor cannot be called if the Face type belongs to a Dcel mesh.
 *
 * @param[in] args: a variable number of vertex pointers in counterclockwise order that will be set
 *                  as vertices of the face.
 */
template<typename MeshType, typename... Args>
template<typename... V>
Face<MeshType, Args...>::Face(V... args) // todo add requires
{
	setVertices({args...});
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of vertices of the list must be
 * equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order that will be set as
 *                  vertices of the face.
 */
template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::setVertices(const std::vector<VertexType*>& list)
{
	using F = Face<Args...>;

	VRefs::setVertices(list);

	if constexpr (comp::HasAdjacentEdges<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::AdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::resizeAdjEdges(list.size());
	}

	if constexpr (face::HasAdjacentFaces<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::AdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::resizeAdjFaces(list.size());
	}

	if constexpr (face::HasWedgeColors<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::WedgeColors;

		if (T::isWedgeColorsEnabled())
			T::resizeWedgeColors(list.size());
	}

	if constexpr (face::HasWedgeTexCoords<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::WedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::resizeWedgeTexCoords(list.size());
	}
}

template<typename MeshType, typename... Args>
template<typename... V>
void Face<MeshType, Args...>::setVertices(V... args)
{
	setVertices({args...});
}

template<typename MeshType, typename... Args>
template<typename Element>
void Face<MeshType, Args...>::importFrom(const Element& f)
{
	(Args::importFrom(f), ...);
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
 * This member function is available only **if the face is polygonal (its size is dynamic, N < 0)**.
 *
 * @param n: the new number of vertices.
 */
//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::resizeVertices(uint n) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::pushVertex(VertexType* v) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::eraseVertex(uint i) requires PolygonFaceConcept<Face<Args...>>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::clearVertices() requires PolygonFaceConcept<Face<Args...>>
//{

//}

template<typename MeshType, typename... Args>
template<typename Comp>
void Face<MeshType, Args...>::construct()
{
	if constexpr (vcl::comp::HasInitMemberFunction<Comp>) {
		// todo - check here if component is optional and, in case, if enabled
		Comp::init();
	}
}

} // namespace vcl


