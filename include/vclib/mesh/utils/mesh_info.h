/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_UTILS_MESH_INFO_H
#define VCL_MESH_UTILS_MESH_INFO_H

#include <array>
#include <list>
#include <string>
#include <typeindex>
#include <vector>
#include <bitset>

#include <vclib/mesh/requirements.h>
#include <vclib/types.h>

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
 * MeshInfo info(m); // compute the default MeshInfo object from the
 * Mesh
 *
 * info.setVertexCoords(true, MeshInfo::FLOAT); // force to store vertex
 * coords using floats info.setVertexColors(false); // do not store
 *
 * vcl::save(m, "meshfile.ply", info);
 * @endcode
 *
 * @ingroup load_save
 * @ingroup miscellaneous
 */
class MeshInfo
{
public:
    /**
     * @brief Enum used to describe the type of the Mesh - by default, a mesh is
     * considered polygonal
     */
    enum MeshType { TRIANGLE_MESH, QUAD_MESH, POLYGON_MESH };

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
    struct CustomComponent {
        std::string name;
        DataType type;
        CustomComponent(std::string n, DataType t) : name(n), type(t) {}
    };

private:
    // Tell if elements are present in the file
    std::bitset<NUM_ELEMENTS> elements = {false};

    // Tell if per element components are present in the file
    std::array<std::bitset<NUM_COMPONENTS>, NUM_ELEMENTS> perElemComponents = {
        false};

    // Tell the data type for each component of each element
    Eigen::Matrix<DataType, NUM_ELEMENTS, NUM_COMPONENTS> perElemComponentsType;

    // Store name and type of per element custom components
    std::array<std::vector<CustomComponent>, NUM_ELEMENTS>
        perElemCustomComponents;

    // Mesh Type
    MeshType type = POLYGON_MESH;

public:
    /**
     * @brief Default constructor.
     *
     * All the Elements/Components are disabled, their type is set to
     * DataType::NONE and the Mesh Type is set to MeshType::POLYGON_MESH.
     */
    MeshInfo() { perElemComponentsType.fill(NONE); }

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
        if constexpr (vcl::HasPerVertexNormal<Mesh>) {
            if (vcl::isPerVertexNormalAvailable(m)) {
                setVertexNormals(
                    true,
                    getType<
                        typename Mesh::VertexType::NormalType::ScalarType>());
            }
        }
        if constexpr (vcl::HasPerVertexColor<Mesh>) {
            if (vcl::isPerVertexColorAvailable(m)) {
                setVertexColors(true, UCHAR);
            }
        }
        if constexpr (vcl::HasPerVertexQuality<Mesh>) {
            if (vcl::isPerVertexQualityAvailable(m)) {
                setVertexQuality(
                    true, getType<typename Mesh::VertexType::QualityType>());
            }
        }
        if constexpr (vcl::HasPerVertexTexCoord<Mesh>) {
            if (vcl::isPerVertexTexCoordAvailable(m)) {
                setVertexTexCoords(
                    true,
                    getType<
                        typename Mesh::VertexType::TexCoordType::ScalarType>());
            }
        }
        if constexpr (vcl::HasPerVertexCustomComponents<Mesh>) {
            auto names = m.perVertexCustomComponentNames();
            for (auto& name : names) {
                DataType dt = getType(m.perVertexCustomComponentType(name));
                if (dt != NONE) {
                    addVertexCustomComponent(name, dt);
                }
            }
        }

        if constexpr (vcl::HasFaces<Mesh>) {
            setFaces();
            setFaceVRefs();
            if (vcl::HasTriangles<Mesh>) {
                setTriangleMesh();
            }
            else if (vcl::HasQuads<Mesh>) {
                setQuadMesh();
            }
            else {
                setPolygonMesh();
            }
            if constexpr (vcl::HasPerFaceNormal<Mesh>) {
                if (vcl::isPerFaceNormalAvailable(m)) {
                    setFaceNormals(
                        true,
                        getType<
                            typename Mesh::FaceType::NormalType::ScalarType>());
                }
            }
            if constexpr (vcl::HasPerFaceColor<Mesh>) {
                if (vcl::isPerFaceColorAvailable(m)) {
                    setFaceColors(true, UCHAR);
                }
            }
            if constexpr (vcl::HasPerFaceQuality<Mesh>) {
                if (vcl::isPerFaceQualityAvailable(m)) {
                    setFaceQuality(
                        true, getType<typename Mesh::FaceType::QualityType>());
                }
            }
            if constexpr (vcl::HasPerFaceWedgeTexCoords<Mesh>) {
                if (vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                    setFaceWedgeTexCoords(
                        true,
                        getType<typename Mesh::FaceType::WedgeTexCoordType::
                                    ScalarType>());
                }
            }
            if constexpr(vcl::HasPerFaceCustomComponents<Mesh>) {
                auto names = m.perFaceCustomComponentNames();
                for (auto& name : names) {
                    DataType dt = getType(m.perFaceCustomComponentType(name));
                    if (dt != NONE) {
                        addFaceCustomComponent(name, dt);
                    }
                }
            }
        }

        if constexpr (vcl::HasEdges<Mesh>) {
            setEdges();
            setEdgeVRefs();
            // if constexpr (vcl::HasPerEdgeColor<Mesh>)
            //     if (vcl::isPerEdgeColorAvailable(m))
            //         setEdgeColors(true, UCHAR);
        }

        if constexpr (vcl::HasTexturePaths<Mesh>) {
            if (m.textureNumber() > 0) {
                setTextures(true);
            }
        }
    }

    /**
     * @brief Returns true if the current object has Mesh type set to
     * MeshType::TRIANGLE_MESH.
     * @return true if the current Mesh type is set to MeshType::TRIANGLE_MESH.
     */
    bool isTriangleMesh() const { return type == TRIANGLE_MESH; }

    /**
     * @brief Returns true if the current object has Mesh type set to
     * MeshType::QUAD_MESH.
     * @return true if the current Mesh type is set to MeshType::QUAD_MESH.
     */
    bool isQuadMesh() const { return type == QUAD_MESH; }

    /**
     * @brief Returns true if the current object has Mesh type set to
     * MeshType::POLYGON_MESH.
     * @return true if the current Mesh type is set to MeshType::POLYGON_MESH.
     */
    bool isPolygonMesh() const { return type == POLYGON_MESH; }

    /*
     * Getter Elements/Components functions: they are used mostly after the
     * loading of a Mesh from a file, to know if Elements/Components have been
     * loaded.
     */

    bool hasElement(Element el) const { return elements[el]; }

    bool hasPerElementComponent(Element el, Component comp) const
    {
        return perElemComponents[el][comp];
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

    bool hasTextures() const { return hasPerElementComponent(MESH, TEXTURES); }

    /*
     * Setter functions: they are used by the load functions to tell which
     * Elements/Components are loaded from a file, and they can be used by the
     * user that wants to save in a file only a specific set of
     * Elements/Components of a Mesh.
     */

    void setTriangleMesh() { type = TRIANGLE_MESH; }

    void setQuadMesh() { type = QUAD_MESH; }

    void setPolygonMesh() { type = POLYGON_MESH; }

    void setMeshType(MeshType t) { type = t; }

    void setElement(Element el, bool b = true) { elements[el] = b; }

    void setElementComponents(Element el, Component c, bool b, DataType t)
    {
        elements[el] = b;
        perElemComponents[el][c] = b;
        if (b)
            perElemComponentsType(el, c) = t;
    }

    void setVertices(bool b = true) { setElement(VERTEX, b); }

    void setVertexCoords(bool b = true, DataType t = DOUBLE)
    {
        setElementComponents(VERTEX, COORD, b, t);
    }

    void setVertexNormals(bool b = true, DataType t = FLOAT)
    {
        setElementComponents(VERTEX, NORMAL, b, t);
    }

    void setVertexColors(bool b = true, DataType t = UCHAR)
    {
        setElementComponents(VERTEX, COLOR, b, t);
    }

    void setVertexQuality(bool b = true, DataType t = DOUBLE)
    {
        setElementComponents(VERTEX, QUALITY, b, t);
    }

    void setVertexTexCoords(bool b = true, DataType t = FLOAT)
    {
        setElementComponents(VERTEX, TEXCOORD, b, t);
    }

    void setVertexCustomComponents(bool b = true)
    {
        setElementComponents(VERTEX, CUSTOM_COMPONENTS, b, NONE);
    }

    void setFaces(bool b = true) { setElement(FACE, b); }

    void setFaceVRefs(bool b = true)
    {
        setElementComponents(FACE, VREFS, b, NONE);
    }

    void setFaceNormals(bool b = true, DataType t = FLOAT)
    {
        setElementComponents(FACE, NORMAL, b, t);
    }

    void setFaceColors(bool b = true, DataType t = UCHAR)
    {
        setElementComponents(FACE, COLOR, b, t);
    }

    void setFaceQuality(bool b = true, DataType t = DOUBLE)
    {
        setElementComponents(FACE, QUALITY, b, t);
    }

    void setFaceWedgeTexCoords(bool b = true, DataType t = FLOAT)
    {
        setElementComponents(FACE, WEDGE_TEXCOORDS, b, t);
    }

    void setFaceCustomComponents(bool b = true)
    {
        setElementComponents(FACE, CUSTOM_COMPONENTS, b, NONE);
    }

    void setEdges(bool b = true) { setElement(EDGE, b); }

    void setEdgeVRefs(bool b = true)
    {
        setElementComponents(EDGE, VREFS, b, NONE);
    }

    void setEdgeColors(bool b = true, DataType t = UCHAR)
    {
        setElementComponents(EDGE, COLOR, b, t);
    }

    void setTextures(bool b = true)
    {
        setElementComponents(MESH, TEXTURES, b, NONE);
    }

    void addElementCustomComponent(
        Element el, const std::string& name, DataType t)
    {
        setElementComponents(el, CUSTOM_COMPONENTS, true, NONE);
        perElemCustomComponents[el].emplace_back(name, t);
    }

    void clearElementCustomComponents(Element el)
    {
        setElementComponents(el, CUSTOM_COMPONENTS, false, NONE);
        perElemCustomComponents[el].clear();
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

    /*
     * Getter Component type functions : they are used mostly by save functions
     * to know the type that needs to use to save a given Component
     */

    DataType elementComponentType(Element el, Component comp) const
    {
        return perElemComponentsType(el, comp);
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

    const std::vector<CustomComponent>& vertexCustomComponents() const
    {
        return perElemCustomComponents[VERTEX];
    }

    const std::vector<CustomComponent>& faceCustomComponents() const
    {
        return perElemCustomComponents[FACE];
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
    MeshInfo intersect(const MeshInfo& info) const
    {
        MeshInfo res;
        for (uint i = 0; i < NUM_ELEMENTS; ++i) {
            res.elements[i] = elements[i] && info.elements[i];
            for (uint j = 0; j < NUM_COMPONENTS; ++j) {
                res.perElemComponents[i][j] =
                    perElemComponents[i][j] && info.perElemComponents[i][j];

                if (res.perElemComponents[i][j]){
                    res.perElemComponentsType(i, j) =
                        perElemComponentsType(i, j);
                }
            }
        }

        if (type == info.type){
            res.type = type;
        }
        res.perElemCustomComponents = perElemCustomComponents;

        return res;
    }

    void reset()
    {
        elements.reset();
        for (auto& comp : perElemComponents)
            comp.reset();
        perElemComponentsType.fill(NONE);

        for (auto& v : perElemCustomComponents)
            v.clear();

        type = TRIANGLE_MESH;
    }

private:
    /**
     * @brief Given the template T, returns the correspoding enum DataType value
     * of T.
     *
     * Returns DataType::NONE if the type T was not part of the type
     * sypported by the DataType enum.
     * @return
     */
    template<typename T>
    static DataType getType()
    {
        if constexpr (std::is_same_v<T, char>) return CHAR;
        if constexpr (std::is_same_v<T, unsigned char>) return UCHAR;
        if constexpr (std::is_same_v<T, short>) return SHORT;
        if constexpr (std::is_same_v<T, unsigned short>) return USHORT;
        if constexpr (std::is_same_v<T, int>) return INT;
        if constexpr (std::is_same_v<T, uint>) return UINT;
        if constexpr (std::is_integral_v<T>) return INT; // fallback to int
        if constexpr (std::is_same_v<T, float>) return FLOAT;
        if constexpr (std::is_same_v<T, double>) return DOUBLE;
        if constexpr (std::is_floating_point_v<T>)
            return FLOAT; // fallback to float
        return NONE;
    }

    static DataType getType(std::type_index ti)
    {
        if (ti == typeid(char)) return CHAR;
        if (ti == typeid(unsigned char)) return UCHAR;
        if (ti == typeid(short)) return SHORT;
        if (ti == typeid(unsigned short)) return USHORT;
        if (ti == typeid(int)) return INT;
        if (ti == typeid(uint)) return UINT;
        if (ti == typeid(float)) return FLOAT;
        if (ti == typeid(double)) return DOUBLE;
        return NONE;
    }
};

template<MeshConcept MeshType>
void addPerVertexCustomComponent(
    MeshType&                        m,
    const MeshInfo::CustomComponent& cc)
{
    switch (cc.type) {
    case CHAR:
        m.template addPerVertexCustomComponent<char>(cc.name);
        break;
    case UCHAR:
        m.template addPerVertexCustomComponent<unsigned char>(cc.name);
        break;
    case SHORT:
        m.template addPerVertexCustomComponent<short>(cc.name);
        break;
    case USHORT:
        m.template addPerVertexCustomComponent<unsigned short>(cc.name);
        break;
    case INT:
        m.template addPerVertexCustomComponent<int>(cc.name);
        break;
    case UINT:
        m.template addPerVertexCustomComponent<uint>(cc.name);
        break;
    case FLOAT:
        m.template addPerVertexCustomComponent<float>(cc.name);
        break;
    case DOUBLE:
        m.template addPerVertexCustomComponent<double>(cc.name);
        break;
    default: assert(0);
    }
}

template<FaceMeshConcept MeshType>
void addPerFaceCustomComponent(MeshType& m, const MeshInfo::CustomComponent& cc)
{
    switch (cc.type) {
    case CHAR:
        m.template addPerFaceCustomComponent<char>(cc.name);
        break;
    case UCHAR:
        m.template addPerFaceCustomComponent<unsigned char>(cc.name);
        break;
    case SHORT:
        m.template addPerFaceCustomComponent<short>(cc.name);
        break;
    case USHORT:
        m.template addPerFaceCustomComponent<unsigned short>(cc.name);
        break;
    case INT:
        m.template addPerFaceCustomComponent<int>(cc.name);
        break;
    case UINT:
        m.template addPerFaceCustomComponent<uint>(cc.name);
        break;
    case FLOAT:
        m.template addPerFaceCustomComponent<float>(cc.name);
        break;
    case DOUBLE:
        m.template addPerFaceCustomComponent<double>(cc.name);
        break;
    default: assert(0);
    }
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
            if (!vcl::enableIfPerVertexColorOptional(m)) {
                info.setVertexColors(false);
            }
        }
        if (info.hasVertexNormals()) {
            if (!vcl::enableIfPerVertexNormalOptional(m)) {
                info.setVertexNormals(false);
            }
        }
        if (info.hasVertexQuality()) {
            if (!vcl::enableIfPerVertexQualityOptional(m)) {
                info.setVertexQuality(false);
            }
        }
        if (info.hasVertexTexCoords()) {
            if (!vcl::enableIfPerVertexTexCoordOptional(m)) {
                info.setVertexTexCoords(false);
            }
        }
        if (info.hasVertexCustomComponents()) {
            if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
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

    if (info.hasFaces()) {
        if (info.hasFaceColors()) {
            if (!vcl::enableIfPerFaceColorOptional(m)) {
                info.setFaceColors(false);
            }
        }
        if (info.hasFaceNormals()) {
            if (!vcl::enableIfPerFaceNormalOptional(m)) {
                info.setFaceNormals(false);
            }
        }
        if (info.hasFaceQuality()) {
            if (!vcl::enableIfPerFaceQualityOptional(m)) {
                info.setFaceQuality(false);
            }
        }
        if (info.hasFaceWedgeTexCoords()) {
            if (!vcl::enableIfPerFaceWedgeTexCoordsOptional(m)) {
                info.setFaceWedgeTexCoords(false);
            }
        }
        if (info.hasFaceCustomComponents()) {
            if constexpr (vcl::HasPerFaceCustomComponents<MeshType>) {
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

} // namespace vcl

#endif // VCL_MESH_UTILS_MESH_INFO_H
