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

#ifndef VCL_SPACE_COMPLEX_MESH_INFO_H
#define VCL_SPACE_COMPLEX_MESH_INFO_H

#include <vclib/mesh/requirements.h>
#include <vclib/types.h>

#include <array>
#include <bitset>
#include <string>
#include <typeindex>
#include <vector>

namespace vcl {

/**
 * @brief A simple class that allows to store which elements and their
 * components have been imported/loaded or are going to be exported/saved on a
 * mesh file or some other data structure.
 *
 * For example, when loading a Mesh from a file, an object of this type is used
 * to know which Elements/Components have been loaded from the file, using the
 * getter functions:
 *
 * @code{.cpp}
 * MeshInfo info;
 * AMeshType m = vcl::load<AMeshType>("meshfile.ply", info);
 *
 * if (info.hasFaces()) { // the file had faces
 *     (info.hasFaceColors()) { // the file had face colors
 *         // ...
 *     }
 * }
 * @endcode
 *
 * When saving a Mesh to a file, an object of this type is used to tell which
 * Elements/Components save on the file and, when the file format supports it,
 * to choose the type used to store a specific component:
 *
 * @code{.cpp}
 * AMeshType m;
 * MeshInfo info(m); // compute the default MeshInfo object from the Mesh
 *
 * info.setVertexCoords(true, MeshInfo::FLOAT); // force to store vertex
 *                                              // coords using floats
 * info.setVertexColors(false); // do not store vertex colors
 *
 * vcl::save(m, "meshfile.ply", info);
 * @endcode
 *
 * @ingroup load_save
 * @ingroup space_complex
 */
class MeshInfo
{
public:
    /**
     * @brief Enum used to describe the type of the Mesh - by default, the value
     * is set to UNKNOWN.
     */
    struct MeshType
    {
        enum Enum { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH, UNKNOWN };
    };

    /**
     * @brief Enum used to describe the type of Elements that can be found in a
     * file.
     *
     * @note: MESH is not an element, but it is used since some components can
     * be stored per mesh.
     */
    enum Element { VERTEX, FACE, EDGE, MESH, NUM_ELEMENTS };

    /**
     * @brief Enum used to describe the type of Components that each Element can
     * have.
     */
    enum Component {
        COORD,
        VREFS,
        NORMAL,
        COLOR,
        QUALITY,
        TEXCOORD,
        WEDGE_TEXCOORDS,
        CUSTOM_COMPONENTS,
        TEXTURES,
        NUM_COMPONENTS
    };

    /**
     * @brief Enum used to describe the type of Data stored in a component
     */
    using DataType = PrimitiveType::Enum;

    /**
     * @brief The CustomComponent struct is a simple structure that describes a
     * custom component of an Element (or of the Mesh)
     */
    struct CustomComponent
    {
        std::string name;
        DataType    type;

        CustomComponent(std::string n, DataType t) : name(n), type(t) {}
    };

private:
    // Tell if elements are present in the file
    std::bitset<NUM_ELEMENTS> mElements = {false};

    // Tell if per element components are present in the file
    std::array<std::bitset<NUM_COMPONENTS>, NUM_ELEMENTS> mPerElemComponents = {
        false};

    // Tell the data type for each component of each element
    Eigen::Matrix<DataType, NUM_ELEMENTS, NUM_COMPONENTS>
        mPerElemComponentsType;

    // Store name and type of per element custom components
    std::array<std::vector<CustomComponent>, NUM_ELEMENTS>
        mPerElemCustomComponents;

    // Mesh Type
    MeshType::Enum mType = MeshType::UNKNOWN;

public:
    /**
     * @brief Default constructor.
     *
     * All the Elements/Components are disabled, their type is set to
     * DataType::NONE and the Mesh Type is set to MeshType::POLYGON_MESH.
     */
    MeshInfo() { mPerElemComponentsType.fill(PrimitiveType::NONE); }

    /**
     * @brief Sets the current status of the MeshInfo object from the input
     * mesh.
     *
     * @tparam Mesh: The type of the input mesh, it must satisfy the
     * MeshConcept.
     *
     * @param[in] m: the mesh from which construct the MeshInfo object
     */
    template<MeshConcept Mesh>
    MeshInfo(const Mesh& m)
    {
        setVertices();
        setVertexCoords(
            true, getType<typename Mesh::VertexType::CoordType::ScalarType>());
        if constexpr (HasPerVertexNormal<Mesh>) {
            if (isPerVertexNormalAvailable(m)) {
                setVertexNormals(
                    true,
                    getType<
                        typename Mesh::VertexType::NormalType::ScalarType>());
            }
        }
        if constexpr (HasPerVertexColor<Mesh>) {
            if (isPerVertexColorAvailable(m)) {
                setVertexColors(true, PrimitiveType::UCHAR);
            }
        }
        if constexpr (HasPerVertexQuality<Mesh>) {
            if (isPerVertexQualityAvailable(m)) {
                setVertexQuality(
                    true, getType<typename Mesh::VertexType::QualityType>());
            }
        }
        if constexpr (HasPerVertexTexCoord<Mesh>) {
            if (isPerVertexTexCoordAvailable(m)) {
                setVertexTexCoords(
                    true,
                    getType<
                        typename Mesh::VertexType::TexCoordType::ScalarType>());
            }
        }
        if constexpr (HasPerVertexCustomComponents<Mesh>) {
            auto names = m.perVertexCustomComponentNames();
            for (auto& name : names) {
                DataType dt = getType(m.perVertexCustomComponentType(name));
                if (dt != PrimitiveType::NONE) {
                    addVertexCustomComponent(name, dt);
                }
            }
        }

        if constexpr (HasFaces<Mesh>) {
            setFaces();
            setFaceVRefs();
            if (HasTriangles<Mesh>) {
                setTriangleMesh();
            }
            else if (HasQuads<Mesh>) {
                setQuadMesh();
            }
            else {
                setPolygonMesh();
            }
            if constexpr (HasPerFaceNormal<Mesh>) {
                if (isPerFaceNormalAvailable(m)) {
                    setFaceNormals(
                        true,
                        getType<
                            typename Mesh::FaceType::NormalType::ScalarType>());
                }
            }
            if constexpr (HasPerFaceColor<Mesh>) {
                if (isPerFaceColorAvailable(m)) {
                    setFaceColors(true, PrimitiveType::UCHAR);
                }
            }
            if constexpr (HasPerFaceQuality<Mesh>) {
                if (isPerFaceQualityAvailable(m)) {
                    setFaceQuality(
                        true, getType<typename Mesh::FaceType::QualityType>());
                }
            }
            if constexpr (HasPerFaceWedgeTexCoords<Mesh>) {
                if (isPerFaceWedgeTexCoordsAvailable(m)) {
                    setFaceWedgeTexCoords(
                        true,
                        getType<typename Mesh::FaceType::WedgeTexCoordType::
                                    ScalarType>());
                }
            }
            if constexpr (HasPerFaceCustomComponents<Mesh>) {
                auto names = m.perFaceCustomComponentNames();
                for (auto& name : names) {
                    DataType dt = getType(m.perFaceCustomComponentType(name));
                    if (dt != PrimitiveType::NONE) {
                        addFaceCustomComponent(name, dt);
                    }
                }
            }
        }

        if constexpr (HasEdges<Mesh>) {
            setEdges();
            setEdgeVRefs();
            // if constexpr (HasPerEdgeColor<Mesh>)
            //     if (isPerEdgeColorAvailable(m))
            //         setEdgeColors(true, UCHAR);
        }

        if constexpr (HasTexturePaths<Mesh>) {
            if (m.textureNumber() > 0) {
                setTextures(true);
            }
        }
    }

    /**
     * @brief Clears the MeshInfo object.
     *
     * All the Elements/Components are disabled, their type is set to
     * DataType::NONE and the Mesh Type is set to MeshType::UNKNOWN.
     */
    void clear()
    {
        mElements.reset();
        for (auto& comp : mPerElemComponents)
            comp.reset();
        mPerElemComponentsType.fill(PrimitiveType::NONE);

        for (auto& v : mPerElemCustomComponents)
            v.clear();

        mType = MeshType::UNKNOWN;
    }

    /**
     * @brief Returns whether the current MeshInfo object is empty, i.e., it has
     * no Elements set.
     */
    bool isEmpty() const { return !mElements.any(); }

    bool isUnkownMesh() const { return mType == MeshType::UNKNOWN; }

    /**
     * @brief Returns true if the current object has Mesh type set to
     * MeshType::TRIANGLE_MESH.
     * @return true if the current Mesh type is set to MeshType::TRIANGLE_MESH.
     */
    bool isTriangleMesh() const { return mType == MeshType::TRIANGLE_MESH; }

    /**
     * @brief Returns true if the current object has Mesh type set to
     * MeshType::QUAD_MESH.
     * @return true if the current Mesh type is set to MeshType::QUAD_MESH.
     */
    bool isQuadMesh() const { return mType == MeshType::QUAD_MESH; }

    /**
     * @brief Returns true if the current object has Mesh type set to
     * MeshType::POLYGON_MESH.
     * @return true if the current Mesh type is set to MeshType::POLYGON_MESH.
     */
    bool isPolygonMesh() const { return mType == MeshType::POLYGON_MESH; }

    /*
     * Getter Elements/Components functions: they are used mostly after the
     * loading of a Mesh from a file, to know if Elements/Components have been
     * loaded.
     */

    bool hasElement(Element el) const { return mElements[el]; }

    bool hasPerElementComponent(Element el, Component comp) const
    {
        return mPerElemComponents[el][comp];
    }

    /**
     * @brief Returns true if the current object has Vertex Elements.
     * @return true if the current object has Vertex Elements.
     */
    bool hasVertices() const { return hasElement(VERTEX); }

    /**
     * @brief Returns true if the current object has Vertex Coordinates.
     * @return true if the current object has Vertex Coordinates.
     */
    bool hasVertexCoords() const
    {
        return hasPerElementComponent(VERTEX, COORD);
    }

    /**
     * @brief Returns true if the current object has Vertex Normals.
     * @return true if the current object has Vertex Normals.
     */
    bool hasVertexNormals() const
    {
        return hasPerElementComponent(VERTEX, NORMAL);
    }

    /**
     * @brief Returns true if the current object has Vertex Colors.
     * @return true if the current object has Vertex Colors.
     */
    bool hasVertexColors() const
    {
        return hasPerElementComponent(VERTEX, COLOR);
    }

    /**
     * @brief Returns true if the current object has Vertex Quality.
     * @return true if the current object has Vertex Quality.
     */
    bool hasVertexQuality() const
    {
        return hasPerElementComponent(VERTEX, QUALITY);
    }

    /**
     * @brief Returns true if the current object has Vertex Texture Coordinates.
     * @return true if the current object has Vertex Texture Coordinates.
     */
    bool hasVertexTexCoords() const
    {
        return hasPerElementComponent(VERTEX, TEXCOORD);
    }

    /**
     * @brief Returns true if the current object has Vertex Custom Components.
     * @return true if the current object has Vertex Custom Components.
     */
    bool hasVertexCustomComponents() const
    {
        return hasPerElementComponent(VERTEX, CUSTOM_COMPONENTS);
    }

    /**
     * @brief Returns true if the current object has Face Elements.
     * @return true if the current object has Face Elements.
     */
    bool hasFaces() const { return hasElement(FACE); }

    /**
     * @brief Returns true if the current object has per Face Vertex References.
     * @return true if the current object has per Face Vertex References.
     */
    bool hasFaceVRefs() const { return hasPerElementComponent(FACE, VREFS); }

    bool hasFaceNormals() const { return hasPerElementComponent(FACE, NORMAL); }

    bool hasFaceColors() const { return hasPerElementComponent(FACE, COLOR); }

    bool hasFaceQuality() const
    {
        return hasPerElementComponent(FACE, QUALITY);
    }

    bool hasFaceWedgeTexCoords() const
    {
        return hasPerElementComponent(FACE, WEDGE_TEXCOORDS);
    }

    bool hasFaceCustomComponents() const
    {
        return hasPerElementComponent(FACE, CUSTOM_COMPONENTS);
    }

    /**
     * @brief Returns true if the current object has Edge Elements.
     * @return true if the current object has Edge Elements.
     */
    bool hasEdges() const { return hasElement(EDGE); }

    bool hasEdgeVRefs() const { return hasPerElementComponent(EDGE, VREFS); }

    bool hasEdgeColors() const { return hasPerElementComponent(EDGE, COLOR); }

    bool hasEdgeNormals() const { return hasPerElementComponent(EDGE, NORMAL); }

    bool hasEdgeQuality() const { return hasPerElementComponent(EDGE, QUALITY); }

    bool hasEdgeCustomComponents() const
    {
        return hasPerElementComponent(EDGE, CUSTOM_COMPONENTS);
    }

    bool hasTextures() const { return hasPerElementComponent(MESH, TEXTURES); }

    /*
     * Setter functions: they are used by the load functions to tell which
     * Elements/Components are loaded from a file, and they can be used by the
     * user that wants to save in a file only a specific set of
     * Elements/Components of a Mesh.
     */

    void updateMeshType(uint faceSize)
    {
        if (mType == MeshType::UNKNOWN) {
            if (faceSize == 3)
                setTriangleMesh();
            else if (faceSize == 4)
                setQuadMesh();
            else
                setPolygonMesh();
        }
        else if (mType == MeshType::TRIANGLE_MESH && faceSize != 3) {
            setPolygonMesh();
        }
        else if (mType == MeshType::QUAD_MESH && faceSize != 4) {
            setPolygonMesh();
        }
    }

    void setUnknownMesh() { mType = MeshType::UNKNOWN; }

    void setTriangleMesh() { mType = MeshType::TRIANGLE_MESH; }

    void setQuadMesh() { mType = MeshType::QUAD_MESH; }

    void setPolygonMesh() { mType = MeshType::POLYGON_MESH; }

    void setMeshType(MeshType::Enum t) { mType = t; }

    void setElement(Element el, bool b = true) { mElements[el] = b; }

    void setElementComponents(Element el, Component c, bool b, DataType t)
    {
        mElements[el]             = b;
        mPerElemComponents[el][c] = b;
        if (b)
            mPerElemComponentsType(el, c) = t;
    }

    void setVertices(bool b = true) { setElement(VERTEX, b); }

    void setVertexCoords(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponents(VERTEX, COORD, b, t);
    }

    void setVertexNormals(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponents(VERTEX, NORMAL, b, t);
    }

    void setVertexColors(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setElementComponents(VERTEX, COLOR, b, t);
    }

    void setVertexQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponents(VERTEX, QUALITY, b, t);
    }

    void setVertexTexCoords(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponents(VERTEX, TEXCOORD, b, t);
    }

    void setVertexCustomComponents(bool b = true)
    {
        setElementComponents(VERTEX, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setFaces(bool b = true) { setElement(FACE, b); }

    void setFaceVRefs(bool b = true)
    {
        setElementComponents(FACE, VREFS, b, PrimitiveType::NONE);
    }

    void setFaceNormals(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponents(FACE, NORMAL, b, t);
    }

    void setFaceColors(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setElementComponents(FACE, COLOR, b, t);
    }

    void setFaceQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponents(FACE, QUALITY, b, t);
    }

    void setFaceWedgeTexCoords(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponents(FACE, WEDGE_TEXCOORDS, b, t);
    }

    void setFaceCustomComponents(bool b = true)
    {
        setElementComponents(FACE, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setEdges(bool b = true) { setElement(EDGE, b); }

    void setEdgeVRefs(bool b = true)
    {
        setElementComponents(EDGE, VREFS, b, PrimitiveType::NONE);
    }

    void setEdgeColors(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setElementComponents(EDGE, COLOR, b, t);
    }

    void setEdgeNormals(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponents(EDGE, NORMAL, b, t);
    }

    void setEdgeQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponents(EDGE, QUALITY, b, t);
    }

    void setEdgeCustomComponents(bool b = true)
    {
        setElementComponents(EDGE, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setTextures(bool b = true)
    {
        setElementComponents(MESH, TEXTURES, b, PrimitiveType::NONE);
    }

    void addElementCustomComponent(
        Element            el,
        const std::string& name,
        DataType           t)
    {
        setElementComponents(el, CUSTOM_COMPONENTS, true, PrimitiveType::NONE);
        mPerElemCustomComponents[el].emplace_back(name, t);
    }

    void clearElementCustomComponents(Element el)
    {
        setElementComponents(el, CUSTOM_COMPONENTS, false, PrimitiveType::NONE);
        mPerElemCustomComponents[el].clear();
    }

    void addVertexCustomComponent(const std::string& name, DataType t)
    {
        addElementCustomComponent(VERTEX, name, t);
    }

    void clearVertexCustomComponents() { clearElementCustomComponents(VERTEX); }

    void addFaceCustomComponent(const std::string& name, DataType t)
    {
        addElementCustomComponent(FACE, name, t);
    }

    void clearFaceCustomComponents() { clearElementCustomComponents(FACE); }

    void addEdgeCustomComponent(const std::string& name, DataType t)
    {
        addElementCustomComponent(EDGE, name, t);
    }

    void clearEdgeCustomComponents() { clearElementCustomComponents(EDGE); }

    /*
     * Getter Component type functions : they are used mostly by save functions
     * to know the type that needs to use to save a given Component
     */

    DataType elementComponentType(Element el, Component comp) const
    {
        return mPerElemComponentsType(el, comp);
    }

    DataType vertexCoordsType() const
    {
        return elementComponentType(VERTEX, COORD);
    }

    DataType vertexNormalsType() const
    {
        return elementComponentType(VERTEX, NORMAL);
    }

    DataType vertexColorsType() const
    {
        return elementComponentType(VERTEX, COLOR);
    }

    DataType vertexQualityType() const
    {
        return elementComponentType(VERTEX, QUALITY);
    }

    DataType vertexTexCoordsType() const
    {
        return elementComponentType(VERTEX, TEXCOORD);
    }

    DataType faceNormalsType() const
    {
        return elementComponentType(FACE, NORMAL);
    }

    DataType faceColorsType() const
    {
        return elementComponentType(FACE, COLOR);
    }

    DataType faceQualityType() const
    {
        return elementComponentType(FACE, QUALITY);
    }

    DataType faceWedgeTexCoordsType() const
    {
        return elementComponentType(FACE, WEDGE_TEXCOORDS);
    }

    DataType edgeColorsType() const
    {
        return elementComponentType(EDGE, COLOR);
    }

    DataType edgeNormalsType() const
    {
        return elementComponentType(EDGE, NORMAL);
    }

    DataType edgeQualityType() const
    {
        return elementComponentType(EDGE, QUALITY);
    }

    const std::vector<CustomComponent>& vertexCustomComponents() const
    {
        return mPerElemCustomComponents[VERTEX];
    }

    const std::vector<CustomComponent>& faceCustomComponents() const
    {
        return mPerElemCustomComponents[FACE];
    }

    const std::vector<CustomComponent>& edgeCustomComponents() const
    {
        return mPerElemCustomComponents[EDGE];
    }

    /**
     * @brief Returns a MeshInfo object that is the intersection between this
     * and `info`.
     *
     * The intersection is a MeshInfo object that has Elements/Components
     * enabled only if they are enabled both in this object and in `info`. Types
     * are imported from this MeshInfo.
     *
     * @param[in] info: The info object to compute the intersection with.
     * @return The intersection between this and `info`.
     */
    [[nodiscard]] MeshInfo intersect(const MeshInfo& info) const
    {
        MeshInfo res;
        for (uint i = 0; i < NUM_ELEMENTS; ++i) {
            res.mElements[i] = mElements[i] && info.mElements[i];
            for (uint j = 0; j < NUM_COMPONENTS; ++j) {
                res.mPerElemComponents[i][j] =
                    mPerElemComponents[i][j] && info.mPerElemComponents[i][j];

                if (res.mPerElemComponents[i][j]) {
                    res.mPerElemComponentsType(i, j) =
                        mPerElemComponentsType(i, j);
                }
            }
        }

        if (mType == info.mType) {
            res.mType = mType;
        }
        res.mPerElemCustomComponents = mPerElemCustomComponents;

        return res;
    }

private:
    /**
     * @brief Given the template T, returns the corresponding enum DataType
     * value of T.
     *
     * Returns DataType::NONE if the type T was not part of the type
     * supported by the DataType enum.
     * @return
     */
    template<typename T>
    static DataType getType()
    {
        if constexpr (std::is_same_v<T, char>)
            return PrimitiveType::CHAR;
        if constexpr (std::is_same_v<T, unsigned char>)
            return PrimitiveType::UCHAR;
        if constexpr (std::is_same_v<T, short>)
            return PrimitiveType::SHORT;
        if constexpr (std::is_same_v<T, unsigned short>)
            return PrimitiveType::USHORT;
        if constexpr (std::is_same_v<T, int>)
            return PrimitiveType::INT;
        if constexpr (std::is_same_v<T, uint>)
            return PrimitiveType::UINT;
        if constexpr (std::is_integral_v<T>)
            return PrimitiveType::INT; // fallback to int
        if constexpr (std::is_same_v<T, float>)
            return PrimitiveType::FLOAT;
        if constexpr (std::is_same_v<T, double>)
            return PrimitiveType::DOUBLE;
        if constexpr (std::is_floating_point_v<T>)
            return PrimitiveType::FLOAT; // fallback to float
        return PrimitiveType::NONE;
    }

    static DataType getType(std::type_index ti)
    {
        if (ti == typeid(char))
            return PrimitiveType::CHAR;
        if (ti == typeid(unsigned char))
            return PrimitiveType::UCHAR;
        if (ti == typeid(short))
            return PrimitiveType::SHORT;
        if (ti == typeid(unsigned short))
            return PrimitiveType::USHORT;
        if (ti == typeid(int))
            return PrimitiveType::INT;
        if (ti == typeid(uint))
            return PrimitiveType::UINT;
        if (ti == typeid(float))
            return PrimitiveType::FLOAT;
        if (ti == typeid(double))
            return PrimitiveType::DOUBLE;
        return PrimitiveType::NONE;
    }
};

template<uint ELEM_ID, MeshConcept MeshType>
void addPerElementCustomComponent(
    MeshType&                        m,
    const MeshInfo::CustomComponent& cc)
{
    switch (cc.type) {
    case PrimitiveType::CHAR:
        m.template addPerElementCustomComponent<ELEM_ID, char>(cc.name);
        break;
    case PrimitiveType::UCHAR:
        m.template addPerElementCustomComponent<ELEM_ID, unsigned char>(
            cc.name);
        break;
    case PrimitiveType::SHORT:
        m.template addPerElementCustomComponent<ELEM_ID, short>(cc.name);
        break;
    case PrimitiveType::USHORT:
        m.template addPerElementCustomComponent<ELEM_ID, unsigned short>(
            cc.name);
        break;
    case PrimitiveType::INT:
        m.template addPerElementCustomComponent<ELEM_ID, int>(cc.name);
        break;
    case PrimitiveType::UINT:
        m.template addPerElementCustomComponent<ELEM_ID, uint>(cc.name);
        break;
    case PrimitiveType::FLOAT:
        m.template addPerElementCustomComponent<ELEM_ID, float>(cc.name);
        break;
    case PrimitiveType::DOUBLE:
        m.template addPerElementCustomComponent<ELEM_ID, double>(cc.name);
        break;
    default: assert(0);
    }
}

template<MeshConcept MeshType>
void addPerVertexCustomComponent(
    MeshType&                        m,
    const MeshInfo::CustomComponent& cc)
{
    addPerElementCustomComponent<ElemId::VERTEX>(m, cc);
}

template<FaceMeshConcept MeshType>
void addPerFaceCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
{
    addPerElementCustomComponent<ElemId::FACE>(m, cc);
}

template<EdgeMeshConcept MeshType>
void addPerEdgeCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
{
    addPerElementCustomComponent<ElemId::EDGE>(m, cc);
}

/**
 * @brief Enables all the components that are in the file mesh info and that may
 * be enabled in the mesh. If these components are not available in the mesh,
 * the info file will be modified in order to tell that a particular component
 * cannot be saved into the mesh.
 *
 * This function is useful when you know what components are going to be load
 * from a file, and you want that all the components that you are going to read
 * in the file will be saved in the mesh, if possible.
 *
 * @param info
 * @param m
 */
template<MeshConcept MeshType>
void enableOptionalComponentsFromInfo(MeshInfo& info, MeshType& m)
{
    if (info.hasVertices()) {
        if (info.hasVertexColors()) {
            if (!enableIfPerVertexColorOptional(m)) {
                info.setVertexColors(false);
            }
        }
        if (info.hasVertexNormals()) {
            if (!enableIfPerVertexNormalOptional(m)) {
                info.setVertexNormals(false);
            }
        }
        if (info.hasVertexQuality()) {
            if (!enableIfPerVertexQualityOptional(m)) {
                info.setVertexQuality(false);
            }
        }
        if (info.hasVertexTexCoords()) {
            if (!enableIfPerVertexTexCoordOptional(m)) {
                info.setVertexTexCoords(false);
            }
        }
        if (info.hasVertexCustomComponents()) {
            if constexpr (HasPerVertexCustomComponents<MeshType>) {
                for (const auto& cc : info.vertexCustomComponents()) {
                    addPerVertexCustomComponent(m, cc);
                }
            }
            else {
                info.clearVertexCustomComponents();
            }
        }
    }
    else {
        info.setVertices(false);
    }

    if constexpr (HasFaces<MeshType>) {
        if (info.hasFaces()) {
            if (info.hasFaceColors()) {
                if (!enableIfPerFaceColorOptional(m)) {
                    info.setFaceColors(false);
                }
            }
            if (info.hasFaceNormals()) {
                if (!enableIfPerFaceNormalOptional(m)) {
                    info.setFaceNormals(false);
                }
            }
            if (info.hasFaceQuality()) {
                if (!enableIfPerFaceQualityOptional(m)) {
                    info.setFaceQuality(false);
                }
            }
            if (info.hasFaceWedgeTexCoords()) {
                if (!enableIfPerFaceWedgeTexCoordsOptional(m)) {
                    info.setFaceWedgeTexCoords(false);
                }
            }
            if (info.hasFaceCustomComponents()) {
                if constexpr (HasPerFaceCustomComponents<MeshType>) {
                    for (const auto& cc : info.faceCustomComponents()) {
                        addPerFaceCustomComponent(m, cc);
                    }
                }
                else {
                    info.clearFaceCustomComponents();
                }
            }
        }
        else {
            info.setFaces(false);
        }
    }
    else {
        info.setFaces(false);
    }

    if constexpr (HasEdges<MeshType>) {
        if (info.hasEdges()) {
            if (info.hasEdgeColors()) {
                if (!enableIfPerEdgeColorOptional(m)) {
                    info.setEdgeColors(false);
                }
            }
            if (info.hasEdgeNormals()) {
                if (!enableIfPerEdgeNormalOptional(m)) {
                    info.setEdgeNormals(false);
                }
            }
            if (info.hasEdgeQuality()) {
                if (!enableIfPerEdgeQualityOptional(m)) {
                    info.setEdgeQuality(false);
                }
            }
            if (info.hasEdgeCustomComponents()) {
                if constexpr (HasPerEdgeCustomComponents<MeshType>) {
                    for (const auto& cc : info.edgeCustomComponents()) {
                        addPerEdgeCustomComponent(m, cc);
                    }
                }
                else {
                    info.clearEdgeCustomComponents();
                }
            }
        }
        else {
            info.setEdges(false);
        }
    }
    else {
        info.setEdges(false);
    }
}

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_MESH_INFO_H
