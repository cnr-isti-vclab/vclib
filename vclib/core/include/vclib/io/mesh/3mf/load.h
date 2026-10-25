// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_3MF_LOAD_H
#define VCL_IO_MESH_3MF_LOAD_H

#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>

#include <vclib/algorithms/mesh.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <lib3mf_implicit.hpp>
#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif

namespace vcl {

namespace detail {

template<MeshConcept MeshType, Matrix44Concept MatrixType>
void readObject(
    std::vector<MeshType>&,
    std::vector<MeshInfo>&,
    const Lib3MF::PObject&,
    const MatrixType&,
    const Lib3MF::PModel&,
    bool);

template<Matrix44Concept MatrixType>
MatrixType sTransformToVclMatrix(const Lib3MF::sTransform& m)
{
    MatrixType vclMatrix = MatrixType::Identity();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            vclMatrix(j, i) = m.m_Fields[i][j];
        }
    }

    return vclMatrix;
}

template<MeshConcept MeshType>
void read3mfVertices(
    MeshType&                             m,
    const std::vector<Lib3MF::sPosition>& vertices)
{
    if (!vertices.empty())
        vertexPositionsFromBuffer(
            m, vertices.data()->m_Coordinates, vertices.size());
}

template<MeshConcept MeshType>
void read3mfTriangles(
    MeshType&                             m,
    const std::vector<Lib3MF::sTriangle>& triangles)
{
    if (!triangles.empty())
        faceIndicesFromBuffer(m, triangles.data()->m_Indices, triangles.size());
}

template<MeshConcept MeshType>
void read3mfColors(
    MeshType&             m,
    const Lib3MF::sColor& meshColor,
    const uint&           faceID)
{
    const vcl::Color c = vcl::Color(
        meshColor.m_Red,
        meshColor.m_Green,
        meshColor.m_Blue,
        meshColor.m_Alpha);

    m.face(faceID).color() = c;
}

template<MeshConcept MeshType>
void readBaseMaterial(
    MeshType&             m,
    const uint&           uniqueResourceID,
    const uint&           propertyID,
    const Lib3MF::PModel& model)
{
}

template<MeshConcept MeshType>
void readFaceProperties(
    MeshType&             m,
    MeshInfo&             info,
    uint                  uniqueResourceID,
    uint                  propertyID,
    uint                  faceID,
    const Lib3MF::PModel& model)
{
    switch (model->GetPropertyTypeByID(uniqueResourceID)) {
    case Lib3MF::ePropertyType::NoPropertyType: break;
    case Lib3MF::ePropertyType::BaseMaterial:
        if constexpr (HasPerFaceColor<MeshType>) {
            enableIfPerFaceColorOptional(m);
            read3mfColors(
                m,
                model->GetBaseMaterialGroupByID(uniqueResourceID)
                    ->GetDisplayColor(propertyID),
                faceID);
            info.setPerFaceColor();
        }
        break;
    case Lib3MF::ePropertyType::TexCoord: break;
    case Lib3MF::ePropertyType::Colors:
        if constexpr (HasPerFaceColor<MeshType>) {
            enableIfPerFaceColorOptional(m);
            read3mfColors(
                m,
                model->GetColorGroupByID(uniqueResourceID)
                    ->GetColor(propertyID),
                faceID);
            info.setPerFaceColor();
        }
        break;
    case Lib3MF::ePropertyType::Composite: break;
    case Lib3MF::ePropertyType::Multi: break;
    }
}

template<MeshConcept MeshType, Matrix44Concept MatrixType>
void readMeshObject(
    std::vector<MeshType>&     m,
    std::vector<MeshInfo>&     info,
    const Lib3MF::PMeshObject& meshObject,
    const MatrixType&          transf,
    const Lib3MF::PModel&      model,
    bool                       enableOptionalComponents)
{
    MeshType newMesh;
    MeshInfo newInfos;

    std::vector<Lib3MF::sPosition> vertices;
    std::vector<Lib3MF::sTriangle> triangles;
    meshObject->GetVertices(vertices);
    meshObject->GetTriangleIndices(triangles);

    if constexpr (HasName<MeshType>) {
        newMesh.name() = meshObject->GetName();
    }

    read3mfVertices(newMesh, vertices);
    newInfos.setVertices();
    newInfos.setPerVertexPosition();

    if constexpr (HasFaces<MeshType>) {
        read3mfTriangles(newMesh, triangles);

        newInfos.setTriangleMesh();
        newInfos.setFaces();
        newInfos.setPerFaceVertexReferences();
    }

    if constexpr (HasTransformMatrix<MeshType>) {
        newMesh.transformMatrix() = transf;
    }
    else {
        // if the mesh does not have a transform matrix, apply the
        // transformation matrix to the vertices
        vcl::applyTransformMatrix(newMesh, transf);
    }

    if (enableOptionalComponents) {
        const uint                  nTriangles = meshObject->GetTriangleCount();
        Lib3MF::sTriangleProperties triangleProperties;
        uint                        uniqueResourceID, propertyID;
        const bool                  hasPerMeshProperties =
            meshObject->GetObjectLevelProperty(uniqueResourceID, propertyID);

        for (uint i = 0; i < nTriangles; i++) {
            meshObject->GetTriangleProperties(i, triangleProperties);

            if (hasPerMeshProperties || triangleProperties.m_ResourceID != 0) {
                if (triangleProperties.m_ResourceID != 0) {
                    const bool isBaseMaterialGroup =
                        model->GetPropertyTypeByID(
                            triangleProperties.m_ResourceID) ==
                        Lib3MF::ePropertyType::BaseMaterial;
                    const bool trianglePropertiesIDsAllEQ =
                        triangleProperties.m_PropertyIDs[0] ==
                            triangleProperties.m_PropertyIDs[1] &&
                        triangleProperties.m_PropertyIDs[0] ==
                            triangleProperties.m_PropertyIDs[2];

                    if (!isBaseMaterialGroup && !trianglePropertiesIDsAllEQ) {
                        // Use per-vertex properties
                    }
                    else if (trianglePropertiesIDsAllEQ) {
                        // Use per-face properties

                        readFaceProperties(
                            newMesh,
                            newInfos,
                            triangleProperties.m_ResourceID,
                            triangleProperties.m_PropertyIDs[0],
                            i,
                            model);
                    }
                    else if (hasPerMeshProperties) {
                        // Invalid per-face base material property

                        readFaceProperties(
                            newMesh,
                            newInfos,
                            uniqueResourceID,
                            propertyID,
                            i,
                            model);
                    }
                }
                else {
                    // Use per-mesh properties

                    readFaceProperties(
                        newMesh,
                        newInfos,
                        uniqueResourceID,
                        propertyID,
                        i,
                        model);
                }
            }
        }
    }

    m.push_back(std::move(newMesh));
    info.push_back(std::move(newInfos));
}

template<MeshConcept MeshType, Matrix44Concept MatrixType>
void readComponentsObject(
    std::vector<MeshType>&           m,
    std::vector<MeshInfo>&           info,
    const Lib3MF::PComponentsObject& componentsObject,
    const MatrixType&                transf,
    const Lib3MF::PModel&            model,
    bool                             enableOptionalComponents)
{
    const uint nComponents = componentsObject->GetComponentCount();

    for (uint i = 0; i < nComponents; i++) {
        const Lib3MF::PComponent currComponent =
            componentsObject->GetComponent(i);
        const Lib3MF::PObject currObject = currComponent->GetObjectResource();
        MatrixType            componentTransform;

        if (currComponent->HasTransform()) {
            componentTransform = transf * sTransformToVclMatrix<MatrixType>(
                                              currComponent->GetTransform());
        }
        else {
            componentTransform = transf;
        }

        readObject(
            m,
            info,
            currObject,
            componentTransform,
            model,
            enableOptionalComponents);
    }
}

template<MeshConcept MeshType, Matrix44Concept MatrixType>
void readObject(
    std::vector<MeshType>& m,
    std::vector<MeshInfo>& info,
    const Lib3MF::PObject& currObject,
    const MatrixType&      transf,
    const Lib3MF::PModel&  model,
    bool                   enableOptionalComponents)
{
    if (currObject->IsMeshObject()) {
        const Lib3MF::PMeshObject currMeshObject =
            model->GetMeshObjectByID(currObject->GetUniqueResourceID());

        readMeshObject(
            m, info, currMeshObject, transf, model, enableOptionalComponents);
    }
    else if (currObject->IsComponentsObject()) {
        const Lib3MF::PComponentsObject currComponentsObject =
            model->GetComponentsObjectByID(currObject->GetUniqueResourceID());

        readComponentsObject(
            m,
            info,
            currComponentsObject,
            transf,
            model,
            enableOptionalComponents);
    }
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load3mf(
    std::vector<MeshType>& m,
    const std::string&     filename,
    std::vector<MeshInfo>& loadedInfo,
    const LoadSettings&    settings = LoadSettings(),
    LogType&               log      = nullLogger)
{
    try {
        // entrypoint to the 3MF library
        Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();

        // Create a new model and a reader for 3MF files
        Lib3MF::PModel  model  = wrapper->CreateModel();
        Lib3MF::PReader reader = model->QueryReader("3mf");

        // Read the 3MF file into the model
        reader->ReadFromFile(filename);

        using ScalarType = MeshType::ScalarType;

        using Matrix44m = std::conditional_t<
            HasTransformMatrix<MeshType>,
            typename MeshType::TransformMatrixType,
            Matrix44<ScalarType>>;

        const Lib3MF::PBuildItemIterator buildIt = model->GetBuildItems();

        while (buildIt->MoveNext()) {
            const Lib3MF::PBuildItem currBuildItem = buildIt->GetCurrent();
            const Lib3MF::PObject    currObject =
                currBuildItem->GetObjectResource();
            Matrix44m transformMatrix = Matrix44m::Identity();

            if (currBuildItem->HasObjectTransform()) {
                transformMatrix =
                    transformMatrix * detail::sTransformToVclMatrix<Matrix44m>(
                                          currBuildItem->GetObjectTransform());
            }

            detail::readObject(
                m,
                loadedInfo,
                currObject,
                transformMatrix,
                model,
                settings.enableOptionalComponents);
        }
    }
    catch (Lib3MF::ELib3MFException& e) {
        throw CannotOpenFileException(
            "Failed to load 3MF file: " + filename + ":\n" + e.what());
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load3mf(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings = LoadSettings(),
    LogType&            log      = nullLogger)
{
    std::vector<MeshType> meshes;
    std::vector<MeshInfo> infos;
    load3mf(meshes, filename, infos, settings, log);

    if (meshes.empty()) {
        return;
    }

    m          = std::move(meshes.front());
    loadedInfo = std::move(infos.front());
    for (std::size_t i = 1; i < meshes.size(); ++i) {
        m.append(std::move(meshes[i]));
        // todo manage infos
    }
}

} // namespace vcl

#endif // VCL_IO_MESH_3MF_LOAD_H
