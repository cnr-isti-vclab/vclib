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

#ifndef VCL_IO_MESH_GLTF_LOAD_MESH_H
#define VCL_IO_MESH_GLTF_LOAD_MESH_H

#include <vclib/algorithms/mesh/update.h>
#include <vclib/concepts/mesh.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/misc/logger.h>
#include <vclib/space/complex/mesh_info.h>
#include <vclib/space/core/texture.h>

#include <tiny_gltf.h>

#include <regex>

namespace vcl::detail {

enum class GltfAttrType { POSITION, NORMAL, COLOR_0, TEXCOORD_0, INDICES };
inline const std::array<std::string, 4> GLTF_ATTR_STR {
    "POSITION",
    "NORMAL",
    "COLOR_0",
    "TEXCOORD_0"};

void checkGltfPrimitiveMaterial(
    const tinygltf::Model&     model,
    const tinygltf::Primitive& p,
    int&                       textureImg,
    bool&                      hasColor,
    vcl::Color&                color)
{
    if (p.material >= 0) { // if the primitive has a material
        const tinygltf::Material& mat = model.materials[p.material];
        auto                      it  = mat.values.find("baseColorTexture");
        if (it != mat.values.end()) { // the material is a texture
            auto it2 = it->second.json_double_value.find("index");
            if (it2 != it->second.json_double_value.end()) {
                textureImg = it2->second; // get the id of the texture
            }
        }
        it = mat.values.find("baseColorFactor");
        if (it !=
            mat.values.end()) { // vertex base color, the same for a primitive
            hasColor                      = true;
            const std::vector<double>& vc = it->second.number_array;
            for (uint i = 0; i < 4; i++)
                color[i] = vc[i] * 255.0;
        }
    }
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

template <MeshConcept MeshType, typename Scalar>
bool populateGltfVNormals(
    MeshType& m,
    uint firstVertex,
    bool enableOptionalComponents,
    const Scalar* normArray,
    unsigned int stride,
    unsigned int vertNumber)
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

template <MeshConcept MeshType, typename Scalar>
bool populateGltfVColors(
    MeshType& m,
    uint firstVertex,
    bool enableOptionalComponents,
    const Scalar* colorArray,
    unsigned int stride,
    unsigned int vertNumber,
    int nElemns)
{
    if constexpr (HasPerVertexColor<MeshType>) {
        if (enableOptionalComponents)
            enableIfPerVertexColorOptional(m);

        if (isPerVertexColorAvailable(m)) {
            for (unsigned int i = 0; i < vertNumber*nElemns; i += nElemns) {
                const Scalar* colorBase =
                    reinterpret_cast<const Scalar*>(reinterpret_cast<const char*>(colorArray) + (i/nElemns) * stride);
                const auto vi = firstVertex + i/nElemns;
                vcl::Color c;
                if constexpr (!std::is_floating_point<Scalar>::value) {
                    uint alpha = nElemns == 4 ? colorBase[3] : 255;
                    c = vcl::Color(colorBase[0], colorBase[1], colorBase[2], alpha);
                }
                else {
                    uint alpha = nElemns == 4 ? colorBase[3] * 255 : 255;
                    c = vcl::Color(colorBase[0] * 255, colorBase[1]*255, colorBase[2]*255, alpha);
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

template <MeshConcept MeshType, typename Scalar>
bool populateGltfVTextCoords(
    MeshType& m,
    uint firstVertex,
    bool enableOptionalComponents,
    const Scalar* textCoordArray,
    unsigned int stride,
    unsigned int vertNumber,
    int textID)
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
                    TexCoordType(textCoordBase[0], 1-textCoordBase[1], textID);
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

template <MeshConcept MeshType, typename Scalar>
bool populateGltfTriangles(
    MeshType& m,
    uint firstVertex,
    const Scalar* triArray,
    uint triNumber)
{
    if constexpr (HasFaces<MeshType>) {
        if (triArray != nullptr) {
            uint fi = m.addFaces(triNumber);
            for (unsigned int i = 0; i < triNumber*3; i+=3, ++fi) {
                auto& f = m.face(fi);
                if constexpr (HasPolygons<MeshType>) {
                    f.resizeVertices(3);
                }
                for (int j = 0; j < 3; ++j) {
                    f.setVertex(j, firstVertex + triArray[i+j]);
                }
            }
        }
        else {
            triNumber = m.vertexNumber() / 3 - firstVertex;
            uint fi = m.addFaces(triNumber);
            for (uint i = 0; i < triNumber*3; i+=3, ++fi) {
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
 * @param textID:
 *     if attr is texcoord, it is the texture id
 *     if attr is color, tells if color has 3 or 4 components
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
    int           textID = -1)
{
    using enum GltfAttrType;

    switch (attr) {
    case POSITION: return populateGltfVertices(m, array, stride, number);
    case NORMAL:
        return populateGltfVNormals(m, firstVertex, enableOptionalComponents, array, stride, number);
    case COLOR_0:
        return populateGltfVColors(m, firstVertex, enableOptionalComponents, array, stride, number, textID);
    case TEXCOORD_0:
        return populateGltfVTextCoords(m, firstVertex, enableOptionalComponents, array, stride, number, textID);
    case INDICES:
        return populateGltfTriangles(m, firstVertex, array, number/3);
    default:
        return false;
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
    GltfAttrType               attr,
    int                        textID)
{
    using enum GltfAttrType;

    const tinygltf::Accessor* accessor   = nullptr;

    // get the accessor associated to the attribute
    if (attr != INDICES) {
        auto it = p.attributes.find(GLTF_ATTR_STR[toUnderlying(attr)]);

        if (it != p.attributes.end()) { // accessor found
            accessor = &model.accessors[it->second];
        }
        else if (attr == POSITION) { // if we were looking for POSITION and
                                     // didn't find any
            throw MalformedFileException("File has not 'Position' attribute");
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
        if (attr == COLOR_0) {
            if (accessor->type == TINYGLTF_TYPE_VEC3)
                textID = 3;
            else if (accessor->type == TINYGLTF_TYPE_VEC4)
                textID = 4;
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
                textID);
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
                textID);
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
                textID);
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
                textID);
        }
        // if data is uint
        else if (
            accessor->componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            // get the starting point of the data as uint pointer
            const uint* triArray =
                (const uint*) (posdata.data() + posOffset);
            return populateGltfAttr(
                attr,
                m,
                startingVertex,
                enableOptionalComponents,
                triArray,
                stride,
                accessor->count,
                textID);
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
    LogType&                   log,
    const LoadSettings&        settings)
{
    int  textureImg = -1;    // id of the texture associated to the material
    bool vTex       = false; // used if a material has a texture
    bool vCol =
        false;      // used if a material has a base color for all the primitive
    vcl::Color col; // the base color, to be set to all the vertices
    checkGltfPrimitiveMaterial(model, p, textureImg, vCol, col);

    if constexpr (HasTexturePaths<MeshType>) {
        if (textureImg != -1) { // if we found a texture
            vTex = true;
            const tinygltf::Image& img =
                model.images[model.textures[textureImg].source];
            // add the path of the texture to the mesh
            std::string uri = img.uri;
            uri             = std::regex_replace(uri, std::regex("\\%20"), " ");

            bool textureAdded = false;
            if constexpr (HasTextureImages<MeshType>) {
                if (img.image.size() > 0) {
                    if (img.bits == 8 || img.component == 4) {
                        if (uri.empty()) {
                            uri = "texture_" + std::to_string(textureImg);
                        }
                        vcl::Texture txt(
                            Image(img.image.data(), img.width, img.height),
                            uri);
                        m.pushTexture(txt);
                        textureAdded = true;
                    }
                }
            }
            if (!textureAdded) {
                // if the image is not valid, just add the path
                m.pushTexturePath(uri);
            }
            textureImg = m.textureNumber() - 1; // update the texture id
        }
    }

    uint firstVertex = m.vertexNumber();

    // load vertex position attribute
    loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::POSITION,
        textureImg);
    info.setVertices();

    bool lvn = loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::NORMAL,
        textureImg);
    info.setPerVertexNormal(lvn);

    if (vCol) {
        if constexpr (HasPerVertexColor<MeshType>) {
            if (settings.enableOptionalComponents) {
                enableIfPerVertexColorOptional(m);
            }
            if (isPerVertexColorAvailable(m)) {
                for (auto& v : m.vertices())
                    v.color() = col;
                info.setPerVertexColor();
            }
        }
    }

    bool lvc = loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::COLOR_0,
        textureImg);
    if (lvc) {
        info.setPerVertexColor();
    }

    bool lvt = loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::TEXCOORD_0,
        textureImg);
    if (lvt) {
        info.setPerVertexTexCoord();
    }

    bool lti = loadGltfAttribute(
        m,
        firstVertex,
        settings.enableOptionalComponents,
        model,
        p,
        GltfAttrType::INDICES,
        textureImg);
    if (lti) {
        info.setTriangleMesh();
        info.setFaces();
        info.setPerFaceVertexReferences();
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
    LogType&               log,
    const LoadSettings&    settings)
{
    if constexpr (HasName<MeshType>) {
        if (!tm.name.empty()) {
            m.name() = tm.name;
        }
    }

    log.startProgress("Reading primitives", tm.primitives.size());

    // for each primitive, load it into the mesh
    for (uint i = 0; const tinygltf::Primitive& p : tm.primitives) {
        loadGltfMeshPrimitive(m, info, model, p, transf, log, settings);
        log.progress(i);
    }

    log.endProgress();

    log.log(
        "Loaded mesh '" + tm.name + "' with " +
            std::to_string(tm.primitives.size()) + " primitives.",
        LogType::LogLevel::MESSAGE_LOG);
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_GLTF_LOAD_MESH_H
