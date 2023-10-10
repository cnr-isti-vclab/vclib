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

#ifndef VCL_MESH_ELEMENTS_FACE_H
#define VCL_MESH_ELEMENTS_FACE_H

#include <vclib/concepts/mesh/elements/face.h>
#include <vclib/concepts/ranges/range.h>
#include <vclib/views/view.h>

#include "element.h"


namespace vcl {

/**
 * @brief The Face class represents an Face element of the vcl::Mesh class.
 *
 * Using the FaceContainer class, it is possible to add a vector of Face
 * elements to a mesh, and manage them with the member functions exposed by the
 * FaceContainer. Each Face element exposes all the member functions of its
 * Component types.
 *
 * @tparam MeshType The type of the parent mesh.
 * @tparam Comps The types of the components of the element.
 *
 * @ingroup elements
 */
template<typename MeshType, typename... Comps>
class Face : public Element<FACE, MeshType, Comps...>
{
	using Base = Element<FACE, MeshType, Comps...>;

	// VertexPointers component of the Face
	using VPtrs = typename Face::VertexPointers;

	static const int NV = VPtrs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	using VertexType = typename VPtrs::VertexType;

	/**
	 * @brief Empty constructor.
	 *
	 * Calls automatically all the empty constructors of all the components
	 * available in the Face (for all the components non-available, their empty
	 * constructor is called only when they become available).
	 */
	Face() = default;

	/**
	 * @brief Constructs a Face with the given range of vertex pointers.
	 *
	 * Sets a range of Vertex pointers to the face.
	 * If the Face size is static, the number of vertices of the range must be
	 * equal to the size of the Face (the value returned by vertexNumber()). If
	 * the Face size is dynamic, it will take care to update the also the size
	 * of the components tied to the vertex number of the face.
	 *
	 * @param[in] r: a range of vertex pointers in counterclockwise order
	 * that will be set as vertices of the face.
	 */
	template<Range Rng>
	Face(Rng&& r) requires RangeOfConvertibleTo<Rng, VertexType*>
	{
		setVertices(r);
	}

	/**
	 * @brief Creates a new Face, setting the Vertex pointer arguments to it.
	 *
	 * If the Face size is static, the number of vertices of the list must be
	 * equal to the size of the Face (the value returned by vertexNumber()). If
	 * the Face size is dynamic, it will take care to update the also the size
	 * of the components tied to the vertex number of the face.
	 *
	 * @param[in] args: a variable number of vertex pointers in counterclockwise
	 * order that will be set as vertices of the face.
	 */
	template<typename... V>
	Face(V... args) requires (std::convertible_to<V, VertexType*> && ...)
	{
		setVertices(std::list({args...}));
	}

	/**
	 * @brief Sets all the Vertex pointers to the face.
	 *
	 * If the Face size is static, the number of vertices of the input range
	 * must be equal to the size of the Face (the value returned by
	 * vertexNumber()). If the Face size is dynamic, it will take care to update
	 * the also the size of the components tied to the vertex number of the
	 * face.
	 *
	 * @param[in] r: a range of vertex pointers in counterclockwise order
	 * that will be set as vertices of the face.
	 */
	template<Range Rng>
	void setVertices(Rng&& r) requires RangeOfConvertibleTo<Rng, VertexType*>
	{
		using F = Face<MeshType, TypeWrapper<Comps...>>;

		VPtrs::setVertices(r);

		// if polygonal, I need to resize all the TTVN components
		if constexpr (NV < 0) {
			(resizeTTVNComponent<Comps>(std::ranges::size(r)), ...);
		}
	}

	/**
	 * @brief Sets a list of Vertex pointers to the face.
	 *
	 * If the Face size is static, the number of vertices of the list must be
	 * equal to the size of the Face (the value returned by vertexNumber()). If
	 * the Face size is dynamic, it will take care to update the also the size
	 * of the components tied to the vertex number of the face.
	 *
	 * @param[in] args: a variable number of vertex pointers in counterclockwise
	 * order that will be set as vertices of the face.
	 */
	template<typename... V>
	void setVertices(V... args)
		requires (std::convertible_to<V, VertexType*> && ...)
	{
		setVertices(std::list({args...}));
	}

	/**
	 * @brief Resize the number of Vertex Pointers of the Face, taking care of
	 * updating also the other components of the Face that are tied to that
	 * number.
	 *
	 * If n is greater than the old number of vertex pointers, n vertex pointers
	 * (and relative tied components) will be added. If n is lower than the old
	 * number of vertex pointers, the difference of vertex pointers (and
	 * relative tied components) will be removed.
	 *
	 * This member function is available only **if the face is polygonal (its
	 * size is dynamic, N < 0)**.
	 *
	 * @param n: the new number of vertices.
	 */
	void resizeVertices(uint n) requires PolygonFaceConcept<Face>
	{
		VPtrs::resizeVertices(n);

		// Now I need to resize all the TTVN components
		(resizeTTVNComponent<Comps>(n), ...);
	}

	void pushVertex(VertexType* v) requires PolygonFaceConcept<Face>
	{
		VPtrs::pushVertex(v);

		// Now I need to pushBack in all the TTVN components
		(pushBackTTVNComponent<Comps>(), ...);
	}

	void insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face>
	{
		VPtrs::insertVertex(i, v);

		// Now I need to insert in all the TTVN components
		(insertTTVNComponent<Comps>(i), ...);
	}

	void eraseVertex(uint i) requires PolygonFaceConcept<Face>
	{
		VPtrs::eraseVertex(i);

		// Now I need to erase in all the TTVN components
		(eraseTTVNComponent<Comps>(i), ...);
	}

	void clearVertices() requires PolygonFaceConcept<Face>
	{
		VPtrs::clearVertices();

		// Now I need to clear all the TTVN components
		(clearTTVNComponent<Comps>(), ...);
	}

	template<typename ElType>
	void importFrom(const ElType& v)
	{
		if constexpr (comp::HasVertexPointers<ElType> && NV < 0) {
			VPtrs::resizeVertices(v.vertexNumber());
			// Now I need to resize all the TTVN components
			(resizeTTVNComponent<Comps>(v.vertexNumber()), ...);
		}

		Base::importFrom(v);
	}

private:
	/**
	 * Calls the resize(n) on all the component containers that are tied to the
	 * vertex number
	 */
	template<typename Comp>
	void resizeTTVNComponent(uint n)
	{
		if constexpr (comp::IsTiedToVertexNumber<Comp>) {
			if (Comp::isAvailable())
				Comp::resize(n);
		}
	}

	/**
	 * Calls the pushBack() on all the component containers that are tied to the
	 * vertex number
	 */
	template<typename Comp>
	void pushBackTTVNComponent()
	{
		if constexpr (comp::IsTiedToVertexNumber<Comp>) {
			if (Comp::isAvailable())
				Comp::pushBack();
		}
	}

	/**
	 * Calls the insert(i) on all the component containers that are tied to the
	 * vertex number
	 */
	template<typename Comp>
	void insertTTVNComponent(uint i)
	{
		if constexpr (comp::IsTiedToVertexNumber<Comp>) {
			if (Comp::isAvailable())
				Comp::insert(i);
		}
	}

	/**
	 * Calls the erase(i) on all the component containers that are tied to the
	 * vertex number
	 */
	template<typename Comp>
	void eraseTTVNComponent(uint i)
	{
		if constexpr (comp::IsTiedToVertexNumber<Comp>) {
			if (Comp::isAvailable())
				Comp::erase(i);
		}
	}

	/**
	 * Calls the clear() on all the component containers that are tied to the
	 * vertex number
	 */
	template<typename Comp>
	void clearTTVNComponent()
	{
		if constexpr (comp::IsTiedToVertexNumber<Comp>) {
			if (Comp::isAvailable())
				Comp::clear();
		}
	}
};

template<typename MeshType, typename... Comps>
class Face<MeshType, TypeWrapper<Comps...>> : public Face<MeshType, Comps...>
{
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_FACE_H
