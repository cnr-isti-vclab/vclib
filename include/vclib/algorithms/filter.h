/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_ALGORITHMS_FILTER_H
#define VCL_ALGORITHMS_FILTER_H

#include <vclib/mesh/requirements.h>
#include <vclib/views.h>

namespace vcl {

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
    const InMeshType&                                             m,
    std::function<bool(const typename InMeshType::VertexType&)>&& vertexFilter,
    bool saveBirthIndicesInCustomComponent = true)
{
    auto view = m.vertices() | std::views::transform(vertexFilter);

    return perVertexMeshFilter<InMeshType, OutMeshType>(
        m, view, saveBirthIndicesInCustomComponent);
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
    OutMeshType res;
    res.enableSameOptionalComponentsOf(m);

    // enable the custom component birthVertex
    if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
        if (saveBirthIndicesInCustomComponent) {
            res.template addPerVertexCustomComponent<uint>("birthVertex");
        }
    }

    for (const auto& [birthV, filter] :
         std::views::zip(m.vertices(), vertexFilterRng))
    {
        if (filter) {
            uint v = res.addVertex();
            // import all the components from the input mesh
            res.vertex(v).importFrom(birthV);
            // set the birth vertex
            if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
                if (saveBirthIndicesInCustomComponent) {
                    res.vertex(v).template customComponent<uint>(
                        "birthVertex") = m.index(birthV);
                }
            }
        }
    }

    return res;
}

/**
 * @brief Generates and returns a new mesh that is composed of the faces of the
 * input mesh `m` filtered using the `faceFilter` function. Only vertices
 * belonging to the imported faces will be imported in the output mesh.
 *
 * Only the faces for which the `faceFilter` function returns  true` and their
 * vertices will be put in the output mesh. The order of the faces and vertices
 * in the output mesh is preserved.
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
    const InMeshType&                                           m,
    std::function<bool(const typename InMeshType::FaceType&)>&& faceFilter,
    bool saveBirthIndicesInCustomComponent = true)
{
    auto view = m.faces() | std::views::transform(faceFilter);

    return perFaceMeshFilter<InMeshType, OutMeshType>(
        m, view, saveBirthIndicesInCustomComponent);
}

/**
 * @brief Generates and returns a new mesh that is composed of the faces of the
 * input mesh `m` filtered using the `faceFilterRng` range. Only vertices
 * belonging to the imported faces will be imported in the output mesh.
 *
 * Only the faces having the corresponding boolean in `faceFilterRng` evaluated
 * to `true` and their vertices will be put in the output mesh. The order of the
 * faces and vertices in the output mesh is preserved.
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
    using InVertexType = InMeshType::VertexType;

    OutMeshType res;
    res.enableSameOptionalComponentsOf(m);

    // enable the custom component birthVertex
    if constexpr (vcl::HasPerVertexCustomComponents<OutMeshType>) {
        if (saveBirthIndicesInCustomComponent) {
            res.template addPerVertexCustomComponent<uint>("birthVertex");
        }
    }

    // enable the custom component birthFace
    if constexpr (vcl::HasPerFaceCustomComponents<OutMeshType>) {
        if (saveBirthIndicesInCustomComponent) {
            res.template addPerFaceCustomComponent<uint>("birthFace");
        }
    }

    std::vector<uint> vertexMapping(m.vertexContainerSize(), UINT_NULL);

    for (const auto& [birthF, filter] :
         std::views::zip(m.faces(), faceFilterRng))
    {
        if (filter) {
            std::vector<uint> verts(birthF.vertexNumber(), UINT_NULL);
            uint              vi = 0; // incremented with vertices of the face
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
                    res.vertex(ov).importFrom(*v);
                    if constexpr (vcl::HasPerVertexCustomComponents<
                                      OutMeshType>)
                    {
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

            // now all the vertices of the face are in the out mesh, we can add
            // the actual face
            uint f = res.addFace(verts);
            // import all the components from the input mesh
            res.face(f).importFrom(birthF);
            if constexpr (vcl::HasPerFaceCustomComponents<OutMeshType>) {
                // set the birth face
                if (saveBirthIndicesInCustomComponent) {
                    res.face(f).template customComponent<uint>("birthFace") =
                        m.index(birthF);
                }
            }
        }
    }
    return res;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_FILTER_H
