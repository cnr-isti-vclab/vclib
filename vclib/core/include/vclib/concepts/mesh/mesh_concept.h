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

#ifndef VCL_CONCEPTS_MESH_MESH_CONCEPT_H
#define VCL_CONCEPTS_MESH_MESH_CONCEPT_H

#include "containers/edge_container.h"
#include "containers/vertex_container.h"
#include "elements/element.h"
#include "per_face.h"

namespace vcl {

namespace mesh {

template<typename MeshType, uint ELEM_ID>
concept HasElementContainer =
    HasContainerOfElementPred<ELEM_ID, MeshType>::value;

template<typename MeshType, uint ELEM_ID, uint COMP_ID>
concept HasPerElementComponent =
    HasElementContainer<MeshType, ELEM_ID> &&
    comp::HasComponentOfType<
        typename ContainerOfElementType<ELEM_ID, MeshType>::ElementType,
        COMP_ID>;

template<typename MeshType, uint ELEM_ID, uint COMP_ID>
concept HasPerElementOptionalComponent =
    HasElementContainer<MeshType, ELEM_ID> &&
    comp::HasOptionalComponentOfType<
        typename ContainerOfElementType<ELEM_ID, MeshType>::ElementType,
        COMP_ID>;

} // namespace mesh

/**
 * @brief The Mesh Concept is evaluated to true when the type is a Mesh.
 *
 * A type T is a Mesh when it contains a VertexContainer, which is the only
 * container that is mandatory in a vcl::Mesh, and provides:
 *
 * - A type definition called Containers, which is a TypeWrapper that contains
 *   all the containers that the Mesh has.
 * - A type definition called Components, which is a TypeWrapper that contains
 *   all the components that the Mesh has.
 * - A method called clear() that clears the mesh.
 * - A method called isCompact() that returns a boolean indicating if the mesh
 *   is compact.
 * - A method called compact() that compacts the mesh.
 * - A method called enableSameOptionalComponentsOf() that enables the same
 *   optional components of another mesh.
 * - A method called importFrom() that imports the mesh from another mesh.
 * - A method called swap() that swaps the mesh with another mesh.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept MeshConcept =
    // the mesh has a VertexContainer
    mesh::HasVertexContainer<T> && requires (
                                       T&&                                obj,
                                       typename RemoveRef<T>::VertexType  v,
                                       typename RemoveRef<T>::VertexType& vR,
                                       typename RemoveRef<T>::VertexType* vP,
                                       std::vector<uint>                  vec) {
        // The mesh defines TypeWrappers for Containers and Components
        typename RemoveRef<T>::Containers;
        typename RemoveRef<T>::Components;

        // calling MeshType::ElementType<ElemId::VERTEX> does return the
        // VertexType
        requires std::same_as<
            decltype(v),
            typename RemoveRef<T>::template ElementType<ElemId::VERTEX>>;

        // can call hasContainerOf static function with element types
        requires RemoveRef<T>::template hasContainerOf<decltype(v)>();

        // can call hasContainerOf static function with element ids
        requires RemoveRef<T>::template hasContainerOf<ElemId::VERTEX>();

        // can call the hasPerElementComponent static function
        requires RemoveRef<T>::template hasPerElementComponent<
            ElemId::VERTEX,
            CompId::COORDINATE>();

        // constructors
        RemoveRef<T>();
        RemoveRef<T>(obj);

        // member functions
        { obj.isCompact() } -> std::same_as<bool>;
        { obj.index(v) } -> std::same_as<uint>;

        // generic per ElemId member functions
        {
            obj.template element<ElemId::VERTEX>(uint())
        } -> std::convertible_to<decltype(v)>;

        { obj.template number<ElemId::VERTEX>() } -> std::same_as<uint>;
        { obj.template containerSize<ElemId::VERTEX>() } -> std::same_as<uint>;
        { obj.template deletedNumber<ElemId::VERTEX>() } -> std::same_as<uint>;
        {
            obj.template compactIndices<ElemId::VERTEX>()
        } -> std::same_as<decltype(vec)>;

        { obj.template begin<ElemId::VERTEX>() } -> InputIterator<decltype(v)>;
        {
            obj.template begin<ElemId::VERTEX>(bool())
        } -> InputIterator<decltype(v)>;
        { obj.template end<ElemId::VERTEX>() } -> InputIterator<decltype(v)>;
        { obj.template elements<ElemId::VERTEX>() } -> RangeOf<decltype(v)>;
        {
            obj.template elements<ElemId::VERTEX>(bool())
        } -> RangeOf<decltype(v)>;

        // non const requirements
        requires vcl::IsConst<T> || requires {
            // member functions
            { obj.clear() } -> std::same_as<void>;
            { obj.compact() } -> std::same_as<void>;
            { obj.enableAllOptionalComponents() } -> std::same_as<void>;
            { obj.disableAllOptionalComponents() } -> std::same_as<void>;
            { obj.enableSameOptionalComponentsOf(obj) } -> std::same_as<void>;
            { obj.append(obj) } -> std::same_as<void>;
            { obj.importFrom(obj) } -> std::same_as<void>;
            { obj.swap(obj) } -> std::same_as<void>;
            { obj.deleteElement(v) } -> std::same_as<void>;
            { obj.deleteElement(vP) } -> std::same_as<void>;

            // assignment operator
            { obj = obj } -> std::same_as<T&>;

            // generic per ElemId member functions
            {
                obj.template element<ElemId::VERTEX>(uint())
            } -> std::same_as<decltype(vR)>;
            { obj.template add<ElemId::VERTEX>() } -> std::same_as<uint>;
            { obj.template add<ElemId::VERTEX>(uint()) } -> std::same_as<uint>;
            {
                obj.template clearElements<ElemId::VERTEX>()
            } -> std::same_as<void>;
            {
                obj.template resize<ElemId::VERTEX>(uint())
            } -> std::same_as<void>;
            {
                obj.template reserve<ElemId::VERTEX>(uint())
            } -> std::same_as<void>;
            {
                obj.template deleteElement<ElemId::VERTEX>(uint())
            } -> std::same_as<void>;
            {
                obj.template updateIndices<ElemId::VERTEX>(vec)
            } -> std::same_as<void>;

            {
                obj.template begin<ElemId::VERTEX>()
            } -> OutputIterator<decltype(v)>;
            {
                obj.template begin<ElemId::VERTEX>(bool())
            } -> OutputIterator<decltype(v)>;
            {
                obj.template end<ElemId::VERTEX>()
            } -> OutputIterator<decltype(v)>;
        };
    };

/**
 * @brief the ElementOrMesh Concept is evaluated to true when the type is either
 * a Mesh or an Element.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept ElementOrMeshConcept = MeshConcept<T> || ElementConcept<T>;

/**
 * @brief The EdgeMeshConcept is evaluated true if the type T is a Mesh (it
 * satisfies the @ref vcl::MeshConcept) and has an EdgeContainer.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept EdgeMeshConcept = MeshConcept<T> && mesh::HasEdgeContainer<T>;

/**
 * @brief The FaceMeshConcept is evaluated true if the type T is a Mesh (it
 * satisfies the @ref vcl::MeshConcept) and has a FaceContainer.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept FaceMeshConcept = MeshConcept<T> && mesh::HasFaceContainer<T>;

template<typename T>
concept TriangleMeshConcept = FaceMeshConcept<T> && HasTriangles<T>;

template<typename T>
concept QuadMeshConcept = FaceMeshConcept<T> && HasQuads<T>;

template<typename T>
concept PolygonMeshConcept = FaceMeshConcept<T> && HasPolygons<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_MESH_CONCEPT_H
