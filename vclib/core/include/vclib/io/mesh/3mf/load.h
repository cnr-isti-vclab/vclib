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

template<MeshConcept MeshType, Matrix44Concept MatrixType>
void readMeshObject(
    std::vector<MeshType>&     m,
    std::vector<MeshInfo>&     info,
    const Lib3MF::PMeshObject& meshObject,
    const MatrixType&          transf)
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

    m.push_back(std::move(newMesh));
    info.push_back(std::move(newInfos));
}

template<MeshConcept MeshType, Matrix44Concept MatrixType>
void readComponentsObject(
    std::vector<MeshType>&           m,
    std::vector<MeshInfo>&           info,
    const Lib3MF::PComponentsObject& componentsObject,
    const MatrixType&                transf,
    const Lib3MF::PModel&            model)
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

        if (currObject->IsMeshObject()) {
            const Lib3MF::PMeshObject currMeshObject =
                model->GetMeshObjectByID(currObject->GetUniqueResourceID());

            readMeshObject(m, info, currMeshObject, componentTransform);
        }
        else if (currObject->IsComponentsObject()) {
            const Lib3MF::PComponentsObject currComponentsObject =
                model->GetComponentsObjectByID(
                    currObject->GetUniqueResourceID());

            readComponentsObject(
                m, info, currComponentsObject, componentTransform, model);
        }
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

            if (currObject->IsMeshObject()) {
                const Lib3MF::PMeshObject currMeshObject =
                    model->GetMeshObjectByID(currObject->GetUniqueResourceID());

                detail::readMeshObject(
                    m, loadedInfo, currMeshObject, transformMatrix);
            }
            else if (currObject->IsComponentsObject()) {
                const Lib3MF::PComponentsObject currComponentsObject =
                    model->GetComponentsObjectByID(
                        currObject->GetUniqueResourceID());

                detail::readComponentsObject(
                    m,
                    loadedInfo,
                    currComponentsObject,
                    transformMatrix,
                    model);
            }
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
