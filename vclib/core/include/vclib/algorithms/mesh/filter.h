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

#ifndef VCL_ALGORITHMS_MESH_FILTER_H
#define VCL_ALGORITHMS_MESH_FILTER_H

#include <vclib/mesh/requirements.h>
#include <vclib/misc/comparators.h>

#include <set>

namespace vcl {

namespace detail {

template<MeshConcept OutMeshType, uint ELEM_ID, MeshConcept InMeshType>
OutMeshType perElementMeshFilter(
    const InMeshType& m,
    Range auto&&      elemFilterRng,
    bool              saveBirthIndicesInCustomComponent = true)
{
    using OutElemType = OutMeshType::template ElementType<ELEM_ID>;

    std::string ccname = "birth" + vcl::elementEnumString<ELEM_ID>();

    OutMeshType res;
    res.enableSameOptionalComponentsOf(m);

    // enable the custom component birth element
    if constexpr (vcl::comp::HasCustomComponents<OutElemType>) {
        if (saveBirthIndicesInCustomComponent) {
            res.template addPerElementCustomComponent<ELEM_ID, uint>(ccname);
        }
    }

    for (const auto& [birthV, filter] :
         std::views::zip(m.template elements<ELEM_ID>(), elemFilterRng)) {
        if (filter) {
            uint v = res.template add<ELEM_ID>();
            // import all the components from the input mesh
            res.template element<ELEM_ID>(v).importFrom(birthV, false);
            // set the birth element
            if constexpr (vcl::comp::HasCustomComponents<OutElemType>) {
                if (saveBirthIndicesInCustomComponent) {
                    res.template element<ELEM_ID>(v)
                        .template customComponent<uint>(ccname) =
                        m.index(birthV);
                }
            }
        }
    }

    return res;
}

template<MeshConcept OutMeshType, uint ELEM_ID, MeshConcept InMeshType>
OutMeshType perElementMeshFilter(
    const InMeshType&                                                m,
    const std::function<bool(
        const typename InMeshType::template ElementType<ELEM_ID>&)>& elemFilter,
    bool saveBirthIndicesInCustomComponent = true)
{
    auto view =
        m.template elements<ELEM_ID>() | std::views::transform(elemFilter);

    return perElementMeshFilter<OutMeshType, ELEM_ID, InMeshType>(
        m, view, saveBirthIndicesInCustomComponent);
}

template<MeshConcept OutMeshType, uint ELEM_ID, MeshConcept InMeshType>
OutMeshType perElementMeshFilterWithVRefs(
    const InMeshType& m,
    Range auto&&      elemFilterRng,
    bool              saveBirthIndicesInCustomComponent = true)
{
    using InVertexType = InMeshType::VertexType;
    using OutElemType  = OutMeshType::template ElementType<ELEM_ID>;

    std::string ccname = "birth" + vcl::elementEnumString<ELEM_ID>();

    OutMeshType res;
    res.enableSameOptionalComponentsOf(m);

    // enable the custom component birthVertex
    if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
        if (saveBirthIndicesInCustomComponent) {
            res.template addPerVertexCustomComponent<uint>("birthVertex");
        }
    }

    // enable the custom component birth element
    if constexpr (vcl::comp::HasCustomComponents<OutElemType>) {
        if (saveBirthIndicesInCustomComponent) {
            res.template addPerElementCustomComponent<ELEM_ID, uint>(ccname);
        }
    }

    std::vector<uint> vertexMapping(m.vertexContainerSize(), UINT_NULL);

    for (const auto& [birthF, filter] :
         std::views::zip(m.template elements<ELEM_ID>(), elemFilterRng)) {
        if (filter) {
            std::vector<uint> verts(birthF.vertexNumber(), UINT_NULL);
            uint vi = 0; // incremented with vertices of the element
            // set all the vertex indices in the verts vector
            // two cases here:
            // - the ith vertex of the face has been already added, we need just
            //   to take its id in the out mesh from the vertexMapping vector
            // - the ith vertex of the face has not been added: we need to add
            //   it and import all its components, and update the
            //   vertexMappingVector
            for (const InVertexType* v : birthF.vertices()) {
                // the vertex has not already added
                if (vertexMapping[m.index(v)] == UINT_NULL) {
                    // add the vertex to the out mesh
                    uint ov = res.addVertex();
                    // import all the components from the input mesh
                    res.vertex(ov).importFrom(*v, false);
                    if constexpr (vcl::HasPerVertexCustomComponents<
                                      OutMeshType>) {
                        // set the birth vertex
                        if (saveBirthIndicesInCustomComponent) {
                            res.vertex(ov).template customComponent<uint>(
                                "birthVertex") = m.index(v);
                        }
                    }
                    vertexMapping[m.index(v)] = ov;
                    verts[vi]                 = ov;
                }
                else {
                    verts[vi] = vertexMapping[m.index(v)];
                }
                ++vi;
            }

            // now all the vertices of the elements are in the out mesh, we can
            // add the actual element
            uint f = res.template add<ELEM_ID>();
            // import all the components from the input mesh
            res.template element<ELEM_ID>(f).importFrom(birthF, false);

            if constexpr (OutElemType::VERTEX_NUMBER < 0) {
                res.template element<ELEM_ID>(f).resizeVertices(verts.size());
            }

            res.template element<ELEM_ID>(f).setVertices(verts);

            if constexpr (vcl::comp::HasCustomComponents<OutElemType>) {
                // set the birth elements
                if (saveBirthIndicesInCustomComponent) {
                    res.template element<ELEM_ID>(f)
                        .template customComponent<uint>(ccname) =
                        m.index(birthF);
                }
            }
        }
    }
    return res;
}

template<MeshConcept OutMeshType, uint ELEM_ID, MeshConcept InMeshType>
OutMeshType perElementMeshFilterWithVRefs(
    const InMeshType&                                                m,
    const std::function<bool(
        const typename InMeshType::template ElementType<ELEM_ID>&)>& elemFilter,
    bool saveBirthIndicesInCustomComponent = true)
{
    auto view =
        m.template elements<ELEM_ID>() | std::views::transform(elemFilter);

    return perElementMeshFilterWithVRefs<OutMeshType, ELEM_ID, InMeshType>(
        m, view, saveBirthIndicesInCustomComponent);
}

} // namespace detail

/**
 * @brief Generates and returns a new mesh that is composed of the vertices of
 * the input mesh `m` filtered using the `vertexFilter` function.
 *
 * Only the vertices for which the `vertexFilter` returns `true` will be put in
 * the output mesh. The order of the vertices in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `MeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `MeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] vertexFilter: a function that takes a vertex as input and returns
 * a boolean value that tells whether the vertex should be put in the output
 * mesh.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex CustomComponents component, will set a
 * per vertex custom component of type `uint` in the output mesh telling, for
 * each vertex, the index of its birth vertex in the input mesh. The name of the
 * custom component is `"birthVertex"`.
 *
 * @return A new Mesh created by filtering the vertices of the input mesh `m`.
 */
template<MeshConcept InMeshType, MeshConcept OutMeshType = InMeshType>
OutMeshType perVertexMeshFilter(
    const InMeshType& m,
    const std::function<bool(const typename InMeshType::VertexType&)>&
         vertexFilter,
    bool saveBirthIndicesInCustomComponent = true)
{
    return detail::
        perElementMeshFilter<OutMeshType, ElemId::VERTEX, InMeshType>(
            m, vertexFilter, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the vertices of
 * the input mesh `m` filtered using the `vertexFilterRng` range.
 *
 * Only the vertices having the corresponding boolean in `vertexFilterRng`
 * evaluated to `true` will be put in the output mesh. The order of the vertices
 * in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `MeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `MeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] vertexFilterRng: range of values that are evaluated as booleans,
 * one for each vertex of the input mesh. Its type must satisfy the `Range`
 * concept.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex CustomComponents component, will set a
 * per vertex custom component of type `uint` in the output mesh telling, for
 * each vertex, the index of its birth vertex in the input mesh. The name of the
 * custom component is `"birthVertex"`.
 *
 * @return A new Mesh created by filtering the vertices of the input mesh `m`.
 */
template<MeshConcept InMeshType, MeshConcept OutMeshType = InMeshType>
OutMeshType perVertexMeshFilter(
    const InMeshType& m,
    Range auto&&      vertexFilterRng,
    bool              saveBirthIndicesInCustomComponent = true)
{
    return detail::
        perElementMeshFilter<OutMeshType, ElemId::VERTEX, InMeshType>(
            m, vertexFilterRng, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the selected
 * vertices of the input mesh `m`.
 *
 * Only the selected vertices will be put in the output mesh. The order of the
 * vertices in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `MeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `MeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex CustomComponents component, will set a
 * per vertex custom component of type `uint` in the output mesh telling, for
 * each vertex, the index of its birth vertex in the input mesh. The name of the
 * custom component is `"birthVertex"`.
 *
 * @return A new Mesh created by filtering by selection the vertices of the
 * input mesh `m`.
 */
template<MeshConcept InMeshType, MeshConcept OutMeshType = InMeshType>
OutMeshType perVertexSelectionMeshFilter(
    const InMeshType& m,
    bool              saveBirthIndicesInCustomComponent = true)
{
    auto selView = m.vertices() | vcl::views::selection;

    return detail::
        perElementMeshFilter<OutMeshType, ElemId::VERTEX, InMeshType>(
            m, selView, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the faces of the
 * input mesh `m` filtered using the `faceFilter` function. Only vertices
 * belonging to the imported faces will be imported in the output mesh.
 *
 * Only the faces for which the `faceFilter` function returns  true` and their
 * vertices will be put in the output mesh. Only the order of the faces in the
 * output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `FaceMeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] faceFilter: a function that takes a face as input and returns a
 * boolean value that tells whether the face should be imported in the output
 * mesh or not.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex and/or per face CustomComponents
 * component, will set a per vertex/per face custom component of type `uint` in
 * the output mesh telling, for each vertex/face, the index of its birth
 * vertex/birth face in the input mesh. The names of the custom components are
 * `"birthVertex"` and `"birthFace"`.
 *
 * @return A new Mesh created by filtering the faces of the input mesh `m`.
 */
template<FaceMeshConcept InMeshType, FaceMeshConcept OutMeshType = InMeshType>
OutMeshType perFaceMeshFilter(
    const InMeshType&                                                m,
    const std::function<bool(const typename InMeshType::FaceType&)>& faceFilter,
    bool saveBirthIndicesInCustomComponent = true)
{
    return detail::
        perElementMeshFilterWithVRefs<OutMeshType, ElemId::FACE, InMeshType>(
            m, faceFilter, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the faces of the
 * input mesh `m` filtered using the `faceFilterRng` range. Only vertices
 * belonging to the imported faces will be imported in the output mesh.
 *
 * Only the faces having the corresponding boolean in `faceFilterRng` evaluated
 * to `true` and their vertices will be put in the output mesh. Only the order
 * of the faces in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `FaceMeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] faceFilterRng: range of values that are evaluated as booleans,
 * one for each face of the input mesh. Its type must satisfy the `Range`
 * concept.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex and/or per face CustomComponents
 * component, will set a per vertex/per face custom component of type `uint` in
 * the output mesh telling, for each vertex/face, the index of its birth
 * vertex/birth face in the input mesh. The names of the custom components are
 * `"birthVertex"` and `"birthFace"`.
 *
 * @return A new Mesh created by filtering the faces of the input mesh `m`.
 */
template<FaceMeshConcept InMeshType, FaceMeshConcept OutMeshType = InMeshType>
OutMeshType perFaceMeshFilter(
    const InMeshType& m,
    Range auto&&      faceFilterRng,
    bool              saveBirthIndicesInCustomComponent = true)
{
    return detail::
        perElementMeshFilterWithVRefs<OutMeshType, ElemId::FACE, InMeshType>(
            m, faceFilterRng, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the selected
 * faces of the input mesh `m`. Only vertices belonging to the imported faces
 * will be imported in the output mesh.
 *
 * Only the delected faces and their vertices will be put in the output mesh.
 * Only the order of the faces in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `FaceMeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex and/or per face CustomComponents
 * component, will set a per vertex/per face custom component of type `uint` in
 * the output mesh telling, for each vertex/face, the index of its birth
 * vertex/birth face in the input mesh. The names of the custom components are
 * `"birthVertex"` and `"birthFace"`.
 *
 * @return A new Mesh created by filtering the selected faces of the input mesh
 * `m`.
 */
template<FaceMeshConcept InMeshType, FaceMeshConcept OutMeshType = InMeshType>
OutMeshType perFaceSelectionMeshFilter(
    const InMeshType& m,
    bool              saveBirthIndicesInCustomComponent = true)
{
    auto selView = m.faces() | vcl::views::selection;

    return detail::
        perElementMeshFilterWithVRefs<OutMeshType, ElemId::FACE, InMeshType>(
            m, selView, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the edges of the
 * input mesh `m` filtered using the `edgeFilter` function. Only vertices
 * belonging to the imported edges will be imported in the output mesh.
 *
 * Only the edges for which the `edgeFilter` function returns  true` and their
 * vertices will be put in the output mesh. Only the order of the edges in the
 * output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `EdgeMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `EdgeMeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] edgeFilter: a function that takes a edge as input and returns a
 * boolean value that tells whether the edge should be imported in the output
 * mesh or not.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex and/or per edge CustomComponents
 * component, will set a per vertex/per edge custom component of type `uint` in
 * the output mesh telling, for each vertex/edge, the index of its birth
 * vertex/birth edge in the input mesh. The names of the custom components are
 * `"birthVertex"` and `"birthEdge"`.
 *
 * @return A new Mesh created by filtering the edges of the input mesh `m`.
 */
template<EdgeMeshConcept InMeshType, EdgeMeshConcept OutMeshType = InMeshType>
OutMeshType perEdgeMeshFilter(
    const InMeshType&                                                m,
    const std::function<bool(const typename InMeshType::EdgeType&)>& edgeFilter,
    bool saveBirthIndicesInCustomComponent = true)
{
    return detail::
        perElementMeshFilterWithVRefs<OutMeshType, ElemId::EDGE, InMeshType>(
            m, edgeFilter, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the edges of the
 * input mesh `m` filtered using the `edgeFilterRng` range. Only vertices
 * belonging to the imported edges will be imported in the output mesh.
 *
 * Only the edges having the corresponding boolean in `edgeFilterRng` evaluated
 * to `true` and their vertices will be put in the output mesh. Only the order
 * of the edges in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `EdgeMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `EdgeMeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] edgeFilterRng: range of values that are evaluated as booleans,
 * one for each edge of the input mesh. Its type must satisfy the `Range`
 * concept.
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex and/or per edge CustomComponents
 * component, will set a per vertex/per edge custom component of type `uint` in
 * the output mesh telling, for each vertex/edge, the index of its birth
 * vertex/birth edge in the input mesh. The names of the custom components are
 * `"birthVertex"` and `"birthEdge"`.
 *
 * @return A new Mesh created by filtering the edges of the input mesh `m`.
 */
template<EdgeMeshConcept InMeshType, EdgeMeshConcept OutMeshType = InMeshType>
OutMeshType perEdgeMeshFilter(
    const InMeshType& m,
    Range auto&&      edgeFilterRng,
    bool              saveBirthIndicesInCustomComponent = true)
{
    return detail::
        perElementMeshFilterWithVRefs<OutMeshType, ElemId::EDGE, InMeshType>(
            m, edgeFilterRng, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the selected
 * edges of the input mesh `m`. Only vertices belonging to the imported edges
 * will be imported in the output mesh.
 *
 * Only the delected edges and their vertices will be put in the output mesh.
 * Only the order of the edges in the output mesh is preserved.
 *
 * By default, the type of the output mesh will be the same of the input mesh
 * type.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `EdgeMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `EdgeMeshConcept`. By default, it is the same of the input mesh type.
 *
 * @param[in] m: input mesh
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex and/or per edge CustomComponents
 * component, will set a per vertex/per edge custom component of type `uint` in
 * the output mesh telling, for each vertex/edge, the index of its birth
 * vertex/birth edge in the input mesh. The names of the custom components are
 * `"birthVertex"` and `"birthEdge"`.
 *
 * @return A new Mesh created by filtering the selected edges of the input mesh
 * `m`.
 */
template<EdgeMeshConcept InMeshType, EdgeMeshConcept OutMeshType = InMeshType>
OutMeshType perEdgeSelectionMeshFilter(
    const InMeshType& m,
    bool              saveBirthIndicesInCustomComponent = true)
{
    auto selView = m.edges() | vcl::views::selection;

    return detail::
        perElementMeshFilterWithVRefs<OutMeshType, ElemId::EDGE, InMeshType>(
            m, selView, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the edges
 * computed from the face edges of the input mesh `m` filtered using the
 * `faceEdgeFilter` function. Only vertices belonging to the imported face edges
 * will be imported in the output mesh.
 *
 * Only the edges for which the `faceEdgeFilter` function returns  true` and
 * their vertices will be put in the output mesh.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `EdgeMeshConcept`.
 *
 * @param[in] m: input mesh
 * @param[in] faceEdgeFilter: a function that takes a face and an uint as input
 * and returns a boolean value that tells whether the edge (uint) of the face
 * should be "imported" in the output mesh or not.
 * @param[in] dontDuplicateEdges: if `true` (default), the output mesh will not
 * have duplicated edges (same vertices, also in different order).
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex CustomComponents component, will set a
 * per vertex custom component of type `uint` in the output mesh telling, for
 * each vertex, the index of its birth vertex in the input mesh. The names of
 * the custom component is `"birthVertex"`.
 *
 * @return A new Mesh created by filtering the face edges of the input mesh `m`.
 */
template<EdgeMeshConcept OutMeshType, FaceMeshConcept InMeshType>
OutMeshType perFaceEdgeMeshFilter(
    const InMeshType& m,
    const std::function<bool(const typename InMeshType::FaceType&, uint)>&
         faceEdgeFilter,
    bool dontDuplicateEdges                = true,
    bool saveBirthIndicesInCustomComponent = true)
{
    using InVertexType = InMeshType::VertexType;
    using OutEdgeType  = OutMeshType::EdgeType;

    OutMeshType res;
    // todo: enable only per vertex same optional components of m
    res.enableSameOptionalComponentsOf(m);

    // enable the custom component birthVertex
    if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
        if (saveBirthIndicesInCustomComponent) {
            res.template addPerVertexCustomComponent<uint>("birthVertex");
        }
    }

    std::vector<uint> vertexMapping(m.vertexContainerSize(), UINT_NULL);

    std::set<std::pair<uint, uint>, UnorderedPairComparator<uint>>
        unorderedEdges;

    for (const auto& f : m.faces()) {
        for (uint ei = 0; ei < f.vertexNumber(); ++ei) {
            if (faceEdgeFilter(f, ei)) {
                std::array<uint, 2> verts = {UINT_NULL, UINT_NULL};
                for (uint i = 0; i < 2; ++i) {
                    const InVertexType* v = f.vertexMod(ei + i);
                    if (vertexMapping[m.index(v)] == UINT_NULL) {
                        uint ov = res.addVertex();
                        res.vertex(ov).importFrom(*v, false);
                        if constexpr (vcl::HasPerVertexCustomComponents<
                                          OutMeshType>) {
                            if (saveBirthIndicesInCustomComponent) {
                                res.vertex(ov).template customComponent<uint>(
                                    "birthVertex") = m.index(v);
                            }
                        }
                        vertexMapping[m.index(v)] = ov;
                        verts[i]                  = ov;
                    }
                    else {
                        verts[i] = vertexMapping[m.index(v)];
                    }
                }

                std::pair<uint, uint> ep {verts[0], verts[1]};
                if (!dontDuplicateEdges || !unorderedEdges.contains(ep)) {
                    uint e = res.addEdge(verts[0], verts[1]);
                    unorderedEdges.insert(ep);
                }
            }
        }
    }

    return res;
}

/**
 * @brief Generates and returns a new mesh that is composed of the edges
 * computed from the face edges of the input mesh `m` filtered using the
 * `faceEdgeFilter` function. Only vertices belonging to the imported face edges
 * will be imported in the output mesh.
 *
 * Only the edges for which the `faceEdgeFilter` function returns  true` and
 * their vertices will be put in the output mesh.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `EdgeMeshConcept`.
 *
 * @param[in] m: input mesh
 * @param[in] faceEdgeFilter: a function that takes a face and an uint as input
 * and returns a boolean value that tells whether the edge (uint) of the face
 * should be "imported" in the output mesh or not.
 * @param[in] dontDuplicateEdges: if `true` (default), the output mesh will not
 * have duplicated edges (same vertices, also in different order).
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex CustomComponents component, will set a
 * per vertex custom component of type `uint` in the output mesh telling, for
 * each vertex, the index of its birth vertex in the input mesh. The names of
 * the custom component is `"birthVertex"`.
 *
 * @return A new Mesh created by filtering the face edges of the input mesh `m`.
 */
template<EdgeMeshConcept OutMeshType, FaceMeshConcept InMeshType>
OutMeshType perFaceEdgeMeshFilter(
    const InMeshType&                      m,
    const std::function<bool(uint, uint)>& faceEdgeFilter,
    bool                                   dontDuplicateEdges = true,
    bool saveBirthIndicesInCustomComponent                    = true)
{
    auto filter = [&](const typename InMeshType::FaceType& f, uint ei) {
        return faceEdgeFilter(m.index(f), ei);
    };

    return perFaceEdgeMeshFilter<OutMeshType, InMeshType>(
        m, filter, dontDuplicateEdges, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the edges
 * computed from the selected face edges of the input mesh `m`. Only vertices
 * belonging to the imported face edges will be imported in the output mesh.
 *
 * Only the selected edges and their vertices will be put in the output mesh.
 *
 * @tparam InMeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 * @tparam OutMeshType: type of the output mesh. It must satisfy the
 * `EdgeMeshConcept`.
 *
 * @param[in] m: input mesh
 * @param[in] dontDuplicateEdges: if `true` (default), the output mesh will not
 * have duplicated edges (same vertices, also in different order).
 * @param[in] saveBirthIndicesInCustomComponent: if `true` (default), and if the
 * output mesh type has the per vertex CustomComponents component, will set a
 * per vertex custom component of type `uint` in the output mesh telling, for
 * each vertex, the index of its birth vertex in the input mesh. The names of
 * the custom component is `"birthVertex"`.
 *
 * @return A new Mesh created by filtering the selected face edges of the input
 * mesh `m`.
 */
template<EdgeMeshConcept OutMeshType, FaceMeshConcept InMeshType>
OutMeshType perFaceEdgeSelectionMeshFilter(
    const InMeshType& m,
    bool              dontDuplicateEdges                = true,
    bool              saveBirthIndicesInCustomComponent = true)
{
    auto filter = [&](const typename InMeshType::FaceType& f, uint ei) {
        return f.edgeSelected(ei);
    };

    return perFaceEdgeMeshFilter<OutMeshType, InMeshType>(
        m, filter, dontDuplicateEdges, saveBirthIndicesInCustomComponent);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_FILTER_H
