// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_MESH_INFO_H
#define VCL_SPACE_COMPLEX_MESH_INFO_H

#include <vclib/base.h>
#include <vclib/mesh.h>

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
 * info.setVertexPosition(true, MeshInfo::FLOAT); // force to store vertex
 *                                                   // positions using floats
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
     * be stored in the MESH.
     */
    enum Element { VERTEX, FACE, EDGE, MESH, NUM_ELEMENTS };

    /**
     * @brief Enum used to describe the type of Components that each Element can
     * have.
     */
    enum Component {
        POSITION,
        VREFS,
        BIT_FLAGS,
        NORMAL,
        TANGENT,
        COLOR,
        QUALITY,
        TEXCOORD,
        MATERIAL_INDEX,
        WEDGE_TEXCOORDS,
        CUSTOM_COMPONENTS,
        MATERIALS,
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

    // Tell if element components are present in the file
    std::array<std::bitset<NUM_COMPONENTS>, NUM_ELEMENTS> mElemComponents = {
        false};

    // Tell the data type for each component of each element
    Eigen::Matrix<DataType, NUM_ELEMENTS, NUM_COMPONENTS> mElemComponentsType;

    // Store name and type of element custom components
    std::array<std::vector<CustomComponent>, NUM_ELEMENTS>
        mElemCustomComponents;

    // Mesh Type
    MeshType mType = MeshType::UNKNOWN;

public:
    /**
     * @brief Default constructor.
     *
     * All the Elements/Components are disabled, their type is set to
     * DataType::NONE and the Mesh Type is set to MeshType::POLYGON_MESH.
     */
    MeshInfo() { mElemComponentsType.fill(PrimitiveType::NONE); }

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
        setVertexPosition(
            true,
            getType<typename Mesh::VertexType::PositionType::ScalarType>());
        setVertexBitFlags();
        if constexpr (HasPerVertexNormal<Mesh>) {
            if (isPerVertexNormalAvailable(m)) {
                setVertexNormal(
                    true,
                    getType<
                        typename Mesh::VertexType::NormalType::ScalarType>());
            }
        }
        if constexpr (HasPerVertexTangent<Mesh>) {
            if (isPerVertexTangentAvailable(m)) {
                setVertexTangent(
                    true,
                    getType<
                        typename Mesh::VertexType::TangentType::ScalarType>());
            }
        }
        if constexpr (HasPerVertexColor<Mesh>) {
            if (isPerVertexColorAvailable(m)) {
                setVertexColor(true, PrimitiveType::UCHAR);
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
                setVertexTexCoord(
                    true,
                    getType<
                        typename Mesh::VertexType::TexCoordType::ScalarType>());
            }
        }
        if constexpr (HasPerVertexMaterialIndex<Mesh>) {
            if (isPerVertexMaterialIndexAvailable(m)) {
                setVertexMaterialIndex(true, PrimitiveType::UINT);
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
            setFaceVertexReferences();
            setFaceBitFlags();
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
                    setFaceNormal(
                        true,
                        getType<
                            typename Mesh::FaceType::NormalType::ScalarType>());
                }
            }
            if constexpr (HasPerFaceColor<Mesh>) {
                if (isPerFaceColorAvailable(m)) {
                    setFaceColor(true, PrimitiveType::UCHAR);
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
            if constexpr (HasPerFaceMaterialIndex<Mesh>) {
                if (isPerFaceMaterialIndexAvailable(m)) {
                    setFaceMaterialIndex(true, PrimitiveType::UINT);
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
            setEdgeVertexReferences();
            setEdgeBitFlags();
            // if constexpr (HasPerEdgeColor<Mesh>)
            //     if (isPerEdgeColorAvailable(m))
            //         setEdgeColors(true, UCHAR);
        }

        if constexpr (HasMaterials<Mesh>) {
            if (m.materialCount() > 0) {
                setMaterials(true);
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
        for (auto& comp : mElemComponents)
            comp.reset();
        mElemComponentsType.fill(PrimitiveType::NONE);

        for (auto& v : mElemCustomComponents)
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

    bool hasElementComponent(Element el, Component comp) const
    {
        return mElemComponents[el][comp];
    }

    /**
     * @brief Returns true if the current object has Vertex Elements.
     * @return true if the current object has Vertex Elements.
     */
    bool hasVertices() const { return hasElement(VERTEX); }

    /**
     * @brief Returns true if the current object has Vertex Positions.
     * @return true if the current object has Vertex Positions.
     */
    bool hasVertexPosition() const
    {
        return hasElementComponent(VERTEX, POSITION);
    }

    /**
     * @brief Returns true if the current object has Vertex Bit Flags.
     * @return true if the current object has Vertex Bit Flags.
     */
    bool hasVertexBitFlags() const
    {
        return hasElementComponent(VERTEX, BIT_FLAGS);
    }

    /**
     * @brief Returns true if the current object has Vertex Normals.
     * @return true if the current object has Vertex Normals.
     */
    bool hasVertexNormal() const { return hasElementComponent(VERTEX, NORMAL); }

    /**
     * @brief Returns true if the current object has Vertex Tangents.
     * @return true if the current object has Vertex Tangents.
     */
    bool hasVertexTangent() const
    {
        return hasElementComponent(VERTEX, TANGENT);
    }

    /**
     * @brief Returns true if the current object has Vertex Colors.
     * @return true if the current object has Vertex Colors.
     */
    bool hasVertexColor() const { return hasElementComponent(VERTEX, COLOR); }

    /**
     * @brief Returns true if the current object has Vertex Quality.
     * @return true if the current object has Vertex Quality.
     */
    bool hasVertexQuality() const
    {
        return hasElementComponent(VERTEX, QUALITY);
    }

    /**
     * @brief Returns true if the current object has Vertex Texture Coordinates.
     * @return true if the current object has Vertex Texture Coordinates.
     */
    bool hasVertexTexCoord() const
    {
        return hasElementComponent(VERTEX, TEXCOORD);
    }

    /**
     * @brief Returns true if the current object has Vertex Material Index.
     * @return true if the current object has Vertex Material Index.
     */
    bool hasVertexMaterialIndex() const
    {
        return hasElementComponent(VERTEX, MATERIAL_INDEX);
    }

    /**
     * @brief Returns true if the current object has Vertex Custom Components.
     * @return true if the current object has Vertex Custom Components.
     */
    bool hasVertexCustomComponents() const
    {
        return hasElementComponent(VERTEX, CUSTOM_COMPONENTS);
    }

    /**
     * @brief Returns true if the current object has Face Elements.
     * @return true if the current object has Face Elements.
     */
    bool hasFaces() const { return hasElement(FACE); }

    /**
     * @brief Returns true if the current object has Face Vertex References.
     * @return true if the current object has Face Vertex References.
     */
    bool hasFaceVertexReferences() const
    {
        return hasElementComponent(FACE, VREFS);
    }

    /**
     * @brief Returns true if the current object has Face Bit Flags.
     * @return true if the current object has Face Bit Flags.
     */
    bool hasFaceBitFlags() const
    {
        return hasElementComponent(FACE, BIT_FLAGS);
    }

    bool hasFaceNormal() const { return hasElementComponent(FACE, NORMAL); }

    bool hasFaceColor() const { return hasElementComponent(FACE, COLOR); }

    bool hasFaceQuality() const { return hasElementComponent(FACE, QUALITY); }

    bool hasFaceWedgeTexCoords() const
    {
        return hasElementComponent(FACE, WEDGE_TEXCOORDS);
    }

    /**
     * @brief Returns true if the current object has Face Material Index.
     * @return true if the current object has Face Material Index.
     */
    bool hasFaceMaterialIndex() const
    {
        return hasElementComponent(FACE, MATERIAL_INDEX);
    }

    bool hasFaceCustomComponents() const
    {
        return hasElementComponent(FACE, CUSTOM_COMPONENTS);
    }

    /**
     * @brief Returns true if the current object has Edge Elements.
     * @return true if the current object has Edge Elements.
     */
    bool hasEdges() const { return hasElement(EDGE); }

    bool hasEdgeVertexReferences() const
    {
        return hasElementComponent(EDGE, VREFS);
    }

    /**
     * @brief Returns true if the current object has Edge Bit Flags.
     * @return true if the current object has Edge Bit Flags.
     */
    bool hasEdgeBitFlags() const
    {
        return hasElementComponent(EDGE, BIT_FLAGS);
    }

    bool hasEdgeColor() const { return hasElementComponent(EDGE, COLOR); }

    bool hasEdgeNormal() const { return hasElementComponent(EDGE, NORMAL); }

    bool hasEdgeQuality() const { return hasElementComponent(EDGE, QUALITY); }

    bool hasEdgeCustomComponents() const
    {
        return hasElementComponent(EDGE, CUSTOM_COMPONENTS);
    }

    bool hasMaterials() const { return hasElementComponent(MESH, MATERIALS); }

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

    void setElementComponent(Element el, Component c, bool b, DataType t)
    {
        mElements[el]          = b;
        mElemComponents[el][c] = b;
        if (b)
            mElemComponentsType(el, c) = t;
    }

    void setVertices(bool b = true) { setElement(VERTEX, b); }

    void setVertexPosition(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponent(VERTEX, POSITION, b, t);
    }

    void setVertexBitFlags(bool b = true, DataType t = PrimitiveType::UINT)
    {
        setElementComponent(VERTEX, BIT_FLAGS, b, t);
    }

    void setVertexNormal(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponent(VERTEX, NORMAL, b, t);
    }

    void setVertexTangent(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponent(VERTEX, TANGENT, b, t);
    }

    void setVertexColor(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setElementComponent(VERTEX, COLOR, b, t);
    }

    void setVertexQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponent(VERTEX, QUALITY, b, t);
    }

    void setVertexTexCoord(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponent(VERTEX, TEXCOORD, b, t);
    }

    void setVertexMaterialIndex(
        bool     b = true,
        DataType t = PrimitiveType::USHORT)
    {
        setElementComponent(VERTEX, MATERIAL_INDEX, b, t);
    }

    void setVertexCustomComponents(bool b = true)
    {
        setElementComponent(VERTEX, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setFaces(bool b = true) { setElement(FACE, b); }

    void setFaceVertexReferences(bool b = true)
    {
        setElementComponent(FACE, VREFS, b, PrimitiveType::NONE);
    }

    void setFaceBitFlags(bool b = true, DataType t = PrimitiveType::UINT)
    {
        setElementComponent(FACE, BIT_FLAGS, b, t);
    }

    void setFaceNormal(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponent(FACE, NORMAL, b, t);
    }

    void setFaceColor(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setElementComponent(FACE, COLOR, b, t);
    }

    void setFaceQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponent(FACE, QUALITY, b, t);
    }

    void setFaceWedgeTexCoords(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponent(FACE, WEDGE_TEXCOORDS, b, t);
    }

    void setFaceMaterialIndex(bool b = true, DataType t = PrimitiveType::USHORT)
    {
        setElementComponent(FACE, MATERIAL_INDEX, b, t);
    }

    void setFaceCustomComponents(bool b = true)
    {
        setElementComponent(FACE, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setEdges(bool b = true) { setElement(EDGE, b); }

    void setEdgeVertexReferences(bool b = true)
    {
        setElementComponent(EDGE, VREFS, b, PrimitiveType::NONE);
    }

    void setEdgeBitFlags(bool b = true, DataType t = PrimitiveType::UINT)
    {
        setElementComponent(EDGE, BIT_FLAGS, b, t);
    }

    void setEdgeColor(bool b = true, DataType t = PrimitiveType::UCHAR)
    {
        setElementComponent(EDGE, COLOR, b, t);
    }

    void setEdgeNormal(bool b = true, DataType t = PrimitiveType::FLOAT)
    {
        setElementComponent(EDGE, NORMAL, b, t);
    }

    void setEdgeQuality(bool b = true, DataType t = PrimitiveType::DOUBLE)
    {
        setElementComponent(EDGE, QUALITY, b, t);
    }

    void setEdgeCustomComponents(bool b = true)
    {
        setElementComponent(EDGE, CUSTOM_COMPONENTS, b, PrimitiveType::NONE);
    }

    void setMaterials(bool b = true)
    {
        setElementComponent(MESH, MATERIALS, b, PrimitiveType::NONE);
    }

    void addElementCustomComponent(
        Element            el,
        const std::string& name,
        DataType           t)
    {
        setElementComponent(el, CUSTOM_COMPONENTS, true, PrimitiveType::NONE);
        mElemCustomComponents[el].emplace_back(name, t);
    }

    void clearElementCustomComponents(Element el)
    {
        setElementComponent(el, CUSTOM_COMPONENTS, false, PrimitiveType::NONE);
        mElemCustomComponents[el].clear();
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
        return mElemComponentsType(el, comp);
    }

    DataType vertexPositionType() const
    {
        return elementComponentType(VERTEX, POSITION);
    }

    DataType vertexBitFlagsType() const
    {
        return elementComponentType(VERTEX, BIT_FLAGS);
    }

    DataType vertexNormalType() const
    {
        return elementComponentType(VERTEX, NORMAL);
    }

    DataType vertexTangentType() const
    {
        return elementComponentType(VERTEX, TANGENT);
    }

    DataType vertexColorType() const
    {
        return elementComponentType(VERTEX, COLOR);
    }

    DataType vertexQualityType() const
    {
        return elementComponentType(VERTEX, QUALITY);
    }

    DataType vertexTexCoordType() const
    {
        return elementComponentType(VERTEX, TEXCOORD);
    }

    DataType vertexMaterialIndexType() const
    {
        return elementComponentType(VERTEX, MATERIAL_INDEX);
    }

    DataType faceBitFlagsType() const
    {
        return elementComponentType(FACE, BIT_FLAGS);
    }

    DataType faceNormalType() const
    {
        return elementComponentType(FACE, NORMAL);
    }

    DataType faceColorType() const { return elementComponentType(FACE, COLOR); }

    DataType faceQualityType() const
    {
        return elementComponentType(FACE, QUALITY);
    }

    DataType faceWedgeTexCoordsType() const
    {
        return elementComponentType(FACE, WEDGE_TEXCOORDS);
    }

    DataType faceMaterialIndexType() const
    {
        return elementComponentType(FACE, MATERIAL_INDEX);
    }

    DataType edgeBitFlagsType() const
    {
        return elementComponentType(EDGE, BIT_FLAGS);
    }

    DataType edgeNormalType() const
    {
        return elementComponentType(EDGE, NORMAL);
    }

    DataType edgeColorType() const { return elementComponentType(EDGE, COLOR); }

    DataType edgeQualityType() const
    {
        return elementComponentType(EDGE, QUALITY);
    }

    const std::vector<CustomComponent>& elementCustomComponents(
        Element el) const
    {
        return mElemCustomComponents[el];
    }

    const std::vector<CustomComponent>& vertexCustomComponents() const
    {
        return mElemCustomComponents[VERTEX];
    }

    const std::vector<CustomComponent>& faceCustomComponents() const
    {
        return mElemCustomComponents[FACE];
    }

    const std::vector<CustomComponent>& edgeCustomComponents() const
    {
        return mElemCustomComponents[EDGE];
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
                res.mElemComponents[i][j] =
                    mElemComponents[i][j] && info.mElemComponents[i][j];

                if (res.mElemComponents[i][j]) {
                    res.mElemComponentsType(i, j) = mElemComponentsType(i, j);
                }
            }
        }

        if (mType == info.mType) {
            res.mType = mType;
        }
        res.mElemCustomComponents = mElemCustomComponents;

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
void addElementCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
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
void addVertexCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
{
    addElementCustomComponent<ElemId::VERTEX>(m, cc);
}

template<FaceMeshConcept MeshType>
void addFaceCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
{
    addElementCustomComponent<ElemId::FACE>(m, cc);
}

template<EdgeMeshConcept MeshType>
void addEdgeCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
{
    addElementCustomComponent<ElemId::EDGE>(m, cc);
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
        if (info.hasVertexColor()) {
            if (!enableIfPerVertexColorOptional(m)) {
                info.setVertexColor(false);
            }
        }
        if (info.hasVertexNormal()) {
            if (!enableIfPerVertexNormalOptional(m)) {
                info.setVertexNormal(false);
            }
        }
        if (info.hasVertexQuality()) {
            if (!enableIfPerVertexQualityOptional(m)) {
                info.setVertexQuality(false);
            }
        }
        if (info.hasVertexTangent()) {
            if (!enableIfPerVertexTangentOptional(m)) {
                info.setVertexTangent(false);
            }
        }
        if (info.hasVertexTexCoord()) {
            if (!enableIfPerVertexTexCoordOptional(m)) {
                info.setVertexTexCoord(false);
            }
        }
        if (info.hasVertexMaterialIndex()) {
            if (!enableIfPerVertexMaterialIndexOptional(m)) {
                info.setVertexMaterialIndex(false);
            }
        }
        if (info.hasVertexCustomComponents()) {
            if constexpr (HasPerVertexCustomComponents<MeshType>) {
                for (const auto& cc : info.vertexCustomComponents()) {
                    addVertexCustomComponent(m, cc);
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
            if (info.hasFaceColor()) {
                if (!enableIfPerFaceColorOptional(m)) {
                    info.setFaceColor(false);
                }
            }
            if (info.hasFaceNormal()) {
                if (!enableIfPerFaceNormalOptional(m)) {
                    info.setFaceNormal(false);
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
            if (info.hasFaceMaterialIndex()) {
                if (!enableIfPerFaceMaterialIndexOptional(m)) {
                    info.setFaceMaterialIndex(false);
                }
            }
            if (info.hasFaceCustomComponents()) {
                if constexpr (HasPerFaceCustomComponents<MeshType>) {
                    for (const auto& cc : info.faceCustomComponents()) {
                        addFaceCustomComponent(m, cc);
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
            if (info.hasEdgeColor()) {
                if (!enableIfPerEdgeColorOptional(m)) {
                    info.setEdgeColor(false);
                }
            }
            if (info.hasEdgeNormal()) {
                if (!enableIfPerEdgeNormalOptional(m)) {
                    info.setEdgeNormal(false);
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
                        addEdgeCustomComponent(m, cc);
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
