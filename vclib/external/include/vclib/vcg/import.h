// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_VCG_IMPORT_H
#define VCL_VCG_IMPORT_H

#include "type_mapping.h"

#include <vclib/mesh.h>

#include <vcg/complex/complex.h>

namespace vcl::vc {

namespace detail {

template<typename T>
concept ElementOrMeshConcept = vcl::ElementConcept<T> || vcl::MeshConcept<T>;

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

template<ElemId ELEM_ID, typename T, MeshConcept MeshType>
void addCustomComponentsIfTypeMatches(MeshType& mesh, auto& p)
{
    if (p._type == std::type_index(typeid(T))) {
        if constexpr (ELEM_ID < vcl::ElemId::ELEMENT_COUNT) {
            mesh.template addElementCustomComponent<
                ELEM_ID,
                typename TypeMapping<T>::type>(p._name);
        }
        else {
            mesh.template addCustomComponent<typename TypeMapping<T>::type>(
                p._name);
        }
    }
}

template<ElemId ELEM_ID, typename T, MeshConcept MeshType, typename VCGMeshType>
void addCustomComponentsOfTypeFromVCGMesh(
    MeshType&          mesh,
    const VCGMeshType& vcgMesh)
{
    using CustomAttrSet = std::set<typename VCGMeshType::PointerToAttribute>;

    const CustomAttrSet* ps = nullptr;

    switch (ELEM_ID) {
    case ElemId::VERTEX: ps = &vcgMesh.vert_attr; break;
    case ElemId::FACE: ps = &vcgMesh.face_attr; break;
    case ElemId::ELEMENT_COUNT: ps = &vcgMesh.mesh_attr;
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
    ElemId ELEM_ID,
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
    if constexpr (ELEM_ID == ElemId::ELEMENT_COUNT) {
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
    using PositionType = PositionType<MeshType>;

    mesh.reserveVertices(vcgMesh.VN());

    // add custom components that can be imported
    if constexpr (HasVertexCustomComponents<MeshType>) {
        // for each supported type, apply the lampda function that adds the
        // custom components of the type T that are in the vcgMesh
        vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
            [&mesh, &vcgMesh]<typename T>() {
                detail::addCustomComponentsOfTypeFromVCGMesh<ElemId::VERTEX, T>(
                    mesh, vcgMesh);
            });
    }

    std::vector<uint> vcgToVclVert(vcgMesh.vert.size(), UINT_NULL);

    // vertices
    for (uint i = 0; i < vcgMesh.vert.size(); i++) {
        if (!vcgMesh.vert[i].IsD()) {
            uint vi = mesh.addVertex(PositionType(
                vcgMesh.vert[i].P()[0],
                vcgMesh.vert[i].P()[1],
                vcgMesh.vert[i].P()[2]));

            vcgToVclVert[i] = vi;

            auto vertex = mesh.vertex(vi);

            // flags
            vertex.importFlagsFromVCGFormat(vcgMesh.vert[i].Flags());

            // normal
            if constexpr (HasVertexNormal<MeshType>) {
                using NormalType = VertexType<MeshType>::NormalType;
                if (vcg::tri::HasPerVertexNormal(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfVertexNormalOptional(mesh);
                    }
                    if (mesh.isVertexNormalAvailable()) {
                        detail::importNormal(vertex, vcgMesh.vert[i]);
                    }
                }
            }

            // color
            if constexpr (HasVertexColor<MeshType>) {
                if (vcg::tri::HasPerVertexColor(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfVertexColorOptional(mesh);
                    }
                    if (mesh.isVertexColorAvailable()) {
                        detail::importColor(vertex, vcgMesh.vert[i]);
                    }
                }
            }

            // quality
            if constexpr (HasVertexQuality<MeshType>) {
                if (vcg::tri::HasPerVertexQuality(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfVertexQualityOptional(mesh);
                    }
                    if (mesh.isVertexQualityAvailable()) {
                        vertex.quality() = vcgMesh.vert[i].Q();
                    }
                }
            }

            // texcoord
            if constexpr (HasVertexTexCoord<MeshType>) {
                using TexCoordType = VertexType<MeshType>::TexCoordType;
                if (vcg::tri::HasPerVertexTexCoord(vcgMesh)) {
                    if (enableOptionalComponents) {
                        vcl::enableIfVertexTexCoordOptional(mesh);
                    }
                    if (mesh.isVertexTexCoordAvailable()) {
                        vertex.texCoord() = TexCoordType(
                            vcgMesh.vert[i].T().U(), vcgMesh.vert[i].T().V());
                    }
                }
            }

            // custom components
            if constexpr (HasVertexCustomComponents<MeshType>) {
                // for each supported type, apply the lampda function that adds
                // the custom components of the type T that are in the vcgMesh
                vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
                    [&vertex, &vcgMesh, i]<typename T>() {
                        detail::importCustomComponentsOfTypeFromVCGMesh<
                            ElemId::VERTEX,
                            T>(vertex, vcgMesh, i);
                    });
            }
        }
    }

    // faces
    if constexpr (HasFaces<MeshType>) {
        using FaceType = FaceType<MeshType>;

        // add custom components that can be imported
        if constexpr (HasFaceCustomComponents<MeshType>) {
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

                auto face = mesh.face(fi);

                if constexpr (FaceType::VERTEX_COUNT < 0) {
                    face.resizeVertices(3);
                }
                for (uint j = 0; j < 3; ++j) {
                    uint vcgVi = vcg::tri::Index(vcgMesh, vcgMesh.face[i].V(j));
                    face.setVertex(j, vcgToVclVert[vcgVi]);
                }

                // flags
                face.importFlagsFromVCGFormat(vcgMesh.face[i].Flags());

                // normal
                if constexpr (HasFaceNormal<MeshType>) {
                    using NormalType = FaceType::NormalType;
                    if (vcg::tri::HasPerFaceNormal(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfFaceNormalOptional(mesh);
                        }
                        if (mesh.isFaceNormalAvailable()) {
                            detail::importNormal(face, vcgMesh.face[i]);
                        }
                    }
                }

                // color
                if constexpr (HasFaceColor<MeshType>) {
                    if (vcg::tri::HasPerFaceColor(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfFaceColorOptional(mesh);
                        }
                        if (mesh.isFaceColorAvailable()) {
                            detail::importColor(face, vcgMesh.face[i]);
                        }
                    }
                }

                // quality
                if constexpr (HasFaceQuality<MeshType>) {
                    if (vcg::tri::HasPerFaceQuality(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfFaceQualityOptional(mesh);
                        }
                        if (mesh.isFaceQualityAvailable()) {
                            face.quality() = vcgMesh.face[i].Q();
                        }
                    }
                }

                // wedge texcoords
                if constexpr (HasFaceWedgeTexCoords<MeshType>) {
                    if (vcg::tri::HasPerWedgeTexCoord(vcgMesh)) {
                        if (enableOptionalComponents) {
                            vcl::enableIfFaceWedgeTexCoordsOptional(mesh);
                        }
                        if (mesh.isFaceWedgeTexCoordsAvailable()) {
                            if constexpr (HasFaceMaterialIndex<MeshType>) {
                                if (enableOptionalComponents) {
                                    vcl::enableIfFaceMaterialIndexOptional(
                                        mesh);
                                }
                                if (mesh.isFaceMaterialIndexAvailable()) {
                                    face.materialIndex() =
                                        vcgMesh.face[i].WT(0).N();
                                }
                            }
                            for (uint j = 0; j < 3; ++j) {
                                face.wedge(j).texCoord().u() =
                                    vcgMesh.face[i].WT(j).U();
                                face.wedge(j).texCoord().v() =
                                    vcgMesh.face[i].WT(j).V();
                            }
                        }
                    }
                }

                // custom components
                if constexpr (HasFaceCustomComponents<MeshType>) {
                    // for each supported type, apply the lampda function that
                    // adds the custom components of the type T that are in the
                    // vcgMesh
                    vcl::ForEachType<detail::SupportedCustomComponentTypes>::
                        apply([&face, &vcgMesh, i]<typename T>() {
                            detail::importCustomComponentsOfTypeFromVCGMesh<
                                ElemId::FACE,
                                T>(face, vcgMesh, i);
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

    if constexpr (HasMaterials<MeshType>) {
        for (const auto& s : vcgMesh.textures) {
            Material m;
            m.baseColorTextureDescriptor().path() = s;
            mesh.pushMaterial(m);
        }
    }

    if constexpr (HasCustomComponents<MeshType>) {
        // for each supported type, apply the lampda function that adds the
        // custom components of the type T that are in the vcgMesh
        vcl::ForEachType<detail::SupportedCustomComponentTypes>::apply(
            [&mesh, &vcgMesh]<typename T>() {
                // ELEMENT_COUNT is used here to indicate the custom
                // components of the mesh
                detail::addCustomComponentsOfTypeFromVCGMesh<
                    ElemId::ELEMENT_COUNT,
                    T>(mesh, vcgMesh);

                detail::importCustomComponentsOfTypeFromVCGMesh<
                    ElemId::ELEMENT_COUNT,
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
