/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
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
class Face : public Element<ElemId::FACE, MeshType, Comps...>
{
    using Base = Element<ElemId::FACE, MeshType, Comps...>;

    // VertexPointers or VertexIndices component of the Face
    using VRefs = typename Face::VertexReferences;

    static const int NV = VRefs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
    using VertexType = typename VRefs::VertexType;

    /**
     * @brief Empty constructor.
     *
     * Calls automatically all the empty constructors of all the components
     * available in the Face (for all the components non-available, their empty
     * constructor is called only when they become available).
     */
    Face() = default;

    /**
     * @brief Sets all the Vertices to the face.
     *
     * If the Face size is static, the number of vertices of the input range
     * must be equal to the size of the Face (the value returned by
     * vertexNumber()). If the Face size is dynamic, it will take care to update
     * the also the size of the components tied to the vertex number of the
     * face.
     *
     * @tparam Rng: A range of vertex pointers or vertex indices in
     * counterclockwise order.
     *
     * @param[in] r: a range of vertex pointers or vertex indices in
     * counterclockwise order that will be set as vertices of the face.
     */
    template<Range Rng>
    void setVertices(Rng&& r) requires (
        RangeOfConvertibleTo<Rng, VertexType*> ||
        RangeOfConvertibleTo<Rng, uint>)
    {
        using F = Face<MeshType, TypeWrapper<Comps...>>;

        VRefs::setVertices(r);

        // if polygonal, I need to resize all the TTVN components
        if constexpr (NV < 0) {
            (resizeTTVNComponent<Comps>(std::ranges::size(r)), ...);
        }
    }

    /**
     * @brief Sets a list of Vertices to the face.
     *
     * If the Face size is static, the number of vertices of the list must be
     * equal to the size of the Face (the value returned by vertexNumber()). If
     * the Face size is dynamic, it will take care to update the also the size
     * of the components tied to the vertex number of the face.
     *
     * @tparam V: A list of vertex pointers or vertex indices in
     * counterclockwise order.
     *
     * @param[in] args: a variable number of vertex pointers or vertex indices
     * in counterclockwise order that will be set as vertices of the face.
     */
    template<typename... V>
    void setVertices(V... args) requires (
        (std::convertible_to<V, VertexType*> || std::convertible_to<V, uint>) &&
        ...)
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
        VRefs::resizeVertices(n);

        // Now I need to resize all the TTVN components
        (resizeTTVNComponent<Comps>(n), ...);
    }

    void pushVertex(VertexType* v) requires PolygonFaceConcept<Face>
    {
        VRefs::pushVertex(v);

        // Now I need to pushBack in all the TTVN components
        (pushBackTTVNComponent<Comps>(), ...);
    }

    void insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face>
    {
        VRefs::insertVertex(i, v);

        // Now I need to insert in all the TTVN components
        (insertTTVNComponent<Comps>(i), ...);
    }

    void eraseVertex(uint i) requires PolygonFaceConcept<Face>
    {
        VRefs::eraseVertex(i);

        // Now I need to erase in all the TTVN components
        (eraseTTVNComponent<Comps>(i), ...);
    }

    void clearVertices() requires PolygonFaceConcept<Face>
    {
        VRefs::clearVertices();

        // Now I need to clear all the TTVN components
        (clearTTVNComponent<Comps>(), ...);
    }

    template<typename ElType>
    void importFrom(const ElType& v)
    {
        if constexpr (comp::HasVertexReferences<ElType> && NV < 0) {
            VRefs::resizeVertices(v.vertexNumber());
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
