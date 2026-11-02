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

struct ResourcesMap
{
    std::unordered_map<uint, Lib3MF::PBaseMaterialGroup> baseMaterialGroupsMap;
    std::unordered_map<uint, Lib3MF::PColorGroup>        colorGroupsMap;
    std::unordered_map<uint, Lib3MF::PTexture2DGroup>    texture2DGroupsMap;
    std::unordered_map<uint, Lib3MF::PAttachment>        textureAttachmentsMap;
    std::unordered_map<uint, int>                        materialIndicesMap;
    std::unordered_map<uint, Lib3MF::ePropertyType>      propertyTypesMap;
};

struct MeshInfoFlags
{
    bool faceColorsFlag    = false;
    bool faceTexCoordsFlag = false;
};

template<MeshConcept MeshType>
bool canStoreFaceColors(MeshType& m, bool enableOptionalComponents)

{
    return isPerFaceColorAvailable(m) ||
           (enableOptionalComponents && enableIfPerFaceColorOptional(m));
}

template<MeshConcept MeshType>
bool canStoreFaceTexCoords(MeshType& m, bool enableOptionalComponents)
{
    return (isPerFaceWedgeTexCoordsAvailable(m) ||
            (enableOptionalComponents &&
             enableIfPerFaceWedgeTexCoordsOptional(m))) &&
           (isPerFaceMaterialIndexAvailable(m) ||
            (enableOptionalComponents &&
             enableIfPerFaceMaterialIndexOptional(m)));
}

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

vcl::Color inline sColorToVclColor(const Lib3MF::sColor& c)
{
    return vcl::Color(c.m_Red, c.m_Green, c.m_Blue, c.m_Alpha);
}

template<typename T>
concept VclTextureFilter =
    std::same_as<T, TextureDescriptor::MinificationFilter> ||
    std::same_as<T, TextureDescriptor::MagnificationFilter>;

template<VclTextureFilter TextureFilterType>
TextureFilterType eTextureFilterToVclFilter(const Lib3MF::eTextureFilter& tf)
{
    switch (tf) {
    case Lib3MF::eTextureFilter::Auto: return TextureFilterType::NONE;
    case Lib3MF::eTextureFilter::Linear: return TextureFilterType::LINEAR;
    case Lib3MF::eTextureFilter::Nearest: return TextureFilterType::NEAREST;
    }

    throw MalformedFileException("Invalid 3mf filter");
}

std::string inline textureExtension(const Lib3MF::eTextureType type)
{
    switch (type) {
    case Lib3MF::eTextureType::Unknown:
        throw MalformedFileException("Invalid 3mf texture file extension");
    case Lib3MF::eTextureType::PNG: return "png";
    case Lib3MF::eTextureType::JPEG: return "jpeg";
    }

    throw MalformedFileException("Invalid 3mf texture file extension");
}

TextureDescriptor::WrapMode inline eTextureTileStyleToVclWrapMode(
    const Lib3MF::eTextureTileStyle& ttt)
{
    switch (ttt) {
    case Lib3MF::eTextureTileStyle::Wrap:
        return TextureDescriptor::WrapMode::REPEAT;
    case Lib3MF::eTextureTileStyle::Mirror:
        return TextureDescriptor::WrapMode::MIRRORED_REPEAT;
    case Lib3MF::eTextureTileStyle::Clamp:
    case Lib3MF::eTextureTileStyle::NoTileStyle:
        return TextureDescriptor::WrapMode::CLAMP_TO_EDGE;
    }

    throw MalformedFileException("Invalid 3mf wrapping mode");
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
int read3mfTexture(
    MeshType&                  m,
    uint                       faceID,
    const Lib3MF::PTexture2D&  textureProperty,
    const Lib3MF::PAttachment& textureAttachment)
{
    Material                mat;
    vcl::TextureDescriptor& texture =
        mat.textureDescriptor(Material::TextureType::BASE_COLOR);

    std::string               uri = textureAttachment->GetPath();
    Lib3MF::eTextureTileStyle wrapU, wrapV;
    textureProperty->GetTileStyleUV(wrapU, wrapV);
    Lib3MF::eTextureFilter textureFilter = textureProperty->GetFilter();

    texture.path() = uri;
    texture.minFilter() =
        eTextureFilterToVclFilter<TextureDescriptor::MinificationFilter>(
            textureProperty->GetFilter());
    texture.magFilter() =
        eTextureFilterToVclFilter<TextureDescriptor::MagnificationFilter>(
            textureProperty->GetFilter());
    texture.wrapU() = eTextureTileStyleToVclWrapMode(wrapU);
    texture.wrapV() = eTextureTileStyleToVclWrapMode(wrapV);

    std::vector<unsigned char> buffer;
    textureAttachment->WriteToBuffer(buffer);
    int w = 0, h = 0;

    std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> pixels(
        stbi_load_from_memory(
            buffer.data(),
            static_cast<int>(buffer.size()),
            &w,
            &h,
            nullptr,
            STBI_rgb_alpha),
        stbi_image_free);

    if (!pixels) {
        throw MalformedFileException("Cannot decode 3MF texture");
    }

    Image timg = Image(pixels.get(), w, h);

    timg.colorSpace() =
        Material::textureTypeToColorSpace(Material::TextureType::BASE_COLOR);

    m.pushTextureImage(uri, timg);

    m.pushMaterial(mat);

    return m.materialCount() - 1;
}

template<MeshConcept MeshType>
bool readTriangleBaseMaterial(
    MeshType&             m,
    uint                  uniqueResourceID,
    uint                  propertyID[3],
    uint                  faceID,
    const Lib3MF::PModel& model,
    bool                  enableOptionalComponents,
    ResourcesMap&         resourcesMap)
{
    const bool trianglePropertiesIDsAllEQ =
        propertyID[0] == propertyID[1] && propertyID[0] == propertyID[2];

    auto& bmg = resourcesMap.baseMaterialGroupsMap[uniqueResourceID];
    if (!bmg) {
        bmg = model->GetBaseMaterialGroupByID(uniqueResourceID);
    }

    if (trianglePropertiesIDsAllEQ) {
        if constexpr (HasPerFaceColor<MeshType>) {
            if (canStoreFaceColors(m, enableOptionalComponents)) {
                m.face(faceID).color() =
                    sColorToVclColor(bmg->GetDisplayColor(propertyID[0]));

                return true;
            }
        }
    }

    return false;
}

template<MeshConcept MeshType>
bool readTriangleColors(
    MeshType&             m,
    uint                  uniqueResourceID,
    uint                  propertyID[3],
    uint                  faceID,
    const Lib3MF::PModel& model,
    bool                  enableOptionalComponents,
    ResourcesMap&         resourcesMap)
{
    const bool trianglePropertiesIDsAllEQ =
        propertyID[0] == propertyID[1] && propertyID[0] == propertyID[2];

    auto& cg = resourcesMap.colorGroupsMap[uniqueResourceID];
    if (!cg) {
        cg = model->GetColorGroupByID(uniqueResourceID);
    }

    if (trianglePropertiesIDsAllEQ) {
        if constexpr (HasPerFaceColor<MeshType>) {
            if (canStoreFaceColors(m, enableOptionalComponents)) {
                m.face(faceID).color() =
                    sColorToVclColor(cg->GetColor(propertyID[0]));

                return true;
            }
        }
    }

    return false;
}

template<MeshConcept MeshType>
bool readTriangleTexCoords(
    MeshType&             m,
    uint                  uniqueResourceID,
    uint                  propertyID[3],
    uint                  faceID,
    const Lib3MF::PModel& model,
    bool                  enableOptionalComponents,
    ResourcesMap&         resourcesMap)
{
    if constexpr (
        HasMaterials<MeshType> && HasPerFaceWedgeTexCoords<MeshType> &&
        HasPerFaceMaterialIndex<MeshType>) {
        if (canStoreFaceTexCoords(m, enableOptionalComponents)) {
            auto& tcg    = resourcesMap.texture2DGroupsMap[uniqueResourceID];
            auto& a      = resourcesMap.textureAttachmentsMap[uniqueResourceID];
            auto& mIndex = resourcesMap.materialIndicesMap[uniqueResourceID];
            if (!tcg) {
                tcg = model->GetTexture2DGroupByID(uniqueResourceID);
                a   = tcg->GetTexture2D()->GetAttachment();

                /*const std::string tempTextureFilePath =
                    "tempTexture." +
                    textureExtension(tcg->GetTexture2D()->GetContentType());
                a->WriteToFile(tempTextureFilePath);*/

                mIndex = read3mfTexture(m, faceID, tcg->GetTexture2D(), a);
            }

            using WedgeTexCoordType = MeshType::FaceType::WedgeTexCoordType;

            const auto& uv0 = tcg->GetTex2Coord(propertyID[0]);
            const auto& uv1 = tcg->GetTex2Coord(propertyID[1]);
            const auto& uv2 = tcg->GetTex2Coord(propertyID[2]);

            m.face(faceID).wedgeTexCoord(0) =
                WedgeTexCoordType(uv0.m_U, uv0.m_V);
            m.face(faceID).wedgeTexCoord(1) =
                WedgeTexCoordType(uv1.m_U, uv1.m_V);
            m.face(faceID).wedgeTexCoord(2) =
                WedgeTexCoordType(uv2.m_U, uv2.m_V);

            m.face(faceID).materialIndex() = mIndex;

            return true;
        }
    }

    return false;
}

template<MeshConcept MeshType>
void readTriangleProperties(
    MeshType&             m,
    uint                  uniqueResourceID,
    uint                  propertyID[3],
    uint                  faceID,
    const Lib3MF::PModel& model,
    bool                  enableOptionalComponents,
    ResourcesMap&         resourcesMap,
    MeshInfoFlags&        infoFlags)
{
    auto it = resourcesMap.propertyTypesMap.find(uniqueResourceID);
    if (it == resourcesMap.propertyTypesMap.end()) {
        it = resourcesMap.propertyTypesMap
                 .emplace(
                     uniqueResourceID,
                     model->GetPropertyTypeByID(uniqueResourceID))
                 .first;
    }

    switch (it->second) {
    case Lib3MF::ePropertyType::NoPropertyType: break;
    case Lib3MF::ePropertyType::BaseMaterial:
        infoFlags.faceColorsFlag = readTriangleBaseMaterial(
            m,
            uniqueResourceID,
            propertyID,
            faceID,
            model,
            enableOptionalComponents,
            resourcesMap);
        break;
    case Lib3MF::ePropertyType::TexCoord:
        infoFlags.faceTexCoordsFlag = readTriangleTexCoords(
            m,
            uniqueResourceID,
            propertyID,
            faceID,
            model,
            enableOptionalComponents,
            resourcesMap);
        break;
    case Lib3MF::ePropertyType::Colors:
        infoFlags.faceColorsFlag = readTriangleColors(
            m,
            uniqueResourceID,
            propertyID,
            faceID,
            model,
            enableOptionalComponents,
            resourcesMap);
        break;
    case Lib3MF::ePropertyType::Composite: break;
    case Lib3MF::ePropertyType::Multi: break;
    }
}

template<MeshConcept MeshType>
void readObjectLevelProperty(
    MeshType&             m,
    uint                  uniqueResourceID,
    uint                  propertyID,
    const Lib3MF::PModel& model,
    ResourcesMap&         resourcesMap)
{
    auto it = resourcesMap.propertyTypesMap.find(uniqueResourceID);
    if (it == resourcesMap.propertyTypesMap.end()) {
        it = resourcesMap.propertyTypesMap
                 .emplace(
                     uniqueResourceID,
                     model->GetPropertyTypeByID(uniqueResourceID))
                 .first;
    }

    switch (it->second) {
    case Lib3MF::ePropertyType::NoPropertyType: break;
    case Lib3MF::ePropertyType::BaseMaterial:
        if constexpr (HasColor<MeshType>) {
            auto mat = model->GetBaseMaterialGroupByID(uniqueResourceID);
            resourcesMap.baseMaterialGroupsMap.insert({uniqueResourceID, mat});
            m.color() = sColorToVclColor(mat->GetDisplayColor(propertyID));
        }
        break;
    case Lib3MF::ePropertyType::TexCoord: break;
    case Lib3MF::ePropertyType::Colors:
        if constexpr (HasColor<MeshType>) {
            auto col = model->GetColorGroupByID(uniqueResourceID);
            resourcesMap.colorGroupsMap.insert({uniqueResourceID, col});
            m.color() = sColorToVclColor(col->GetColor(propertyID));
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

    if constexpr (HasName<MeshType>) {
        newMesh.name() = meshObject->GetName();
    }

    std::vector<Lib3MF::sPosition> vertices;
    std::vector<Lib3MF::sTriangle> triangles;
    meshObject->GetVertices(vertices);
    meshObject->GetTriangleIndices(triangles);

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

    ResourcesMap  resourcesMap;
    MeshInfoFlags infoFlags;
    uint          uniqueResourceID, propertyID;
    const bool    hasObjectLevelProperty =
        meshObject->GetObjectLevelProperty(uniqueResourceID, propertyID);

    if (hasObjectLevelProperty) {
        readObjectLevelProperty(
            newMesh, uniqueResourceID, propertyID, model, resourcesMap);
    }

    if constexpr (HasFaces<MeshType>) {
        bool                        faceColorFlag = false;
        const uint                  nTriangles = meshObject->GetTriangleCount();
        Lib3MF::sTriangleProperties triangleProperties;

        for (uint i = 0; i < nTriangles; i++) {
            meshObject->GetTriangleProperties(i, triangleProperties);

            if (triangleProperties.m_ResourceID != 0) {
                readTriangleProperties(
                    newMesh,
                    triangleProperties.m_ResourceID,
                    triangleProperties.m_PropertyIDs,
                    i,
                    model,
                    enableOptionalComponents,
                    resourcesMap,
                    infoFlags);
            }
        }

        if (infoFlags.faceColorsFlag)
            newInfos.setPerFaceColor();
        if (infoFlags.faceTexCoordsFlag) {
            newInfos.setPerFaceMaterialIndex();
            newInfos.setPerFaceWedgeTexCoords();
        }
    }

    m.push_back(std::move(newMesh));
    info.push_back(std::move(newInfos));
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
        // recursive call: for each component, read the corresponding object and
        // apply the transformation matrix to it
        const Lib3MF::PComponentsObject currComps =
            model->GetComponentsObjectByID(currObject->GetUniqueResourceID());

        const uint nComponents = currComps->GetComponentCount();

        for (uint i = 0; i < nComponents; i++) {
            const Lib3MF::PComponent currComp = currComps->GetComponent(i);
            const Lib3MF::PObject    currObj  = currComp->GetObjectResource();
            MatrixType               componentTransform;

            if (currComp->HasTransform()) {
                componentTransform = transf * sTransformToVclMatrix<MatrixType>(
                                                  currComp->GetTransform());
            }
            else {
                componentTransform = transf;
            }

            readObject(
                m,
                info,
                currObj,
                componentTransform,
                model,
                enableOptionalComponents);
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
