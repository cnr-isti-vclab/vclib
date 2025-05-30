/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_VCG_IMPORT_H
#define VCL_VCG_IMPORT_H

#include "type_mapping.h"

#include <vclib/mesh/requirements.h>

#include <vcg/complex/complex.h>

namespace vcl::vc {

namespace detail {

// all the custom components of these types will be imported
using SupportedCustomComponentTypes = TypeWrapper<
    int,
    float,
    double,
    vcg::Point2i,
    vcg::Point2f,
    vcg::Point2d,
    vcg::Point3i,
    vcg::Point3f,
    vcg::Point3d,
    vcg::Point4i,
    vcg::Point4f,
    vcg::Point4d>;

template<ElementConcept VCLElem, typename VCGElem>
void importNormal(VCLElem& vclElem, const VCGElem& vcgElem)
{
    using NormalType = VCLElem::NormalType;
    vclElem.normal() =
        NormalType(vcgElem.N()[0], vcgElem.N()[1], vcgElem.N()[2]);
}

template<ElementConcept VCLElem, typename VCGElem>
void importColor(VCLElem& vclElem, const VCGElem& vcgElem)
{
    vclElem.color() = vcl::Color(
        vcgElem.C()[0], vcgElem.C()[1], vcgElem.C()[2], vcgElem.C()[3]);
}

template<uint ELEM_ID, typename T, MeshConcept MeshType>
void addCustomComponentsIfTypeMatches(MeshType& mesh, auto& p)
{
    if (p._type == std::type_index(typeid(T))) {
        if constexpr (ELEM_ID < vcl::ElemId::ELEMENTS_NUMBER) {
            mesh.template addPerElementCustomComponent<
                ELEM_ID,
                typename TypeMapping<T>::type>(p._name);
        }
        else {
            mesh.template addCustomComponent<typename TypeMapping<T>::type>(
                p._name);
        }
    }
}

template<uint ELEM_ID, typename T, MeshConcept MeshType, typename VCGMeshType>
void addCustomComponentsOfTypeFromVCGMesh(
    MeshType&          mesh,
    const VCGMeshType& vcgMesh)
{
    using CustomAttrSet = std::set<typename VCGMeshType::PointerToAttribute>;

    const CustomAttrSet* ps = nullptr;

    switch (ELEM_ID) {
    case ElemId::VERTEX: ps = &vcgMesh.vert_attr; break;
    case ElemId::FACE: ps = &vcgMesh.face_attr; break;
    case ElemId::ELEMENTS_NUMBER: ps = &vcgMesh.mesh_attr;
    default: break;
    }

    if (ps) {
        for (auto& p : *ps) {
            addCustomComponentsIfTypeMatches<ELEM_ID, T>(mesh, p);
        }
    }
}

template<typename T, ElementOrMeshConcept ElementType>
void importCustomComponent(
    ElementType&       el,
    auto&              h,
    uint               elemIndex,
    const std::string& name)
{
    el.template customComponent<typename TypeMapping<T>::type>(name) =
        fromVCG(h[elemIndex]);
}

template<
    uint ELEM_ID,
    typename T,
    ElementOrMeshConcept ElementType,
    typename VCGMeshType>
void importCustomComponentsOfTypeFromVCGMesh(
    ElementType&       el,
    const VCGMeshType& vcgMesh,
    uint               elemIndex)
{
    if constexpr (ELEM_ID == ElemId::VERTEX) {
        for (auto& p : vcgMesh.vert_attr) {
            if (p._type == std::type_index(typeid(T))) {
                const auto& h = vcg::tri::Allocator<VCGMeshType>::
                    template FindPerVertexAttribute<T>(vcgMesh, p._name);

                importCustomComponent<T>(el, h, elemIndex, p._name);
            }
        }
    }

    if constexpr (ELEM_ID == ElemId::FACE) {
        for (auto& p : vcgMesh.face_attr) {
            if (p._type == std::type_index(typeid(T))) {
                const auto& h = vcg::tri::Allocator<VCGMeshType>::
                    template FindPerFaceAttribute<T>(vcgMesh, p._name);
                importCustomComponent<T>(el, h, elemIndex, p._name);
            }
        }
    }

    // Here el is the mesh!
    if constexpr (ELEM_ID == ElemId::ELEMENTS_NUMBER) {
        for (auto& p : vcgMesh.mesh_attr) {
            if (p._type == std::type_index(typeid(T))) {
                const auto& h = vcg::tri::Allocator<VCGMeshType>::
                    template FindPerMeshAttribute<T>(vcgMesh, p._name);

                el.template customComponent<typename TypeMapping<T>::type>(
                    p._name) = fromVCG(h());
            }
        }
    }
}

} // namespace detail

template<MeshConcept MeshType, typename VCGMeshType>
void importMeshFromVCGMesh(
    MeshType&          mesh,
    const VCGMeshType& vcgMesh,
    bool               enableOptionalComponents = true)
{
    using PositionType = MeshType::VertexType::PositionType;

    mesh.reserveVertices(vcgMesh.VN());

    // add custom components that can be imported
    if (HasPerVertexCustomComponents<MeshType>) {
        // for each supported type, apply the lampda function that adds the
        // custom components of the type T that are in the vcgMesh
        vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
            [&mesh, &vcgMesh]<typename T>() {
                detail::addCustomComponentsOfTypeFromVCGMesh<ElemId::VERTEX, T>(
                    mesh, vcgMesh);
            });
    }

    // vertices
    for (uint i = 0; i < vcgMesh.vert.size(); i++) {
        if (!vcgMesh.vert[i].IsD()) {
            uint vi = mesh.addVertex(PositionType(
                vcgMesh.vert[i].P()[0],
                vcgMesh.vert[i].P()[1],
                vcgMesh.vert[i].P()[2]));

            auto& vertex = mesh.vertex(vi);

            // flags
            vertex.importFlagsFromVCGFormat(vcgMesh.vert[i].Flags());

            // normal
            if constexpr (HasPerVertexNormal<MeshType>) {
                using NormalType = MeshType::VertexType::NormalType;
                if (vcg::tri::HasPerVertexNormal(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfPerVertexNormalOptional(mesh);
                    }
                    if (isPerVertexNormalAvailable(mesh)) {
                        detail::importNormal(vertex, vcgMesh.vert[i]);
                    }
                }
            }

            // color
            if constexpr (HasPerVertexColor<MeshType>) {
                if (vcg::tri::HasPerVertexColor(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfPerVertexColorOptional(mesh);
                    }
                    if (isPerVertexColorAvailable(mesh)) {
                        detail::importColor(vertex, vcgMesh.vert[i]);
                    }
                }
            }

            // quality
            if constexpr (HasPerVertexQuality<MeshType>) {
                if (vcg::tri::HasPerVertexQuality(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfPerVertexQualityOptional(mesh);
                    }
                    if (isPerVertexQualityAvailable(mesh)) {
                        vertex.quality() = vcgMesh.vert[i].Q();
                    }
                }
            }

            // texcoord
            if constexpr (HasPerVertexTexCoord<MeshType>) {
                using TexCoordType = MeshType::VertexType::TexCoordType;
                if (vcg::tri::HasPerVertexTexCoord(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfPerVertexTexCoordOptional(mesh);
                    }
                    if (isPerVertexTexCoordAvailable(mesh)) {
                        vertex.texCoord() = TexCoordType(
                            vcgMesh.vert[i].T().U(), vcgMesh.vert[i].T().V());
                    }
                }
            }

            // custom components
            if constexpr (HasPerVertexCustomComponents<MeshType>) {
                // for each supported type, apply the lampda function that adds
                // the custom components of the type T that are in the vcgMesh
                vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
                    [&vertex, &vcgMesh, vi]<typename T>() {
                        detail::importCustomComponentsOfTypeFromVCGMesh<
                            ElemId::VERTEX,
                            T>(vertex, vcgMesh, vi);
                    });
            }
        }
    }

    // faces
    if constexpr (HasFaces<MeshType>) {
        using FaceType = MeshType::FaceType;

        // add custom components that can be imported
        if (HasPerFaceCustomComponents<MeshType>) {
            // for each supported type, apply the lampda function that adds the
            // custom components of the type T that are in the vcgMesh
            vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
                [&mesh, &vcgMesh]<typename T>() {
                    detail::
                        addCustomComponentsOfTypeFromVCGMesh<ElemId::FACE, T>(
                            mesh, vcgMesh);
                });
        }

        for (uint i = 0; i < vcgMesh.face.size(); ++i) {
            if (!vcgMesh.face[i].IsD()) {
                uint fi = mesh.addFace();

                auto& face = mesh.face(fi);

                if constexpr (FaceType::VERTEX_NUMBER < 0) {
                    face.resizeVertices(3);
                }
                for (uint j = 0; j < 3; ++j) {
                    uint vi = vcg::tri::Index(vcgMesh, vcgMesh.face[i].V(j));
                    face.setVertex(j, vi);
                }

                // flags
                face.importFlagsFromVCGFormat(vcgMesh.face[i].Flags());

                // normal
                if constexpr (HasPerFaceNormal<MeshType>) {
                    using NormalType = MeshType::FaceType::NormalType;
                    if (vcg::tri::HasPerFaceNormal(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfPerFaceNormalOptional(mesh);
                        }
                        if (isPerFaceNormalAvailable(mesh)) {
                            detail::importNormal(face, vcgMesh.face[i]);
                        }
                    }
                }

                // color
                if constexpr (HasPerFaceColor<MeshType>) {
                    if (vcg::tri::HasPerFaceColor(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfPerFaceColorOptional(mesh);
                        }
                        if (isPerFaceColorAvailable(mesh)) {
                            detail::importColor(face, vcgMesh.face[i]);
                        }
                    }
                }

                // quality
                if constexpr (HasPerFaceQuality<MeshType>) {
                    if (vcg::tri::HasPerFaceQuality(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfPerFaceQualityOptional(mesh);
                        }
                        if (isPerFaceQualityAvailable(mesh)) {
                            face.quality() = vcgMesh.face[i].Q();
                        }
                    }
                }

                // wedge texcoords
                if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                    using WTType = MeshType::FaceType::WedgeTexCoordType;
                    if (vcg::tri::HasPerWedgeTexCoord(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfPerFaceWedgeTexCoordsOptional(mesh);
                        }
                        if (isPerFaceWedgeTexCoordsAvailable(mesh)) {
                            face.textureIndex() = vcgMesh.face[i].WT(0).N();
                            for (uint j = 0; j < 3; ++j) {
                                face.wedgeTexCoord(j) = WTType(
                                    vcgMesh.face[i].WT(j).U(),
                                    vcgMesh.face[i].WT(j).V());
                            }
                        }
                    }
                }

                // custom components
                if constexpr (HasPerVertexCustomComponents<MeshType>) {
                    // for each supported type, apply the lampda function that
                    // adds the custom components of the type T that are in the
                    // vcgMesh
                    vcl::ForEachType<detail::SupportedCustomComponentTypes>::
                        apply([&face, &vcgMesh, fi]<typename T>() {
                            detail::importCustomComponentsOfTypeFromVCGMesh<
                                ElemId::FACE,
                                T>(face, vcgMesh, fi);
                        });
                }
            }
        }
    }

    if constexpr (HasBoundingBox<MeshType>) {
        using BoundingBoxType = MeshType::BoundingBoxType;
        using PointType       = typename BoundingBoxType::PointType;

        mesh.boundingBox().min() = PointType(
            vcgMesh.bbox.min.X(), vcgMesh.bbox.min.Y(), vcgMesh.bbox.min.Z());
        mesh.boundingBox().max() = PointType(
            vcgMesh.bbox.max.X(), vcgMesh.bbox.max.Y(), vcgMesh.bbox.max.Z());
    }

    if constexpr (HasTexturePaths<MeshType>) {
        for (const auto& s : vcgMesh.textures) {
            mesh.pushTexturePath(s);
        }
    }

    if constexpr (HasCustomComponents<MeshType>) {
        // for each supported type, apply the lampda function that adds the
        // custom components of the type T that are in the vcgMesh
        vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
            [&mesh, &vcgMesh]<typename T>() {
                // ELEMENTS_NUMBER is used here to indicate the custom
                // components of the mesh
                detail::addCustomComponentsOfTypeFromVCGMesh<
                    ElemId::ELEMENTS_NUMBER,
                    T>(mesh, vcgMesh);

                detail::importCustomComponentsOfTypeFromVCGMesh<
                    ElemId::ELEMENTS_NUMBER,
                    T>(mesh, vcgMesh, 0);
            });
    }
}

template<MeshConcept MeshType, typename VCGMeshType>
MeshType meshFromVCGMesh(const VCGMeshType& vcgMesh)
{
    MeshType mesh;

    importMeshFromVCGMesh(mesh, vcgMesh);

    return mesh;
}

} // namespace vcl::vc

#endif // VCL_VCG_IMPORT_H
