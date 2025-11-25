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

#ifndef VCL_IO_MESH_GLTF_DETAIL_LOAD_MESH_H
#define VCL_IO_MESH_GLTF_DETAIL_LOAD_MESH_H

#include <vclib/io/mesh/settings.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>
#include <vclib/space/complex.h>
#include <vclib/space/core.h>

#include <tiny_gltf.h>

#include <regex>

namespace vcl::detail {

enum class GltfAttrType { POSITION, NORMAL, COLOR_0, TEXCOORD_0, TANGENT, INDICES };
inline const std::array<std::string, 5> GLTF_ATTR_STR {
    "POSITION",
    "NORMAL",
    "COLOR_0",
    "TEXCOORD_0",
    "TANGENT"};

template<MeshConcept MeshType>
int loadGltfPrimitiveMaterial(
    MeshType&                  m,
    const tinygltf::Model&     model,
    const tinygltf::Primitive& p)
{
    int idx = -1;

    if (p.material >= 0) {
        vcl::Color          baseColor, emissiveColor;
        Material::AlphaMode alphaMode;
        double metallic, roughness, alphaCutoff, normalScale, occlusionStrength;
        bool   doubleSided;
        int    baseColorTextureId, metallicRoughnessTextureId, normalTextureId,
            occlusionTextureId, emissiveTextureId;
        const tinygltf::Material& mat = model.materials[p.material];

        std::string matName = mat.name;

        // baseColorFactor
        const std::vector<double>& vc =
            mat.pbrMetallicRoughness.baseColorFactor; // has default value
        for (uint i = 0; i < 4; i++)
            baseColor[i] = vc[i] * 255.0;

        // baseColorTexture
        baseColorTextureId =
            mat.pbrMetallicRoughness.baseColorTexture
                .index; // get the id of the texture, -1 if not present

        // metallicFactor
        metallic = mat.pbrMetallicRoughness.metallicFactor; // has default value

        // roughnessFactor
        roughness =
            mat.pbrMetallicRoughness.roughnessFactor; // has default value

        metallicRoughnessTextureId =
            mat.pbrMetallicRoughness.metallicRoughnessTexture.index;

        // emissiveFactor
        const std::vector<double>& emissiveFactor =
            mat.emissiveFactor; // has default value
        for (uint i = 0; i < 3; i++)
            emissiveColor[i] = emissiveFactor[i] * 255.0;

        // emissiveTexture
        emissiveTextureId = mat.emissiveTexture.index;

        // normalTexture
        normalTextureId = mat.normalTexture.index;

        // occlusionTexture
        occlusionTextureId = mat.occlusionTexture.index;

        // doubleSided
        doubleSided = mat.doubleSided; // has default value

        // alphaMode
        if (mat.alphaMode == "MASK")
            alphaMode = Material::AlphaMode::ALPHA_MASK;
        else if (mat.alphaMode == "BLEND")
            alphaMode = Material::AlphaMode::ALPHA_BLEND;
        else
            alphaMode = Material::AlphaMode::ALPHA_OPAQUE; // has default value

        // alphaCutoff
        alphaCutoff = mat.alphaCutoff; // has default value

        // normalScale
        normalScale = mat.normalTexture.scale;

        // occlusionStrength
        occlusionStrength = mat.occlusionTexture.strength;

        // function to load a texture in a material
        auto loadTextureInMaterial =
            [&](Material& mat, int textureId, Material::TextureType type) {
                if (textureId != -1) {
                    const tinygltf::Image& img =
                        model.images[model.textures[textureId].source];
                    // add the path of the texture to the mesh
                    std::string uri = img.uri;
                    uri = std::regex_replace(uri, std::regex("\\%20"), " ");
                    if (uri.empty()) {
                        uri = "texture_" + std::to_string(textureId);
                    }

                    vcl::Texture& texture = mat.texture(type);

                    texture.path() = uri;

                    texture.colorSpace() = Material::isSRGBTexture(type) ?
                                               Texture::ColorSpace::SRGB :
                                               Texture::ColorSpace::LINEAR;

                    // set sampler parameters
                    int samplerId = model.textures[textureId].sampler;
                    if (samplerId >= 0) {
                        const tinygltf::Sampler& sampler =
                            model.samplers[samplerId];
                        texture.minFilter() =
                            static_cast<Texture::MinificationFilter>(
                                sampler.minFilter);
                        texture.magFilter() =
                            static_cast<Texture::MagnificationFilter>(
                                sampler.magFilter);
                        texture.wrapU() =
                            static_cast<Texture::WrapMode>(sampler.wrapS);
                        texture.wrapV() =
                            static_cast<Texture::WrapMode>(sampler.wrapT);
                    }
                    else {
                        assert(samplerId == -1);
                        assert(
                            texture.minFilter() ==
                            Texture::MinificationFilter::NONE);
                        assert(
                            texture.magFilter() ==
                            Texture::MagnificationFilter::NONE);
                        assert(texture.wrapU() == Texture::WrapMode::REPEAT);
                        assert(texture.wrapV() == Texture::WrapMode::REPEAT);
                    }

                    // if the image is valid, load it to the texture
                    if (img.image.size() > 0 &&
                        (img.bits == 8 || img.component == 4)) {
                        Image timg(img.image.data(), img.width, img.height);

                        m.pushTextureImage(uri, std::move(timg));
                    }
                }
            };

        /* Put the data in the mesh */

        if constexpr (HasMaterials<MeshType>) {
            Material mat;
            mat.name()              = matName;
            mat.baseColor()         = baseColor;
            mat.metallic()          = metallic;
            mat.roughness()         = roughness;
            mat.emissiveColor()     = emissiveColor;
            mat.alphaMode()         = alphaMode;
            mat.alphaCutoff()       = alphaCutoff;
            mat.doubleSided()       = doubleSided;
            mat.normalScale()       = normalScale;
            mat.occlusionStrength() = occlusionStrength;
            loadTextureInMaterial(
                mat, baseColorTextureId, Material::TextureType::BASE_COLOR);
            loadTextureInMaterial(
                mat,
                metallicRoughnessTextureId,
                Material::TextureType::METALLIC_ROUGHNESS);
            loadTextureInMaterial(
                mat, normalTextureId, Material::TextureType::NORMAL);
            loadTextureInMaterial(
                mat, occlusionTextureId, Material::TextureType::OCCLUSION);
            loadTextureInMaterial(
                mat, emissiveTextureId, Material::TextureType::EMISSIVE);
            m.pushMaterial(mat);
            idx = m.materialsNumber() - 1; // index of the added material
        }
        else if constexpr (HasColor<MeshType>) {
            // base color is set to the mesh color only if the mesh has no
            // materials
            m.color() = baseColor;
        }
    }

    return idx;
}

template<MeshConcept MeshType, typename Scalar>
bool populateGltfVertices(
    MeshType&     m,
    const Scalar* posArray,
    uint          stride,
    uint          vertNumber)
{
    using PositionType = typename MeshType::VertexType::PositionType;

    uint base = m.addVertices(vertNumber);

    for (uint i = 0; i < vertNumber; ++i) {
        const Scalar* posBase = reinterpret_cast<const Scalar*>(
            reinterpret_cast<const char*>(posArray) + (i) *stride);
        m.vertex(base + i).position() =
            PositionType(posBase[0], posBase[1], posBase[2]);
    }
    return true;
}

template<MeshConcept MeshType, typename Scalar>
bool populateGltfVNormals(
    MeshType&     m,
    uint          firstVertex,
    bool          enableOptionalComponents,
    const Scalar* normArray,
    unsigned int  stride,
    unsigned int  vertNumber)
{
    if constexpr (HasPerVertexNormal<MeshType>) {
        using NormalType = typename MeshType::VertexType::NormalType;

        if (enableOptionalComponents)
            enableIfPerVertexNormalOptional(m);

        if (isPerVertexNormalAvailable(m)) {
            for (unsigned int i = 0; i < vertNumber; i++) {
                const Scalar* normBase = reinterpret_cast<const Scalar*>(
                    reinterpret_cast<const char*>(normArray) + i * stride);
                m.vertex(firstVertex + i).normal() =
                    NormalType(normBase[0], normBase[1], normBase[2]);
            }
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

template<MeshConcept MeshType, typename Scalar>
bool populateGltfVTangents(
    MeshType&     m,
    uint          firstVertex,
    bool          enableOptionalComponents,
    const Scalar* tangArray,
    unsigned int  stride,
    unsigned int  vertNumber)
{
    if constexpr (HasPerVertexTangent<MeshType>) {
        using TangentType = typename MeshType::VertexType::TangentType;

        if (enableOptionalComponents)
            enableIfPerVertexTangentOptional(m);

        if (isPerVertexTangentAvailable(m)) {
            for (unsigned int i = 0; i < vertNumber; i++) {
                const Scalar* tangBase = reinterpret_cast<const Scalar*>(
                    reinterpret_cast<const char*>(tangArray) + i * stride);
                m.vertex(firstVertex + i).tangent() =
                    TangentType(tangBase[0], tangBase[1], tangBase[2]);
                bool rh = tangBase[3] >= 0;
                m.vertex(firstVertex + i).tangentRightHanded() = rh;
            }
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

template<MeshConcept MeshType, typename Scalar>
bool populateGltfVColors(
    MeshType&     m,
    uint          firstVertex,
    bool          enableOptionalComponents,
    const Scalar* colorArray,
    unsigned int  stride,
    unsigned int  vertNumber,
    bool          colorWithAlpha)
{
    unsigned int nElemns = colorWithAlpha ? 4 : 3;
    if constexpr (HasPerVertexColor<MeshType>) {
        if (enableOptionalComponents)
            enableIfPerVertexColorOptional(m);

        if (isPerVertexColorAvailable(m)) {
            for (unsigned int i = 0; i < vertNumber * nElemns; i += nElemns) {
                const Scalar* colorBase = reinterpret_cast<const Scalar*>(
                    reinterpret_cast<const char*>(colorArray) +
                    (i / nElemns) * stride);
                const auto vi = firstVertex + i / nElemns;
                vcl::Color c;
                if constexpr (!std::is_floating_point<Scalar>::value) {
                    uint alpha = nElemns == 4 ? colorBase[3] : 255;
                    c          = vcl::Color(
                        colorBase[0], colorBase[1], colorBase[2], alpha);
                }
                else {
                    uint alpha = nElemns == 4 ? colorBase[3] * 255 : 255;
                    c          = vcl::Color(
                        colorBase[0] * 255,
                        colorBase[1] * 255,
                        colorBase[2] * 255,
                        alpha);
                }
                m.vertex(vi).color() = c;
            }
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

template<MeshConcept MeshType, typename Scalar>
bool populateGltfVTextCoords(
    MeshType&     m,
    uint          firstVertex,
    bool          enableOptionalComponents,
    const Scalar* textCoordArray,
    unsigned int  stride,
    unsigned int  vertNumber)
{
    if constexpr (HasPerVertexTexCoord<MeshType>) {
        using TexCoordType = typename MeshType::VertexType::TexCoordType;

        if (enableOptionalComponents)
            enableIfPerVertexTexCoordOptional(m);

        if (isPerVertexTexCoordAvailable(m)) {
            for (unsigned int i = 0; i < vertNumber; i++) {
                const Scalar* textCoordBase = reinterpret_cast<const Scalar*>(
                    reinterpret_cast<const char*>(textCoordArray) + i * stride);

                m.vertex(firstVertex + i).texCoord() =
                    TexCoordType(textCoordBase[0], 1 - textCoordBase[1]);
            }
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

template<MeshConcept MeshType, typename Scalar>
bool populateGltfTriangles(
    MeshType&     m,
    uint          firstVertex,
    const Scalar* triArray,
    uint          triNumber)
{
    if constexpr (HasFaces<MeshType>) {
        if (triArray != nullptr) {
            uint fi = m.addFaces(triNumber);
            for (unsigned int i = 0; i < triNumber * 3; i += 3, ++fi) {
                auto& f = m.face(fi);
                if constexpr (HasPolygons<MeshType>) {
                    f.resizeVertices(3);
                }
                for (int j = 0; j < 3; ++j) {
                    f.setVertex(j, firstVertex + triArray[i + j]);
                }
            }
        }
        else {
            triNumber = m.vertexNumber() / 3 - firstVertex;
            uint fi   = m.addFaces(triNumber);
            for (uint i = 0; i < triNumber * 3; i += 3, ++fi) {
                auto& f = m.face(fi);
                for (uint j = 0; j < 3; ++j) {
                    f.setVertex(j, firstVertex + i + j);
                }
            }
        }
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief given the attribute and the pointer to the data,
 * it calls the appropriate functions that put the data into the mesh
 * appropriately
 * @param attr
 * @param m
 * @param array: vector containing the data
 * @param stride:
 *     number of bytes between consecutive elements in the vector
 *     (only applies to vertex attributes; indices are always tightly packed)
 * @param number: number of elements contained in the data
 * @param colorWithAlpha: if attr is color, tells if color has 3 or 4 components
 */
template<typename Scalar, MeshConcept MeshType>
bool populateGltfAttr(
    GltfAttrType  attr,
    MeshType&     m,
    uint          firstVertex,
    bool          enableOptionalComponents,
    const Scalar* array,
    unsigned int  stride,
    unsigned int  number,
    bool          colorWithAlpha = true)
{
    using enum GltfAttrType;

    switch (attr) {
    case POSITION: return populateGltfVertices(m, array, stride, number);
    case NORMAL:
        return populateGltfVNormals(
            m, firstVertex, enableOptionalComponents, array, stride, number);
    case COLOR_0:
        return populateGltfVColors(
            m,
            firstVertex,
            enableOptionalComponents,
            array,
            stride,
            number,
            colorWithAlpha);
    case TEXCOORD_0:
        return populateGltfVTextCoords(
            m,
            firstVertex,
            enableOptionalComponents,
            array,
            stride,
            number);
    case TANGENT:
        return populateGltfVTangents(
            m, 
            firstVertex, 
            enableOptionalComponents, 
            array, 
            stride, 
            number);
    case INDICES:
        return populateGltfTriangles(m, firstVertex, array, number / 3);
    default: return false;
    }
}

/**
 * @brief loads the attribute attr from the primitive p contained in the
 * gltf model. If the attribute is vertex position, sets also vertex pointers
 * vector ivp. For all the other parameters, ivp is a const input.
 *
 * If the primitive does not contain the attribute attr, nothing is done.
 * However, if the attribute is POSITION, then a MLException will be thrown.
 *
 *
 * @param m
 * @param model
 * @param p
 * @param attr
 * @param textID: id of the texture in case of the attr is TEXCOORD_0
 * @return true if the attribute has been loaded
 */
template<MeshConcept MeshType>
bool loadGltfAttribute(
    MeshType&                  m,
    uint                       startingVertex,
    bool                       enableOptionalComponents,
    const tinygltf::Model&     model,
    const tinygltf::Primitive& p,
    GltfAttrType               attr)
{
    using enum GltfAttrType;

    const tinygltf::Accessor* accessor = nullptr;

    // get the accessor associated to the attribute
    if (attr != INDICES) {
        auto it = p.attributes.find(GLTF_ATTR_STR[toUnderlying(attr)]);

        if (it != p.attributes.end()) { // accessor found
            accessor = &model.accessors[it->second];
        }
        else if (attr == POSITION) { // if we were looking for POSITION and
                                     // didn't find any
            throw std::runtime_error("File has not 'Position' attribute");
        }
    }
    else { // if the attribute is triangle indices
        // if the mode is GL_TRIANGLES and we have triangle indices
        if (p.mode == TINYGLTF_MODE_TRIANGLES && p.indices >= 0 &&
            (uint) p.indices < model.accessors.size()) {
            accessor = &model.accessors[p.indices];
        }
    }

    // if we found an accessor of the attribute
    if (accessor) {
        // bufferview: contains infos on how to access buffer with the accessor
        const tinygltf::BufferView& posbw =
            model.bufferViews[accessor->bufferView];

        // data of the whole buffer (vector of bytes);
        // may contain also other data not associated to our attribute
        const std::vector<unsigned char>& posdata =
            model.buffers[posbw.buffer].data;

        // offset where the data of the attribute starts
        uint posOffset = posbw.byteOffset + accessor->byteOffset;
        // hack:
        // if the attribute is a color, textid is used to tell the size of the
        // color (3 or 4 components)
        bool colorWithAlpha = true;
        if (attr == COLOR_0) {
            if (accessor->type == TINYGLTF_TYPE_VEC3)
                colorWithAlpha = false;
        }

        const uint elementSize =
            tinygltf::GetNumComponentsInType(accessor->type) *
            tinygltf::GetComponentSizeInBytes(accessor->componentType);
        const uint stride =
            (posbw.byteStride > elementSize) ? posbw.byteStride : elementSize;

        // if data is float
        if (accessor->componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
            // get the starting point of the data as float pointer
            const float* posArray = (const float*) (posdata.data() + posOffset);
            return populateGltfAttr(
                attr,
                m,
                startingVertex,
                enableOptionalComponents,
                posArray,
                stride,
                accessor->count,
                colorWithAlpha);
        }
        // if data is double
        else if (accessor->componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
            // get the starting point of the data as double pointer
            const double* posArray =
                (const double*) (posdata.data() + posOffset);
            return populateGltfAttr(
                attr,
                m,
                startingVertex,
                enableOptionalComponents,
                posArray,
                stride,
                accessor->count,
                colorWithAlpha);
        }
        // if data is ubyte
        else if (
            accessor->componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
            // get the starting point of the data as uchar pointer
            const unsigned char* triArray =
                (const unsigned char*) (posdata.data() + posOffset);
            return populateGltfAttr(
                attr,
                m,
                startingVertex,
                enableOptionalComponents,
                triArray,
                stride,
                accessor->count,
                colorWithAlpha);
        }
        // if data is ushort
        else if (
            accessor->componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            // get the starting point of the data as ushort pointer
            const unsigned short* triArray =
                (const unsigned short*) (posdata.data() + posOffset);
            return populateGltfAttr(
                attr,
                m,
                startingVertex,
                enableOptionalComponents,
                triArray,
                stride,
                accessor->count,
                colorWithAlpha);
        }
        // if data is uint
        else if (
            accessor->componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            // get the starting point of the data as uint pointer
            const uint* triArray = (const uint*) (posdata.data() + posOffset);
            return populateGltfAttr(
                attr,
                m,
                startingVertex,
                enableOptionalComponents,
                triArray,
                stride,
                accessor->count,
                colorWithAlpha);
        }
    }
    // if accessor not found and attribute is indices, it means that
    // the mesh is not indexed, and triplets of contiguous vertices
    // generate triangles
    else if (attr == INDICES) {
        // avoid explicitly the point clouds
        if (p.mode != TINYGLTF_MODE_POINTS) {
            // this case is managed when passing nullptr as data
            return populateGltfAttr<unsigned char>(
                attr,
                m,
                startingVertex,
                enableOptionalComponents,
                nullptr,
                0,
                0);
        }
    }
    return false;
}

/**
 * @brief loads the given primitive into the mesh
 * @param m
 * @param model
 * @param p
 */
template<
    MeshConcept     MeshType,
    Matrix44Concept MatrixType,
    LoggerConcept   LogType>
void loadGltfMeshPrimitive(
    MeshType&                  m,
    MeshInfo&                  info,
    const tinygltf::Model&     model,
    const tinygltf::Primitive& p,
    const MatrixType&          transf,
    const LoadSettings&        settings,
    LogType&                   log)
{
    int materialId = loadGltfPrimitiveMaterial(m, model, p);

    uint firstVertex = m.vertexNumber();

    // load vertex position attribute
    loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::POSITION);
    info.setVertices();

    bool lvn = loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::NORMAL);
    info.setPerVertexNormal(lvn);

    bool lvc = loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::COLOR_0);
    if (lvc) {
        info.setPerVertexColor();
    }

    bool lvt = loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::TEXCOORD_0);
    if (lvt) {
        info.setPerVertexTexCoord();
    }

    loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::TANGENT);

    if constexpr (HasPerVertexMaterialIndex<MeshType>) {
        if (settings.enableOptionalComponents) {
            enableIfPerVertexMaterialIndexOptional(m);
        }
        if (isPerVertexMaterialIndexAvailable(m)) {
            uint vnum = m.vertexNumber();
            for (uint v = firstVertex; v < vnum; ++v) {
                m.vertex(v).materialIndex() = materialId;
            }
            info.setPerVertexMaterialIndex();
        }
    }

    if constexpr (HasFaces<MeshType>) {
        uint firstFace = m.faceNumber();
        bool lti = loadGltfAttribute(
            m,
            firstVertex,
            settings.enableOptionalComponents,
            model,
            p,
            GltfAttrType::INDICES);
        if (lti) {
            info.setTriangleMesh();
            info.setFaces();
            info.setPerFaceVertexReferences();

            if constexpr (HasPerFaceMaterialIndex<MeshType>) {
                if (settings.enableOptionalComponents) {
                    enableIfPerFaceMaterialIndexOptional(m);
                }
                if (isPerFaceMaterialIndexAvailable(m)) {
                    uint fnum = m.faceNumber();
                    for (uint f = firstFace; f < fnum; ++f) {
                        m.face(f).materialIndex() = materialId;
                    }
                    info.setPerFaceMaterialIndex();
                }
            }
        }
    }

    if (HasTransformMatrix<MeshType>) {
        m.transformMatrix() = transf;
    }
    else {
        // if the mesh does not have a transform matrix, apply the
        // transformation matrix to the vertices
        vcl::applyTransformMatrix(m, transf);
    }
}

/**
 * @brief loads a mesh from gltf file.
 * It merges all the primitives in the loaded mesh.
 *
 * @param m: the mesh that will contain the loaded mesh
 * @param tm: tinygltf structure of the mesh to load
 * @param model: tinygltf file
 */
template<
    MeshConcept     MeshType,
    Matrix44Concept MatrixType,
    LoggerConcept   LogType>
void gltfLoadMesh(
    MeshType&              m,
    MeshInfo&              info,
    const tinygltf::Mesh&  tm,
    const tinygltf::Model& model,
    const MatrixType&      transf,
    const LoadSettings&    settings,
    LogType&               log)
{
    if constexpr (HasName<MeshType>) {
        if (!tm.name.empty()) {
            m.name() = tm.name;
        }
    }

    // TODO: fix logger - save the progress state each time a new task is
    // started
    // log.startProgress("Reading primitives", tm.primitives.size());

    // for each primitive, load it into the mesh
    for (uint i = 0; const tinygltf::Primitive& p : tm.primitives) {
        loadGltfMeshPrimitive(m, info, model, p, transf, settings, log);
        // log.progress(++i);
    }

    // log.endProgress();

    log.log(
        "Loaded mesh '" + tm.name + "' with " +
            std::to_string(tm.primitives.size()) + " primitives.",
        LogType::LogLevel::MESSAGE_LOG);
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_GLTF_DETAIL_LOAD_MESH_H
