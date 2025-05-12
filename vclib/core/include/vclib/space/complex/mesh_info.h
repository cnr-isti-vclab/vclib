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
 * @ingroup io_mesh
 * @ingroup space_complex
 */
class MeshInfo
{
public:
    /**
     * @brief Enum used to describe the type of the Mesh - by default, the value
     * is set to UNKNOWN.
     */
    enum class MeshType { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH, UNKNOWN };

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
    using DataType = PrimitiveType;

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
    MeshType mType = MeshType::UNKNOWN;

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
        setPerVertexCoordinate(
            true, getType<typename Mesh::VertexType::CoordType::ScalarType>());
        if constexpr (HasPerVertexNormal<Mesh>) {
            if (isPerVertexNormalAvailable(m)) {
                setPerVertexNormal(
                    true,
                    getType<
                        typename Mesh::VertexType::NormalType::ScalarType>());
            }
        }
        if constexpr (HasPerVertexColor<Mesh>) {
            if (isPerVertexColorAvailable(m)) {
                setPerVertexColor(true, PrimitiveType::UCHAR);
            }
        }
        if constexpr (HasPerVertexQuality<Mesh>) {
            if (isPerVertexQualityAvailable(m)) {
                setPerVertexQuality(
                    true, getType<typename Mesh::VertexType::QualityType>());
            }
        }
        if constexpr (HasPerVertexTexCoord<Mesh>) {
            if (isPerVertexTexCoordAvailable(m)) {
                setPerVertexTexCoord(
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
                    addPerVertexCustomComponent(name, dt);
                }
            }
        }

        if constexpr (HasFaces<Mesh>) {
            setFaces();
            setPerFaceVertexReferences();
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
                    setPerFaceNormal(
                        true,
                        getType<
                            typename Mesh::FaceType::NormalType::ScalarType>());
                }
            }
            if constexpr (HasPerFaceColor<Mesh>) {
                if (isPerFaceColorAvailable(m)) {
                    setPerFaceColor(true, PrimitiveType::UCHAR);
                }
            }
            if constexpr (HasPerFaceQuality<Mesh>) {
                if (isPerFaceQualityAvailable(m)) {
                    setPerFaceQuality(
                        true, getType<typename Mesh::FaceType::QualityType>());
                }
            }
            if constexpr (HasPerFaceWedgeTexCoords<Mesh>) {
                if (isPerFaceWedgeTexCoordsAvailable(m)) {
                    setPerFaceWedgeTexCoords(
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
                        addPerFaceCustomComponent(name, dt);
                    }
                }
            }
        }

        if constexpr (HasEdges<Mesh>) {
            setEdges();
            setPerEdgeVertexReferences();
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

    MeshType meshType() const { return mType; }

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
    bool hasPerVertexCoordinate() const
    {
        return hasPerElementComponent(VERTEX, COORD);
    }

    /**
     * @brief Returns true if the current object has Vertex Normals.
     * @return true if the current object has Vertex Normals.
     */
    bool hasPerVertexNormal() const
    {
        return hasPerElementComponent(VERTEX, NORMAL);
    }

    /**
     * @brief Returns true if the current object has Vertex Colors.
     * @return true if the current object has Vertex Colors.
     */
    bool hasPerVertexColor() const
    {
        return hasPerElementComponent(VERTEX, COLOR);
    }

    /**
     * @brief Returns true if the current object has Vertex Quality.
     * @return true if the current object has Vertex Quality.
     */
    bool hasPerVertexQuality() const
    {
        return hasPerElementComponent(VERTEX, QUALITY);
    }

    /**
     * @brief Returns true if the current object has Vertex Texture Coordinates.
     * @return true if the current object has Vertex Texture Coordinates.
     */
    bool hasPerVertexTexCoord() const
    {
        return hasPerElementComponent(VERTEX, TEXCOORD);
    }

    /**
     * @brief Returns true if the current object has Vertex Custom Components.
     * @return true if the current object has Vertex Custom Components.
     */
    bool hasPerVertexCustomComponents() const
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
    bool hasPerFaceVertexReferences() const
    {
        return hasPerElementComponent(FACE, VREFS);
    }

    bool hasPerFaceNormal() const
    {
        return hasPerElementComponent(FACE, NORMAL);
    }

    bool hasPerFaceColor() const { return hasPerElementComponent(FACE, COLOR); }

    bool hasPerFaceQuality() const
    {
        return hasPerElementComponent(FACE, QUALITY);
    }

    bool hasPerFaceWedgeTexCoords() const
    {
        return hasPerElementComponent(FACE, WEDGE_TEXCOORDS);
    }

    bool hasPerFaceCustomComponents() const
    {
        return hasPerElementComponent(FACE, CUSTOM_COMPONENTS);
    }

    /**
     * @brief Returns true if the current object has Edge Elements.
     * @return true if the current object has Edge Elements.
     */
    bool hasEdges() const { return hasElement(EDGE); }

    bool hasPerEdgeVertexReferences() const
    {
        return hasPerElementComponent(EDGE, VREFS);
    }

    bool hasPerEdgeColor() const { return hasPerElementComponent(EDGE, COLOR); }

    bool hasPerEdgeNormal() const
    {
        return hasPerElementComponent(EDGE, NORMAL);
    }

    bool hasPerEdgeQuality() const
    {
        return hasPerElementComponent(EDGE, QUALITY);
    }

    bool hasPerEdgeCustomComponents() const
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

    void setMeshType(MeshType t) { mType = t; }

    void setElement(Element el, bool b = true) { mElements[el] = b; }

    void setPerElementComponent(Element el, Component c, bool b, DataType t)
    {
        mElements[el]             = b;
        mPerElemComponents[el][c] = b;
        if (b)
            mPerElemComponentsType(el, c) = t;
    }

    void setVertices(bool b = true) { setElement(VERTEX, b); }

    void setPerVertexCoordinate(
        bool     b = true,
        DataType t = PrimitiveType::DOUBLE)
    {
        setPerElementComponent(VERTEX, COORD, b, t);
    }

    void setPerVertexNormal(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setPerElementComponent(VERTEX, NORMAL, b, t);
    }

    void setPerVertexColor(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setPerElementComponent(VERTEX, COLOR, b, t);
    }

    void setPerVertexQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setPerElementComponent(VERTEX, QUALITY, b, t);
    }

    void setPerVertexTexCoord(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setPerElementComponent(VERTEX, TEXCOORD, b, t);
    }

    void setPerVertexCustomComponents(bool b = true)
    {
        setPerElementComponent(
            VERTEX, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setFaces(bool b = true) { setElement(FACE, b); }

    void setPerFaceVertexReferences(bool b = true)
    {
        setPerElementComponent(FACE, VREFS, b, PrimitiveType::NONE);
    }

    void setPerFaceNormal(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setPerElementComponent(FACE, NORMAL, b, t);
    }

    void setPerFaceColor(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setPerElementComponent(FACE, COLOR, b, t);
    }

    void setPerFaceQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setPerElementComponent(FACE, QUALITY, b, t);
    }

    void setPerFaceWedgeTexCoords(
        bool     b = true,
        DataType t = PrimitiveType::FLOAT)
    {
        setPerElementComponent(FACE, WEDGE_TEXCOORDS, b, t);
    }

    void setPerFaceCustomComponents(bool b = true)
    {
        setPerElementComponent(FACE, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setEdges(bool b = true) { setElement(EDGE, b); }

    void setPerEdgeVertexReferences(bool b = true)
    {
        setPerElementComponent(EDGE, VREFS, b, PrimitiveType::NONE);
    }

    void setPerEdgeColor(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setPerElementComponent(EDGE, COLOR, b, t);
    }

    void setPerEdgeNormal(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setPerElementComponent(EDGE, NORMAL, b, t);
    }

    void setPerEdgeQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setPerElementComponent(EDGE, QUALITY, b, t);
    }

    void setPerEdgeCustomComponents(bool b = true)
    {
        setPerElementComponent(EDGE, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setTextures(bool b = true)
    {
        setPerElementComponent(MESH, TEXTURES, b, PrimitiveType::NONE);
    }

    void addPerElementCustomComponent(
        Element            el,
        const std::string& name,
        DataType           t)
    {
        setPerElementComponent(
            el, CUSTOM_COMPONENTS, true, PrimitiveType::NONE);
        mPerElemCustomComponents[el].emplace_back(name, t);
    }

    void clearPerElementCustomComponents(Element el)
    {
        setPerElementComponent(
            el, CUSTOM_COMPONENTS, false, PrimitiveType::NONE);
        mPerElemCustomComponents[el].clear();
    }

    void addPerVertexCustomComponent(const std::string& name, DataType t)
    {
        addPerElementCustomComponent(VERTEX, name, t);
    }

    void clearPerVertexCustomComponents()
    {
        clearPerElementCustomComponents(VERTEX);
    }

    void addPerFaceCustomComponent(const std::string& name, DataType t)
    {
        addPerElementCustomComponent(FACE, name, t);
    }

    void clearPerFaceCustomComponents()
    {
        clearPerElementCustomComponents(FACE);
    }

    void addPerEdgeCustomComponent(const std::string& name, DataType t)
    {
        addPerElementCustomComponent(EDGE, name, t);
    }

    void clearPerEdgeCustomComponents()
    {
        clearPerElementCustomComponents(EDGE);
    }

    /*
     * Getter Component type functions : they are used mostly by save functions
     * to know the type that needs to use to save a given Component
     */

    DataType perElementComponentType(Element el, Component comp) const
    {
        return mPerElemComponentsType(el, comp);
    }

    DataType perVertexCoordinateType() const
    {
        return perElementComponentType(VERTEX, COORD);
    }

    DataType perVertexNormalType() const
    {
        return perElementComponentType(VERTEX, NORMAL);
    }

    DataType perVertexColorType() const
    {
        return perElementComponentType(VERTEX, COLOR);
    }

    DataType perVertexQualityType() const
    {
        return perElementComponentType(VERTEX, QUALITY);
    }

    DataType perVertexTexCoordType() const
    {
        return perElementComponentType(VERTEX, TEXCOORD);
    }

    DataType perFaceNormalType() const
    {
        return perElementComponentType(FACE, NORMAL);
    }

    DataType perFaceColorType() const
    {
        return perElementComponentType(FACE, COLOR);
    }

    DataType perFaceQualityType() const
    {
        return perElementComponentType(FACE, QUALITY);
    }

    DataType perFaceWedgeTexCoordsType() const
    {
        return perElementComponentType(FACE, WEDGE_TEXCOORDS);
    }

    DataType perEdgeNormalType() const
    {
        return perElementComponentType(EDGE, NORMAL);
    }

    DataType perEdgeColorType() const
    {
        return perElementComponentType(EDGE, COLOR);
    }

    DataType perEdgeQualityType() const
    {
        return perElementComponentType(EDGE, QUALITY);
    }

    const std::vector<CustomComponent>& perElementCustomComponents(
        Element el) const
    {
        return mPerElemCustomComponents[el];
    }

    const std::vector<CustomComponent>& perVertexCustomComponents() const
    {
        return mPerElemCustomComponents[VERTEX];
    }

    const std::vector<CustomComponent>& perFaceCustomComponents() const
    {
        return mPerElemCustomComponents[FACE];
    }

    const std::vector<CustomComponent>& perEdgeCustomComponents() const
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
        if (info.hasPerVertexColor()) {
            if (!enableIfPerVertexColorOptional(m)) {
                info.setPerVertexColor(false);
            }
        }
        if (info.hasPerVertexNormal()) {
            if (!enableIfPerVertexNormalOptional(m)) {
                info.setPerVertexNormal(false);
            }
        }
        if (info.hasPerVertexQuality()) {
            if (!enableIfPerVertexQualityOptional(m)) {
                info.setPerVertexQuality(false);
            }
        }
        if (info.hasPerVertexTexCoord()) {
            if (!enableIfPerVertexTexCoordOptional(m)) {
                info.setPerVertexTexCoord(false);
            }
        }
        if (info.hasPerVertexCustomComponents()) {
            if constexpr (HasPerVertexCustomComponents<MeshType>) {
                for (const auto& cc : info.perVertexCustomComponents()) {
                    addPerVertexCustomComponent(m, cc);
                }
            }
            else {
                info.clearPerVertexCustomComponents();
            }
        }
    }
    else {
        info.setVertices(false);
    }

    if constexpr (HasFaces<MeshType>) {
        if (info.hasFaces()) {
            if (info.hasPerFaceColor()) {
                if (!enableIfPerFaceColorOptional(m)) {
                    info.setPerFaceColor(false);
                }
            }
            if (info.hasPerFaceNormal()) {
                if (!enableIfPerFaceNormalOptional(m)) {
                    info.setPerFaceNormal(false);
                }
            }
            if (info.hasPerFaceQuality()) {
                if (!enableIfPerFaceQualityOptional(m)) {
                    info.setPerFaceQuality(false);
                }
            }
            if (info.hasPerFaceWedgeTexCoords()) {
                if (!enableIfPerFaceWedgeTexCoordsOptional(m)) {
                    info.setPerFaceWedgeTexCoords(false);
                }
            }
            if (info.hasPerFaceCustomComponents()) {
                if constexpr (HasPerFaceCustomComponents<MeshType>) {
                    for (const auto& cc : info.perFaceCustomComponents()) {
                        addPerFaceCustomComponent(m, cc);
                    }
                }
                else {
                    info.clearPerFaceCustomComponents();
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
            if (info.hasPerEdgeColor()) {
                if (!enableIfPerEdgeColorOptional(m)) {
                    info.setPerEdgeColor(false);
                }
            }
            if (info.hasPerEdgeNormal()) {
                if (!enableIfPerEdgeNormalOptional(m)) {
                    info.setPerEdgeNormal(false);
                }
            }
            if (info.hasPerEdgeQuality()) {
                if (!enableIfPerEdgeQualityOptional(m)) {
                    info.setPerEdgeQuality(false);
                }
            }
            if (info.hasPerEdgeCustomComponents()) {
                if constexpr (HasPerEdgeCustomComponents<MeshType>) {
                    for (const auto& cc : info.perEdgeCustomComponents()) {
                        addPerEdgeCustomComponent(m, cc);
                    }
                }
                else {
                    info.clearPerEdgeCustomComponents();
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
